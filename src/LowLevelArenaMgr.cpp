/*
 * Copyright (C) 2026+ AzerothCore Project
 * Released under GNU AGPL v3 license.
 */

#include "LowLevelArenaMgr.h"
#include "LowLevelArenaConfig.h"
#include "Chat.h"
#include "Creature.h"
#include "GameTime.h"
#include "Log.h"
#include "Map.h"
#include "MapMgr.h"
#include "Player.h"
#include <cmath>

LowLevelArenaMgr* LowLevelArenaMgr::instance()
{
    static LowLevelArenaMgr instance;
    return &instance;
}

void LowLevelArenaMgr::HandleSummon(Player* player)
{
    if (!player || !sLowLevelArenaConfig->Enable || !sLowLevelArenaConfig->NpcEnable)
        return;

    // Check if in combat or dead
    if (player->IsInCombat() || !player->IsAlive())
    {
        ChatHandler(player->GetSession()).SendSysMessage("You cannot summon the Arena Master while in combat or dead.");
        return;
    }

    // Check if in arena or battleground
    if (player->InBattleground() || player->InArena())
    {
        ChatHandler(player->GetSession()).SendSysMessage("You cannot summon the Arena Master inside a battleground or arena.");
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);

    // Duplicate-spawn prevention: if an active NPC is already tracked, despawn it first
    auto it = _summonedNpcs.find(player->GetGUID());
    if (it != _summonedNpcs.end())
    {
        Map* prevMap = sMapMgr->FindBaseMap(it->second.MapId);
        if (prevMap)
        {
            if (Creature* prevNpc = prevMap->GetCreature(it->second.CreatureGuid))
            {
                prevNpc->DespawnOrUnsummon();
            }
        }
        _summonedNpcs.erase(it);
    }

    // Calculate spawn position 2 yards directly in front of the player, facing them
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    player->GetClosePoint(x, y, z, player->GetObjectSize() + 2.0f);
    float orientation = Position::NormalizeOrientation(player->GetOrientation() + static_cast<float>(M_PI));

    uint32 durationMs = sLowLevelArenaConfig->SummonDuration * IN_MILLISECONDS;
    Creature* npc = player->SummonCreature(sLowLevelArenaConfig->NpcEntry, x, y, z, orientation,
        TEMPSUMMON_TIMED_DESPAWN, durationMs);

    if (!npc)
    {
        ChatHandler(player->GetSession()).SendSysMessage("Failed to summon the Low-Level Arena Master.");
        return;
    }

    SummonedNpcRecord record;
    record.CreatureGuid = npc->GetGUID();
    record.MapId = player->GetMapId();
    record.ExpireTimeMs = durationMs;

    _summonedNpcs[player->GetGUID()] = record;

    uint32 minutes = sLowLevelArenaConfig->SummonDuration / 60;
    if (minutes > 0)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Low-Level Arena Master summoned. He will remain for {} minutes.", minutes);
    }
    else
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Low-Level Arena Master summoned. He will remain for {} seconds.", sLowLevelArenaConfig->SummonDuration);
    }
}

void LowLevelArenaMgr::HandleDespawn(Player* player)
{
    if (!player)
        return;

    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _summonedNpcs.find(player->GetGUID());
    if (it == _summonedNpcs.end())
    {
        ChatHandler(player->GetSession()).SendSysMessage("You do not have an active summoned Arena Master.");
        return;
    }

    Map* map = player->GetMap();
    if (map)
    {
        if (Creature* npc = map->GetCreature(it->second.CreatureGuid))
        {
            npc->DespawnOrUnsummon();
        }
    }

    _summonedNpcs.erase(it);
    ChatHandler(player->GetSession()).SendSysMessage("Low-Level Arena Master dismissed.");
}

void LowLevelArenaMgr::HandlePlayerLogout(Player* player)
{
    if (!player)
        return;

    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _summonedNpcs.find(player->GetGUID());
    if (it == _summonedNpcs.end())
        return;

    Map* map = player->GetMap();
    if (map)
    {
        if (Creature* npc = map->GetCreature(it->second.CreatureGuid))
        {
            npc->DespawnOrUnsummon();
        }
    }

    _summonedNpcs.erase(it);
}

void LowLevelArenaMgr::HandlePlayerMapChanged(Player* player)
{
    if (!player)
        return;

    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _summonedNpcs.find(player->GetGUID());
    if (it == _summonedNpcs.end())
        return;

    if (it->second.MapId != player->GetMapId())
    {
        Map* prevMap = sMapMgr->FindBaseMap(it->second.MapId);
        if (prevMap)
        {
            if (Creature* npc = prevMap->GetCreature(it->second.CreatureGuid))
            {
                npc->DespawnOrUnsummon();
            }
        }
        _summonedNpcs.erase(it);
    }
}

void LowLevelArenaMgr::Update(uint32 diff)
{
    if (_cleanupTimer <= diff)
    {
        _cleanupTimer = 5000;

        std::lock_guard<std::mutex> lock(_mutex);
        for (auto it = _summonedNpcs.begin(); it != _summonedNpcs.end();)
        {
            if (it->second.ExpireTimeMs <= 5000)
            {
                it = _summonedNpcs.erase(it);
                continue;
            }

            it->second.ExpireTimeMs -= 5000;

            Map* map = sMapMgr->FindBaseMap(it->second.MapId);
            if (!map || !map->GetCreature(it->second.CreatureGuid))
            {
                it = _summonedNpcs.erase(it);
                continue;
            }

            ++it;
        }
    }
    else
    {
        _cleanupTimer -= diff;
    }
}

void LowLevelArenaMgr::CheckWorldSpawns()
{
    // If permanent world spawns are disabled, find and despawn them
    if (!sLowLevelArenaConfig->WorldSpawnEnable)
    {
        std::vector<uint32> mapIds = { 0, 1, 571 };
        for (uint32 mapId : mapIds)
        {
            Map* map = sMapMgr->FindBaseMap(mapId);
            if (!map)
                continue;

            // Permanent spawn GUIDs defined in SQL: 7001001, 7001002, 7001003
            std::vector<uint32> spawnGuids = { 7001001, 7001002, 7001003 };
            for (uint32 lowGuid : spawnGuids)
            {
                ObjectGuid guid = ObjectGuid::Create<HighGuid::Unit>(sLowLevelArenaConfig->NpcEntry, lowGuid);
                if (Creature* c = map->GetCreature(guid))
                {
                    c->DespawnOrUnsummon();
                }
            }
        }
    }
}

bool LowLevelArenaMgr::IsSummonedBy(Player const* player, Creature const* creature)
{
    if (!player || !creature)
        return false;

    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _summonedNpcs.find(player->GetGUID());
    if (it != _summonedNpcs.end())
    {
        return it->second.CreatureGuid == creature->GetGUID();
    }

    return false;
}

bool LowLevelArenaMgr::GetBracketForLevel(uint8 level, uint8& outMin, uint8& outMax, uint8& outBracketId) const
{
    for (size_t i = 0; i < sLowLevelArenaConfig->Brackets.size(); ++i)
    {
        auto const& b = sLowLevelArenaConfig->Brackets[i];
        if (level >= b.MinLevel && level <= b.MaxLevel)
        {
            outMin = b.MinLevel;
            outMax = b.MaxLevel;
            outBracketId = static_cast<uint8>(i);
            return true;
        }
    }

    return false;
}

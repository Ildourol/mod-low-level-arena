/*
 * Copyright (C) 2026+ AzerothCore Project
 * Released under GNU AGPL v3 license.
 */

#include "LowLevelArenaConfig.h"
#include "LowLevelArenaMgr.h"
#include "BattlegroundMgr.h"
#include "Chat.h"
#include "Creature.h"
#include "GossipDef.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "Opcodes.h"
#include "ServerScript.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "WorldSessionMgr.h"

enum LowLevelArenaGossipSender : uint32
{
    SENDER_LLA_MAIN = 1000,
    SENDER_LLA_INFO = 1001
};

enum LowLevelArenaGossipAction : uint32
{
    ACTION_LLA_QUEUE   = 1,
    ACTION_LLA_INFO    = 2,
    ACTION_LLA_DISMISS = 3,
    ACTION_LLA_CLOSE   = 4,
    ACTION_LLA_BACK    = 5
};

class LowLevelArenaWorldScript : public WorldScript
{
public:
    LowLevelArenaWorldScript() : WorldScript("LowLevelArenaWorldScript") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sLowLevelArenaConfig->LoadConfig();
        sLowLevelArenaMgr->CheckWorldSpawns();
    }

    void OnUpdate(uint32 diff) override
    {
        sLowLevelArenaMgr->Update(diff);
    }
};

class LowLevelArenaPlayerScript : public PlayerScript
{
public:
    LowLevelArenaPlayerScript() : PlayerScript("LowLevelArenaPlayerScript") { }

    void OnPlayerLogin(Player* player) override
    {
        if (!sLowLevelArenaConfig->Enable || !player)
            return;

        if (sLowLevelArenaConfig->SummonSpellEnable && !player->HasSpell(sLowLevelArenaConfig->SummonSpellId))
        {
            sLowLevelArenaMgr->TeachSpell(player, sLowLevelArenaConfig->SummonSpellId);
        }

        if (sLowLevelArenaConfig->DespawnSpellEnable && !player->HasSpell(sLowLevelArenaConfig->DespawnSpellId))
        {
            sLowLevelArenaMgr->TeachSpell(player, sLowLevelArenaConfig->DespawnSpellId);
        }
    }

    void OnPlayerLearnSpell(Player* player, uint32 spellId) override
    {
        if (!sLowLevelArenaConfig->Enable || !player)
            return;

        if (sLowLevelArenaMgr->IsArenaSpell(spellId))
        {
            sLowLevelArenaMgr->MarkAutoLearningSpell(player->GetGUID(), spellId);
        }
    }

    void OnPlayerLogout(Player* player) override
    {
        sLowLevelArenaMgr->HandlePlayerLogout(player);
    }

    void OnPlayerMapChanged(Player* player) override
    {
        sLowLevelArenaMgr->HandlePlayerMapChanged(player);
    }

    bool OnPlayerCanJoinInArenaQueue(Player* player, ObjectGuid /*battlemasterGuid*/, uint8 arenaslot,
        BattlegroundTypeId /*bgTypeId*/, uint8 /*joinAsGroup*/, uint8 isRated, GroupJoinBattlegroundResult& err) override
    {
        if (!sLowLevelArenaConfig->Enable || !player)
            return true;

        uint8 level = player->GetLevel();
        if (!sLowLevelArenaConfig->IsLowLevel(level))
            return true;

        if (!sLowLevelArenaConfig->IsSlotEnabled(arenaslot))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This arena bracket slot is currently disabled for low-level characters.");
            err = ERR_BATTLEGROUND_JOIN_FAILED;
            return false;
        }

        if (isRated && !sLowLevelArenaConfig->RatedEnable)
        {
            ChatHandler(player->GetSession()).SendSysMessage("Rated arena matches are not enabled for low-level brackets. Please queue for Skirmish.");
            err = ERR_BATTLEGROUND_CANNOT_QUEUE_FOR_RATED;
            return false;
        }

        if (!isRated && !sLowLevelArenaConfig->SkirmishEnable)
        {
            ChatHandler(player->GetSession()).SendSysMessage("Skirmish arena matches are currently disabled for low-level brackets.");
            err = ERR_BATTLEGROUND_JOIN_FAILED;
            return false;
        }

        return true;
    }

    void OnPlayerJoinArena(Player* player) override
    {
        if (!sLowLevelArenaConfig->Enable || !sLowLevelArenaConfig->AnnounceQueue || !player)
            return;

        uint8 level = player->GetLevel();
        if (!sLowLevelArenaConfig->IsLowLevel(level))
            return;

        uint8 minLvl = 0;
        uint8 maxLvl = 0;
        uint8 bracketId = 0;
        if (sLowLevelArenaMgr->GetBracketForLevel(level, minLvl, maxLvl, bracketId))
        {
            std::string msg = Acore::StringFormat("|cff00ccff[Arena]|r Player |cffffffff{}|r (Level {}) has joined the Low-Level Arena queue (Bracket {}-{})!",
                player->GetName(), level, minLvl, maxLvl);
            sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, msg);
        }
    }
};

class LowLevelArenaSpellScript : public AllSpellScript
{
public:
    LowLevelArenaSpellScript() : AllSpellScript("LowLevelArenaSpellScript") { }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* spellInfo, bool /*skipCheck*/) override
    {
        if (!sLowLevelArenaConfig->Enable || !caster || !spellInfo)
            return;

        Player* player = caster->ToPlayer();
        if (!player)
            return;

        uint32 spellId = spellInfo->Id;

        if (sLowLevelArenaConfig->SummonSpellEnable && spellId == sLowLevelArenaConfig->SummonSpellId)
        {
            sLowLevelArenaMgr->HandleSummon(player);
            player->RemoveAura(spellId);
        }
        else if (sLowLevelArenaConfig->DespawnSpellEnable && spellId == sLowLevelArenaConfig->DespawnSpellId)
        {
            sLowLevelArenaMgr->HandleDespawn(player);
            player->RemoveAura(spellId);
        }
    }
};

class npc_low_level_arena_master : public CreatureScript
{
public:
    npc_low_level_arena_master() : CreatureScript("npc_low_level_arena_master") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!sLowLevelArenaConfig->Enable || !player || !creature)
            return false;

        ClearGossipMenuFor(player);

        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "I would like to enter the Arena.",
            SENDER_LLA_MAIN, ACTION_LLA_QUEUE);

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "What are the Low-Level Arena brackets?",
            SENDER_LLA_MAIN, ACTION_LLA_INFO);

        if (sLowLevelArenaMgr->IsSummonedBy(player, creature))
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Dismiss Arena Master.",
                SENDER_LLA_MAIN, ACTION_LLA_DISMISS);
        }

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Goodbye.",
            SENDER_LLA_MAIN, ACTION_LLA_CLOSE);

        SendGossipMenuFor(player, 1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        if (!player || !creature)
            return false;

        ClearGossipMenuFor(player);

        if (sender == SENDER_LLA_MAIN)
        {
            switch (action)
            {
                case ACTION_LLA_QUEUE:
                    CloseGossipMenuFor(player);
                    player->GetSession()->SendBattleGroundList(creature->GetGUID(), BATTLEGROUND_AA);
                    return true;

                case ACTION_LLA_INFO:
                {
                    uint8 playerLvl = player->GetLevel();
                    uint8 pMin = 0;
                    uint8 pMax = 0;
                    uint8 pBracket = 0;
                    bool hasBracket = sLowLevelArenaMgr->GetBracketForLevel(playerLvl, pMin, pMax, pBracket);

                    std::string infoText = "Active Low-Level Arena Brackets:\n";
                    for (size_t i = 0; i < sLowLevelArenaConfig->Brackets.size(); ++i)
                    {
                        auto const& b = sLowLevelArenaConfig->Brackets[i];
                        infoText += Acore::StringFormat(" - Bracket {}: Levels {}-{}\n", i, b.MinLevel, b.MaxLevel);
                    }

                    if (hasBracket)
                    {
                        infoText += Acore::StringFormat("\nYour level ({}) places you in Bracket {} ({}-{}).",
                            playerLvl, pBracket, pMin, pMax);
                    }

                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Queue for Arena.",
                        SENDER_LLA_MAIN, ACTION_LLA_QUEUE);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back to Main Menu.",
                        SENDER_LLA_MAIN, ACTION_LLA_BACK);

                    ChatHandler(player->GetSession()).SendSysMessage(infoText.c_str());
                    SendGossipMenuFor(player, 1, creature->GetGUID());
                    return true;
                }

                case ACTION_LLA_DISMISS:
                    CloseGossipMenuFor(player);
                    sLowLevelArenaMgr->HandleDespawn(player);
                    return true;

                case ACTION_LLA_CLOSE:
                    CloseGossipMenuFor(player);
                    return true;

                case ACTION_LLA_BACK:
                    return OnGossipHello(player, creature);

                default:
                    CloseGossipMenuFor(player);
                    return true;
            }
        }

        return true;
    }
};

class LowLevelArenaServerScript : public ServerScript
{
public:
    LowLevelArenaServerScript() : ServerScript("LowLevelArenaServerScript") { }

    bool CanPacketReceive(WorldSession* session, WorldPacket const& packet) override
    {
        if (!sLowLevelArenaConfig->Enable || !session)
            return true;

        if (packet.GetOpcode() == CMSG_SET_ACTION_BUTTON)
        {
            WorldPacket copy = packet;
            uint8 button;
            uint32 packetData;
            copy >> button >> packetData;

            uint32 action = ACTION_BUTTON_ACTION(packetData);
            uint8 type = ACTION_BUTTON_TYPE(packetData);

            if (type == ACTION_BUTTON_SPELL && sLowLevelArenaMgr->IsArenaSpell(action))
            {
                if (Player* player = session->GetPlayer())
                {
                    if (sLowLevelArenaMgr->IsAutoLearningSpell(player->GetGUID(), action))
                    {
                        sLowLevelArenaMgr->ClearAutoLearningSpell(player->GetGUID(), action);
                        player->SendActionButtons(1);
                        return false;
                    }
                }
            }
        }

        return true;
    }
};

void AddLowLevelArenaScripts()
{
    new LowLevelArenaWorldScript();
    new LowLevelArenaPlayerScript();
    new LowLevelArenaSpellScript();
    new LowLevelArenaServerScript();
    new npc_low_level_arena_master();
}

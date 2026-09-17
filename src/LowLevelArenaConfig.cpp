/*
 * Copyright (C) 2026+ AzerothCore Project
 * Released under GNU AGPL v3 license.
 */

#include "LowLevelArenaConfig.h"
#include "Config.h"
#include "Log.h"
#include "StringConvert.h"
#include <sstream>

LowLevelArenaConfig* LowLevelArenaConfig::instance()
{
    static LowLevelArenaConfig instance;
    return &instance;
}

void LowLevelArenaConfig::LoadConfig()
{
    Enable = sConfigMgr->GetOption<bool>("LowLevelArena.Enable", true);
    BracketsStr = sConfigMgr->GetOption<std::string>("LowLevelArena.Brackets", "10-19,20-29,30-39,40-49,50-59,60-69,70-79,80-80");

    Enable2v2 = sConfigMgr->GetOption<bool>("LowLevelArena.Enable2v2", true);
    Enable3v3 = sConfigMgr->GetOption<bool>("LowLevelArena.Enable3v3", true);
    Enable5v5 = sConfigMgr->GetOption<bool>("LowLevelArena.Enable5v5", true);

    SkirmishEnable = sConfigMgr->GetOption<bool>("LowLevelArena.Skirmish.Enable", true);
    RatedEnable = sConfigMgr->GetOption<bool>("LowLevelArena.Rated.Enable", false);

    AnnounceQueue = sConfigMgr->GetOption<bool>("LowLevelArena.AnnounceQueue", false);

    NpcEnable = sConfigMgr->GetOption<bool>("LowLevelArena.NPC.Enable", true);
    NpcEntry = sConfigMgr->GetOption<uint32>("LowLevelArena.NPC.Entry", 700100);
    WorldSpawnEnable = sConfigMgr->GetOption<bool>("LowLevelArena.NPC.WorldSpawn.Enable", true);

    SummonSpellEnable = sConfigMgr->GetOption<bool>("LowLevelArena.NPC.SummonSpell.Enable", true);
    SummonSpellId = sConfigMgr->GetOption<uint32>("LowLevelArena.NPC.SummonSpell.SpellId", 56189);

    DespawnSpellEnable = sConfigMgr->GetOption<bool>("LowLevelArena.NPC.DespawnSpell.Enable", true);
    DespawnSpellId = sConfigMgr->GetOption<uint32>("LowLevelArena.NPC.DespawnSpell.SpellId", 43014);

    SummonDuration = sConfigMgr->GetOption<uint32>("LowLevelArena.NPC.SummonDuration", 900);
    Debug = sConfigMgr->GetOption<bool>("LowLevelArena.Debug", false);

    ParseBrackets();

    LOG_INFO("server.loading", "[LowLevelArena] Configuration loaded: Enable={}, Brackets='{}', 2v2={}, 3v3={}, 5v5={}, Skirmish={}, Rated={}",
        Enable, BracketsStr, Enable2v2, Enable3v3, Enable5v5, SkirmishEnable, RatedEnable);
}

void LowLevelArenaConfig::ParseBrackets()
{
    Brackets.clear();
    std::stringstream ss(BracketsStr);
    std::string token;

    while (std::getline(ss, token, ','))
    {
        auto dashPos = token.find('-');
        if (dashPos != std::string::npos)
        {
            std::string minStr = token.substr(0, dashPos);
            std::string maxStr = token.substr(dashPos + 1);

            auto minLvl = Acore::StringTo<uint32>(minStr);
            auto maxLvl = Acore::StringTo<uint32>(maxStr);

            if (minLvl && maxLvl && *minLvl <= *maxLvl && *maxLvl <= 80)
            {
                Brackets.push_back({ static_cast<uint8>(*minLvl), static_cast<uint8>(*maxLvl) });
            }
        }
    }

    if (Brackets.empty())
    {
        Brackets = {
            { 10, 19 },
            { 20, 29 },
            { 30, 39 },
            { 40, 49 },
            { 50, 59 },
            { 60, 69 },
            { 70, 79 },
            { 80, 80 }
        };
    }
}

bool LowLevelArenaConfig::IsLowLevel(uint8 level) const
{
    return level < 80;
}

bool LowLevelArenaConfig::IsSlotEnabled(uint8 arenaslot) const
{
    switch (arenaslot)
    {
        case 0:
            return Enable2v2;
        case 1:
            return Enable3v3;
        case 2:
            return Enable5v5;
        default:
            return false;
    }
}

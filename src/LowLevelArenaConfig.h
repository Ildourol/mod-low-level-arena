/*
 * Copyright (C) 2026+ AzerothCore Project
 * Released under GNU AGPL v3 license.
 */

#ifndef LOW_LEVEL_ARENA_CONFIG_H
#define LOW_LEVEL_ARENA_CONFIG_H

#include "Common.h"
#include <string>
#include <vector>

struct ArenaBracketConfig
{
    uint8 MinLevel;
    uint8 MaxLevel;
};

class LowLevelArenaConfig
{
public:
    static LowLevelArenaConfig* instance();

    void LoadConfig();

    bool Enable{true};
    std::string BracketsStr{"10-19,20-29,30-39,40-49,50-59,60-69,70-79,80-80"};
    std::vector<ArenaBracketConfig> Brackets;

    bool Enable2v2{true};
    bool Enable3v3{true};
    bool Enable5v5{true};

    bool SkirmishEnable{true};
    bool RatedEnable{false};

    bool AnnounceQueue{false};

    bool NpcEnable{true};
    uint32 NpcEntry{700100};
    bool WorldSpawnEnable{true};

    bool SummonSpellEnable{true};
    uint32 SummonSpellId{56189};

    bool DespawnSpellEnable{true};
    uint32 DespawnSpellId{43014};

    uint32 SummonDuration{900};
    bool Debug{false};

    [[nodiscard]] bool IsLowLevel(uint8 level) const;
    [[nodiscard]] bool IsSlotEnabled(uint8 arenaslot) const;

private:
    LowLevelArenaConfig() = default;
    void ParseBrackets();
};

#define sLowLevelArenaConfig LowLevelArenaConfig::instance()

#endif // LOW_LEVEL_ARENA_CONFIG_H

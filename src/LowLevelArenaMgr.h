/*
 * Copyright (C) 2026+ AzerothCore Project
 * Released under GNU AGPL v3 license.
 */

#ifndef LOW_LEVEL_ARENA_MGR_H
#define LOW_LEVEL_ARENA_MGR_H

#include "Common.h"
#include "ObjectGuid.h"
#include <mutex>
#include <unordered_map>
#include <unordered_set>

class Creature;
class Player;

struct SummonedNpcRecord
{
    ObjectGuid CreatureGuid;
    uint32 MapId{0};
    uint32 ExpireTimeMs{0};
};

class LowLevelArenaMgr
{
public:
    static LowLevelArenaMgr* instance();

    void HandleSummon(Player* player);
    void HandleDespawn(Player* player);
    void HandlePlayerLogout(Player* player);
    void HandlePlayerMapChanged(Player* player);

    void Update(uint32 diff);
    void CheckWorldSpawns();

    [[nodiscard]] bool IsSummonedBy(Player const* player, Creature const* creature);
    bool GetBracketForLevel(uint8 level, uint8& outMin, uint8& outMax, uint8& outBracketId) const;

    [[nodiscard]] bool IsArenaSpell(uint32 spellId) const;
    [[nodiscard]] bool IsAutoLearningSpell(ObjectGuid const& playerGuid, uint32 spellId) const;
    void MarkAutoLearningSpell(ObjectGuid const& playerGuid, uint32 spellId);
    void ClearAutoLearningSpell(ObjectGuid const& playerGuid, uint32 spellId);
    void TeachSpell(Player* player, uint32 spellId);

private:
    LowLevelArenaMgr() = default;

    std::mutex _mutex;
    std::unordered_map<ObjectGuid, SummonedNpcRecord> _summonedNpcs;
    std::unordered_map<ObjectGuid, std::unordered_set<uint32>> _autoLearningSpells;
    uint32 _cleanupTimer{5000};
};

#define sLowLevelArenaMgr LowLevelArenaMgr::instance()

#endif // LOW_LEVEL_ARENA_MGR_H

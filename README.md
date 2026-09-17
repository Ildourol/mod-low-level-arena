# AzerothCore mod-low-level-arena

![Low-Level Arena](assets/banner.png)

A standalone AzerothCore module providing native low-level arena brackets, native queue matchmaking integration, natural Playerbots participation, and a summonable Low-Level Arena Master NPC.

## Description

In standard Wrath of the Lich King emulators, arena matches are predominantly tuned for endgame level 80 combat, leaving low-level and leveling characters excluded from competitive small-scale PvP outside of battlegrounds.

The **mod-low-level-arena** module bridges this gap by introducing structured, authoritative low-level arena brackets starting from level 10. Characters across all leveling ranges can step into the arena pits, test their class mechanics, and experience gladiatorial combat.

Through native data override architecture, the module populates genuine bracket partitions across all standard arena maps. Existing Playerbots automatically detect these active queue brackets and join matches naturally alongside real players without artificial bots scripting or parallel matchmaking logic.

In addition to permanent arena master spawns in major faction capitals, players receive General spellbook abilities to summon a temporary Arena Master anywhere in the open world, complete with duplicate protection and automatic cleanup.

## Features

- **Native Arena Brackets**: Authoritative bracket ranges (10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79, and 80-80).
- **Native Queue Integration**: Operates through standard AzerothCore 2v2, 3v3, and 5v5 arena matchmaking queues.
- **Natural Playerbot Compatibility**: Playerbots scan, discover, and join the exact same level brackets automatically.
- **Dedicated Low-Level Arena Master NPC**: Custom battlemaster NPC (Entry 700100) with full gossip and direct arena queue access.
- **Permanent World Spawns**: Optional permanent spawns in Stormwind, Orgrimmar, and Dalaran Underbelly.
- **On-Demand Summon and Despawn Spells**: Spells in the General spellbook allow players to summon the Arena Master in the field and dismiss him on command.
- **Automated Lifecycle Management**: Configurable temporary lifetime (default 15 minutes), duplicate-spawn prevention, logout despawn, and map transition cleanup.
- **Zero Core or Playerbot Modifications**: Completely standalone module utilizing supported database override mechanisms and standard ScriptMgr hooks.

## Configuration

Settings are fully configurable in `mod_low_level_arena.conf`:

```ini
[worldserver]
LowLevelArena.Enable = 1
LowLevelArena.Brackets = "10-19,20-29,30-39,40-49,50-59,60-69,70-79,80-80"
LowLevelArena.Enable2v2 = 1
LowLevelArena.Enable3v3 = 1
LowLevelArena.Enable5v5 = 1
LowLevelArena.Skirmish.Enable = 1
LowLevelArena.Rated.Enable = 0
LowLevelArena.AnnounceQueue = 0
LowLevelArena.NPC.Enable = 1
LowLevelArena.NPC.WorldSpawn.Enable = 1
LowLevelArena.NPC.SummonSpell.Enable = 1
LowLevelArena.NPC.DespawnSpell.Enable = 1
LowLevelArena.NPC.SummonDuration = 900
LowLevelArena.Debug = 0
```

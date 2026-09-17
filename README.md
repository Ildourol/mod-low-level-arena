# AzerothCore mod-low-level-arena

A standalone AzerothCore module providing native low-level arena brackets, native queue integration, Playerbots support, and a summonable Low-Level Arena Master NPC.

## Features

- **Native Low-Level Arena Brackets**: Authoritative level ranges (10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79, 80-80).
- **Native AzerothCore Arena Queue**: Seamless integration with 2v2, 3v3, and 5v5 skirmish and rated queues.
- **Playerbots Natural Queuing**: Existing playerbots naturally discover and join the exact same low-level brackets.
- **Dedicated Low-Level Arena Master NPC**: Entry 700100 with gossip and direct battlemaster interface.
- **Permanent World Spawns**: Optional permanent spawns in Stormwind, Orgrimmar, and Dalaran Underbelly.
- **Player Summon & Despawn Spells**: Spells in the General spellbook allow players to summon the Arena Master anywhere in the world and dismiss him on command.
- **Automatic Lifecycle Management**: Default 15-minute lifetime with automatic cleanup, logout cleanup, map change cleanup, and duplicate-spawn prevention.
- **Zero Core & Playerbot Changes**: Operates strictly within the module boundary using AzerothCore's supported `pvpdifficulty_dbc` SQL override mechanism and standard `ScriptMgr` hooks.

## Configuration

Settings can be customized in `mod_low_level_arena.conf`:

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

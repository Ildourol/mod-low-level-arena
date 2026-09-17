-- ============================================================================
-- mod-low-level-arena: World Database Setup
-- Native low-level arena brackets, battlemaster entry, creature template & spawns
-- ============================================================================

-- ----------------------------------------------------------------------------
-- 1. Native Arena PvP Difficulty Brackets (10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79, 80-80)
-- Overrides PvpDifficulty.dbc rows via pvpdifficulty_dbc table
-- ----------------------------------------------------------------------------
DELETE FROM `pvpdifficulty_dbc` WHERE `ID` BETWEEN 20 AND 35;
INSERT INTO `pvpdifficulty_dbc` (`ID`, `MapID`, `RangeIndex`, `MinLevel`, `MaxLevel`, `Difficulty`) VALUES
-- Map 559: Nagrand Arena (Active brackets 0-7)
(20, 559, 0, 10, 19, 0),
(21, 559, 1, 20, 29, 0),
(22, 559, 2, 30, 39, 0),
(23, 559, 3, 40, 49, 0),
(24, 559, 4, 50, 59, 0),
(25, 559, 5, 60, 69, 0),
(26, 559, 6, 70, 79, 0),
(27, 559, 7, 80, 80, 0),
-- Map 559: Zero out unused DBC range slots 8-15
(28, 0, 0, 0, 0, 0),
(29, 0, 0, 0, 0, 0),
(30, 0, 0, 0, 0, 0),
(31, 0, 0, 0, 0, 0),
(32, 0, 0, 0, 0, 0),
(33, 0, 0, 0, 0, 0),
(34, 0, 0, 0, 0, 0),
(35, 0, 0, 0, 0, 0);

DELETE FROM `pvpdifficulty_dbc` WHERE `ID` BETWEEN 36 AND 51;
INSERT INTO `pvpdifficulty_dbc` (`ID`, `MapID`, `RangeIndex`, `MinLevel`, `MaxLevel`, `Difficulty`) VALUES
-- Map 562: Blade's Edge Arena (Active brackets 0-7)
(36, 562, 0, 10, 19, 0),
(37, 562, 1, 20, 29, 0),
(38, 562, 2, 30, 39, 0),
(39, 562, 3, 40, 49, 0),
(40, 562, 4, 50, 59, 0),
(41, 562, 5, 60, 69, 0),
(42, 562, 6, 70, 79, 0),
(43, 562, 7, 80, 80, 0),
-- Map 562: Zero out unused DBC range slots 8-15
(44, 0, 0, 0, 0, 0),
(45, 0, 0, 0, 0, 0),
(46, 0, 0, 0, 0, 0),
(47, 0, 0, 0, 0, 0),
(48, 0, 0, 0, 0, 0),
(49, 0, 0, 0, 0, 0),
(50, 0, 0, 0, 0, 0),
(51, 0, 0, 0, 0, 0);

DELETE FROM `pvpdifficulty_dbc` WHERE `ID` BETWEEN 55 AND 70;
INSERT INTO `pvpdifficulty_dbc` (`ID`, `MapID`, `RangeIndex`, `MinLevel`, `MaxLevel`, `Difficulty`) VALUES
-- Map 572: Ruins of Lordaeron (Active brackets 0-7)
(55, 572, 0, 10, 19, 0),
(56, 572, 1, 20, 29, 0),
(57, 572, 2, 30, 39, 0),
(58, 572, 3, 40, 49, 0),
(59, 572, 4, 50, 59, 0),
(60, 572, 5, 60, 69, 0),
(61, 572, 6, 70, 79, 0),
(62, 572, 7, 80, 80, 0),
-- Map 572: Zero out unused DBC range slots 8-15
(63, 0, 0, 0, 0, 0),
(64, 0, 0, 0, 0, 0),
(65, 0, 0, 0, 0, 0),
(66, 0, 0, 0, 0, 0),
(67, 0, 0, 0, 0, 0),
(68, 0, 0, 0, 0, 0),
(69, 0, 0, 0, 0, 0),
(70, 0, 0, 0, 0, 0);

DELETE FROM `pvpdifficulty_dbc` WHERE `ID` BETWEEN 74 AND 89;
INSERT INTO `pvpdifficulty_dbc` (`ID`, `MapID`, `RangeIndex`, `MinLevel`, `MaxLevel`, `Difficulty`) VALUES
-- Map 617: Dalaran Sewers (Active brackets 0-7)
(74, 617, 0, 10, 19, 0),
(75, 617, 1, 20, 29, 0),
(76, 617, 2, 30, 39, 0),
(77, 617, 3, 40, 49, 0),
(78, 617, 4, 50, 59, 0),
(79, 617, 5, 60, 69, 0),
(80, 617, 6, 70, 79, 0),
(81, 617, 7, 80, 80, 0),
-- Map 617: Zero out unused DBC range slots 8-15
(82, 0, 0, 0, 0, 0),
(83, 0, 0, 0, 0, 0),
(84, 0, 0, 0, 0, 0),
(85, 0, 0, 0, 0, 0),
(86, 0, 0, 0, 0, 0),
(87, 0, 0, 0, 0, 0),
(88, 0, 0, 0, 0, 0),
(89, 0, 0, 0, 0, 0);

DELETE FROM `pvpdifficulty_dbc` WHERE `ID` BETWEEN 90 AND 105;
INSERT INTO `pvpdifficulty_dbc` (`ID`, `MapID`, `RangeIndex`, `MinLevel`, `MaxLevel`, `Difficulty`) VALUES
-- Map 618: The Ring of Valor (Active brackets 0-7)
(90, 618, 0, 10, 19, 0),
(91, 618, 1, 20, 29, 0),
(92, 618, 2, 30, 39, 0),
(93, 618, 3, 40, 49, 0),
(94, 618, 4, 50, 59, 0),
(95, 618, 5, 60, 69, 0),
(96, 618, 6, 70, 79, 0),
(97, 618, 7, 80, 80, 0),
-- Map 618: Zero out unused DBC range slots 8-15
(98, 0, 0, 0, 0, 0),
(99, 0, 0, 0, 0, 0),
(100, 0, 0, 0, 0, 0),
(101, 0, 0, 0, 0, 0),
(102, 0, 0, 0, 0, 0),
(103, 0, 0, 0, 0, 0),
(104, 0, 0, 0, 0, 0),
(105, 0, 0, 0, 0, 0);

-- ----------------------------------------------------------------------------
-- 2. Dedicated Low-Level Arena Master NPC Template
-- Entry: 700100, npcflag: 1048577 (GOSSIP | BATTLEMASTER), faction: 35 (Friendly)
-- ----------------------------------------------------------------------------
DELETE FROM `creature_template` WHERE `entry` = 700100;
INSERT INTO `creature_template` (
    `entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`,
    `KillCredit1`, `KillCredit2`,
    `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`,
    `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`,
    `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`,
    `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`,
    `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`,
    `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`,
    `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`,
    `flags_extra`, `ScriptName`, `VerifiedBuild`
) VALUES (
    700100, 0, 0, 0,
    0, 0,
    'Low-Level Arena Master', 'Arena Battlemaster', 'Directions', 0, 80, 80, 0,
    35, 1048577, 1.0, 1.14286, 1.0, 1.0, 1.0,
    0, 0, 1.0, 2000, 2000, 1.0, 1.0,
    1, 0, 0, 0, 0, 7, 0,
    0, 0, 0, 0, 0, 0, 0,
    '', 0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0, 0, 1, 0,
    0, 'npc_low_level_arena_master', 12340
);

DELETE FROM `creature_template_model` WHERE `CreatureID` = 700100;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(700100, 0, 16905, 1.0, 1.0, 12340);

-- ----------------------------------------------------------------------------
-- 3. Battlemaster Entry Binding for Entry 700100 -> BATTLEGROUND_AA (6)
-- ----------------------------------------------------------------------------
DELETE FROM `battlemaster_entry` WHERE `entry` = 700100;
INSERT INTO `battlemaster_entry` (`entry`, `bg_template`) VALUES (700100, 6);

-- ----------------------------------------------------------------------------
-- 4. Optional Permanent World Spawns (Stormwind, Orgrimmar, Dalaran Underbelly)
-- ----------------------------------------------------------------------------
DELETE FROM `creature` WHERE `id` = 700100;
INSERT INTO `creature` (
    `guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`,
    `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`,
    `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`,
    `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`
) VALUES
-- Stormwind (near Champions' Hall / Old Town PvP area)
(7001001, 700100, 0, 1519, 1519, 1, 1, 0, -8772.3, 377.2, 100.2, 4.7, 300, 0, 0, 1000, 0, 0, 0, 0, 0, '', 12340, 0, 'Low-Level Arena Master SW'),
-- Orgrimmar (Valley of Honor / Hall of Legends)
(7001002, 700100, 1, 1637, 1637, 1, 1, 0, 1968.2, -4344.8, 24.3, 0.5, 300, 0, 0, 1000, 0, 0, 0, 0, 0, '', 12340, 0, 'Low-Level Arena Master ORG'),
-- Dalaran (Underbelly / Sewers)
(7001003, 700100, 571, 4395, 4395, 1, 1, 0, 5815.1, 588.3, 655.8, 1.5, 300, 0, 0, 1000, 0, 0, 0, 0, 0, '', 12340, 0, 'Low-Level Arena Master DAL');

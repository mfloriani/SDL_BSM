--------
--Assets
spritesheet		= "Assets\\spriteSheet16x16.png"
player1_sprite	= "Assets\\player48x35.png"
enemy_sprite	= "Assets\\enemy53x35.png"
map_name		= "Assets\\map_01.map"
font_name		= "Assets\\arial.ttf"
music_theme     = "Assets\\Suspense-Militar-GGJ18.mp3"
sound_shot      = "Assets\\391725__morganpurkis__battle-rifle.wav"

----------
--Missions
mission_success_text = "MISSION ACCOMPLISHED"
mission_failure_text = "MISSION FAILURE"

--------
--Tiled
tiled_firstmap	= "Scripting\\tiled-importer.lua"

-----------
--Messaging

--1/4 from a sec
smallest_delay	= 0.25

--GameObjects
default_mass		= 1.0
default_maxforce	= 2.0
default_maxvelocity	= 150.0

--Player
player_rateoffire	= 0.5
plyer_inputforce	= 600.0

--Enemy
enemy_rateoffire	= 0.5
enemy_maxforce		= 400.0
enemy_maxvelocity	= 50.0
enemy_fov			= 135.0
enemy_attackdist	= 300000.0

--Bullet
bullet_velocity		= 1000.0
bullet_width		= 4
bullet_height		= 4
bullet_damage		= 100


--Steering Behaviors


--Pathfinding
path_waypointdist	= 1850
edge_defaultcost	= 1.0

--Volume
volume_music		= 50
volume_sfx			= 20
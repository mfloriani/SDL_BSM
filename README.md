# SDL_BSM

Game prototype developed as part of a gamedev postgraduation course in 2018.

# Tech

* C++
* SDL2-2.0.7 - https://www.libsdl.org/
* Lua - https://www.lua.org/
* Tiled - https://www.mapeditor.org/

# Gameplay video

[![Gameplay Video](Assets/sample-gameplay.png)](https://youtu.be/TY-zbuZDEy4-Y "Gameplay")

# Level Design

To design the levels I used a tool named Tiled. Using some components provided by the tool I defined the following objects:

* Walls
* Player initial position
* Enemies initial position
* Enemies patrol route

When the level was ready I exported .lua file that is loaded and parsed by the game to draw and set all game objects for the level.

Level desing in Tiled with the components mentioned above
[![Level Design](Assets/level-design.png)]

Level loaded in the game (debug mode)
[![Debug Mode](Assets/debug-mode.png)]

In blue the walls used by the line of sight (LOS) system.
In green the colliders.
In purple the enemies patrol routes.
In red the direction and field of view (FOV).

# Pathfinding

To move the enemies through the level I used the A* algorithm to navigate on a navgraph created whenever the level is loaded always considering the walls there defined.

In light blue the navgraph compose by the nodes and edges:
[![Navgraph](Assets/navgraph.png)]

# Finite State Machine and Steering Behaviors

I implemented a FSM for the enemies actions based on the following list:

* Global (special state that is always executed)
* Idle
* Patrol
* Chase
* Attack
* Dying
* Dead

To move the enemies based on the action defined by the FSM I implemented some steering behaviors:

* seek
* arrive
* follow path
* wall avoidance (not used yet)

# Messaging

All the gameobjects comunicate among them using a messaging system that I implemented.

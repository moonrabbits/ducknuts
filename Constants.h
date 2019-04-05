#pragma once
#include <iostream>
#include <chrono>
using namespace std::literals::chrono_literals;
// TYPEDEFS
typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timer;
// SYSTEM
const float SCREEN_WIDTH = 500.0f;
const float SCREEN_HEIGHT = 720.0f;
const float THICCNESS = 15.0f;
const float UP = 1;
const float DOWN = -1;
const float LEFT = -1;
const float RIGHT = 1;
const float NONE = 0;
const float PI = 3.1415926535;
// COOLDOWN/TIMING

// HEALTH
const float MAX_PLAYER_HEALTH = 2000.0f;
const float PAWN_HEALTH = 600.0f;
const float GUARD_HEALTH = 1000.0f;
const float PRAETOR_HEALTH = 2000.0f;
const float KING_HEALTH = 4000.0f;
// DAMAGE
const float LOW_DAMAGE = 50.0f;
const float NORMAL_DAMAGE = 100.0f;
const float HIGH_DAMAGE = 200.0f;
const float COLL_DAMAGE = 20.0f;
// SPEED
const float PLAYER_SPEED = 300.0f;
const float PROJECTILE_SPEED = 500.0f;
const float PAWN_SPEED = 175.0f;
const float GUARD_SPEED = 75.0f;
const float PRAETOR_SPEED = 400.0f;
const float KING_SPEED = 300.0f;
// STRUCTS
struct Vector2 {
	float x;
	float y;
};

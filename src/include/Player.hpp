#pragma once

#include "Settings.hpp"
#include <raylib.h>
#include <raymath.h>
class Player {
private:
  bool isJumping;
  Vector2 force;
public:
  Vector2 pos;
  Player();
  void update(unsigned char map[][MAP_HEIGHT], float deltaTime);
  void render();
  void inputHandler(float deltaTime);
  Vector2 getPos();
};

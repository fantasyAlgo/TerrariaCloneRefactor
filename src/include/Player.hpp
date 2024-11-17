#pragma once

#include <raylib.h>
#include <raymath.h>
class Player {
public:
  Vector2 pos;
  Player();
  void update();
  void render();
  void inputHandler(float deltaTime);
};

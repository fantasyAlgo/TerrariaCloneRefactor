#pragma once

#include "Blocks.h"
#include "LightHandler.hpp"
#include "Settings.hpp"
#include <raylib.h>
#include <raymath.h>
#include <utility>
class Zombie {
private:
  float isJumping = -1;
  bool whichDir;
  float animationFrame;
  bool needsJump;
  Vector2 force;
  Vector2 *target;
  float life;

public:
  Vector2 pos;
  Zombie();
  void init(Vector2 *new_target);
  void update(unsigned char map[][settings::MAP_HEIGHT], float deltaTime);
  void moveTowardsTarget(float deltaTime);
  void render(LightHandler &lightHandler);

  Vector2 getPos();
  void setTarget(Vector2 *new_target);
  Vector2 getTarget();
};

#pragma once

#include "Blocks.h"
#include "Settings.hpp"
#include <raylib.h>
#include <raymath.h>
class Player {
private:
  float isJumping = -1;
  bool whichDir;
  float animationFrame;
  Vector2 force;
  PlayerItem inventory[N_INVENTORY_ROWS][N_INVENTORY_COLUMNS+1] = {{
    {STONE, 1, EMPTY_TOOL, 10},
    {DIRT, 1, EMPTY_TOOL, 10},
    {COPPER_ORE, 1, EMPTY_TOOL, 10},
    {EMPTY, 1, EMPTY_TOOL, 0},
    {EMPTY, 1, EMPTY_TOOL, 0},
    {EMPTY, 1, EMPTY_TOOL, 0},
    {EMPTY, 1, EMPTY_TOOL, 0},
    {EMPTY, 1, EMPTY_TOOL, 0},
    {EMPTY, 1, EMPTY_TOOL, 0},
  }};

public:
  Vector2 pos;
  int selected_item;
  Player();
  void update(unsigned char map[][MAP_HEIGHT], float deltaTime);
  void render();
  void inputHandler(float deltaTime);

  Vector2 getPos();
  PlayerItem getInventoryItem(int i, int j);
};

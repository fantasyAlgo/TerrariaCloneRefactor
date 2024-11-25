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
  bool isInventoryOpen;
  PlayerItem inventory[settings::N_INVENTORY_ROWS][settings::N_INVENTORY_COLUMNS+1];

public:
  Vector2 pos;
  int selected_item;
  Player();
  void update(unsigned char map[][settings::MAP_HEIGHT], float deltaTime);
  void render();
  void inputHandler(float deltaTime);

  Vector2 getPos();
  bool getShowInventory();
  PlayerItem getInventoryItem(int i, int j);
};

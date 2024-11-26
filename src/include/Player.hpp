#pragma once

#include "Blocks.h"
#include "Settings.hpp"
#include <raylib.h>
#include <raymath.h>
#include <utility>
class Player {
private:
  float isJumping = -1;
  float toolAnimation;
  bool whichDir;
  float animationFrame;
  Vector2 force;
  bool isInventoryOpen;
  PlayerItem inventory[settings::N_INVENTORY_ROWS][settings::N_INVENTORY_COLUMNS+1];

public:
  Vector2 pos;
  int selected_item;
  int slotIndices[2];
  Player();
  void update(unsigned char map[][settings::MAP_HEIGHT], float deltaTime);
  void render(Color player_light);
  void renderTool(Color player_light);

  void inputHandler(float deltaTime);

  // tool movement
  void initAction();

  void addBlockToInventory(BlockType id);
  void reduceSelectedBlock();

  void swapShowInventory(Vector2 ps1, Vector2 ps2);
  Vector2 getPos();
  bool getShowInventory();
  PlayerItem getInventoryItem(int i, int j);
};

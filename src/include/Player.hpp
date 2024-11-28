#pragma once

#include "Blocks.h"
#include "Settings.hpp"
#include "Zombie.hpp"
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
  Vector2 externalForce;

  bool isInventoryOpen;
  PlayerItem inventory[settings::N_INVENTORY_ROWS][settings::N_INVENTORY_COLUMNS+1];
  float life;
  float resitanceHit; 
public:
  Vector2 pos;
  int selected_item;
  int slotIndices[2];
  bool hasHit = false;
  Player();
  void update(unsigned char map[][settings::MAP_HEIGHT], float deltaTime);
  void render(Color player_light);
  void renderTool(Color player_light);

  void inputHandler(float deltaTime);

  // tool movement
  void initAction();
  // zombie stuff
  bool isColliding(Zombie zombie);
  bool isCollidingSwing(Zombie zombie);
  void hit(Zombie zombie);

  void addBlockToInventory(BlockType id);
  void reduceSelectedBlock();

  void swapShowInventory(Vector2 ps1, Vector2 ps2);
  Vector2 getPos();
  bool getShowInventory();
  PlayerItem getInventoryItem(int i, int j);
  float getLife();
  bool isSwinging();
};

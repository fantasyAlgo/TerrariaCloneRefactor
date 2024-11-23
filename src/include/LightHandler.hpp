#pragma once

#include "Blocks.h"
#include "Player.hpp"
#include "Settings.hpp"
#include <atomic>
#include <mutex>
#include <raylib.h>

class LightHandler {
private:
  unsigned char light_map[(int)BLOCK_SCREEN_RATIO_X + 2][(int)BLOCK_SCREEN_RATIO_Y + 3];
  std::atomic<bool> isRunning;
  std::atomic<bool> needsUpdate;
  std::atomic<bool> isUpdating;
  std::mutex light_map_mutex;
public:
  LightHandler();
  bool run(const Player &player, unsigned char map[MAP_WIDTH][MAP_HEIGHT]);
  void update(Vector2 start_tile, unsigned char map[MAP_WIDTH][MAP_HEIGHT]);

  Color getLightValue(int tileX, int tileY, unsigned char type);
  void askForUpdate();
  void kill();
};

#pragma once

#include "Blocks.h"
#include "Settings.hpp"
#include <atomic>
#include <mutex>
#include <raylib.h>
#include <vector>

class LightHandler {
private:
  unsigned char light_map[1000][1000];
  std::atomic<bool> isRunning;
  std::atomic<bool> needsUpdate;
  std::atomic<bool> isUpdating;
  std::mutex light_map_mutex;
public:
  std::vector<Vector2> torches;
  LightHandler();
  bool run(bool *is_running, Vector2 *pos, unsigned char map[settings::MAP_WIDTH][settings::MAP_HEIGHT], float &time);
  void update(Vector2 start_tile, unsigned char map[settings::MAP_WIDTH][settings::MAP_HEIGHT], float time = 255);

  Color getLightValue(int tileX, int tileY, unsigned char type);
  void askForUpdate();
  void kill();
};

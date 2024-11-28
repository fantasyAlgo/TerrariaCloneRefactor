#include "include/LightHandler.hpp"
#include "include/Blocks.h"
#include "include/Player.hpp"
#include "include/Settings.hpp"
#include "include/TileRenderUtil.hpp"
#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <raylib.h>
#include <vector>

LightHandler::LightHandler()  : isRunning(false), needsUpdate(false){
  for (int i = 0; i < settings::BLOCK_SCREEN_RATIO_X+1; i++)  
    for (int j = 0; j < settings::BLOCK_SCREEN_RATIO_Y+2; j++) 
      light_map[i][j] = 255;
}
bool LightHandler::run(bool *is_running, Vector2 *pos, unsigned char map[settings::MAP_WIDTH][settings::MAP_HEIGHT], float &time){
  isRunning.store(true, std::memory_order_relaxed);
  float xTile, yTile;
  while (is_running){
    //std::cout << pos->x << " " << pos->y << std::endl; 
    if (needsUpdate.load(std::memory_order_relaxed)){
      yTile = floor(pos->y)-((float)settings::SCREEN_HEIGHT/(float)settings::BLOCK_SIZE_Y)/2-2;
      xTile = floor(pos->x)-((float)settings::SCREEN_WIDTH/(float)settings::BLOCK_SIZE_X)/2;
      this->update({xTile, yTile}, map, time);
      needsUpdate.store(false, std::memory_order_release); // Reset after processing
    }
  }
  std::cout << "exploded" << std::endl;
  return true;
}
void LightHandler::update(Vector2 start_tile, unsigned char map[settings::MAP_WIDTH][settings::MAP_HEIGHT], float time){
  //int map_columns[settings::MAP_WIDTH];
  int light_value;
  int highest_empty = -1;
  std::vector<Vector2> light_sources;
  std::vector<Vector2> torch_sources;
  unsigned char light_map_copy[(int)settings::BLOCK_SCREEN_RATIO_X + 3][(int)settings::BLOCK_SCREEN_RATIO_Y + 3];
  //std::cout << "is it updating?: " << start_tile.x << " " << start_tile.y << std::endl;
  //std::cout << "time: " << time << std::endl;
  for (int i = 0; i < settings::BLOCK_SCREEN_RATIO_X+2; i++) {
    light_value = time;
    highest_empty = -1;
    for (int j = 0; j < settings::MAP_HEIGHT; j++) {
      unsigned char map_tile = map[(int)start_tile.x + i][j];
      if (map_tile != WALL_DIRT && !TileRenderUtil::isCollisionTileHelper(map, start_tile.x + i, j, map_tile)){
        highest_empty = highest_empty == -1 ? j : highest_empty;
        light_value -= 80;
      }else if (highest_empty != -1) light_value -= 10;
      light_value = std::max(0, light_value);
      if (j-(int)start_tile.y >= 0 && j-(int)start_tile.y <= settings::BLOCK_SCREEN_RATIO_Y+2)
        light_map_copy[i][j-(int)start_tile.y] = light_value;
      if (map_tile == TORCH)
        torch_sources.push_back({(float)i, (float)j});

    }
  }
  //light_map_copy[0][0] = 0;
  int val;
  for (int i = 0; i < settings::BLOCK_SCREEN_RATIO_X+2; i++) {
    for (int j = 0; j < settings::BLOCK_SCREEN_RATIO_Y+3; j++) {
      for (Vector2 light_source : torch_sources) {
        if (i-light_source.x > 15 || j - light_source.y > 15) continue;
        val = 255- 25*(int)(abs(i-light_source.x) + abs(j+start_tile.y+2 - light_source.y));
        light_map_copy[i][j] = std::max((int)light_map_copy[i][j], val);
      }
    }
  }

  for (int _ = 0; _ < settings::LIGHT_PASSES; _++) {
    for (int i = 1; i < settings::BLOCK_SCREEN_RATIO_X+1; i++) {
      for (int j = 1; j < settings::BLOCK_SCREEN_RATIO_Y+1; j++) {
        light_map_copy[i][j] = ((int)light_map_copy[i-1][j-1] + (int)light_map_copy[i-1][j] + (int)light_map_copy[i-1][j+1] +
                                (int)light_map_copy[i][j-1] + (int)light_map_copy[i][j] + (int)light_map_copy[i][j+1] +
                                (int)light_map_copy[i+1][j-1] + (int)light_map_copy[i+1][j] + (int)light_map_copy[i+1][j+1])/9;
      }
    }
  }
  isUpdating.store(true, std::memory_order_relaxed);
  for (int i = 0; i < settings::BLOCK_SCREEN_RATIO_X+1; i++) 
    for (int j = 0; j < settings::BLOCK_SCREEN_RATIO_Y+2; j++) 
      this->light_map[i][j] = light_map_copy[i][j];
  isUpdating.store(false, std::memory_order_relaxed);

}
Color LightHandler::getLightValue(int tileX, int tileY, unsigned char type){
  //std::cout << "yeah: " << (int)isUpdating.load(std::memory_order_relaxed) << "first tile:  " << tileX << " " << tileY << " then: " << (int)light_map[tileX][tileY] << std::endl;
  unsigned char light_value = light_map[tileX][tileY];
  unsigned char light_value2 = light_value/2;
  if (type != WALL_DIRT)
    return {light_value, 
            light_value, 
            light_value, 255};
  return {light_value2, 
          light_value2, 
          light_value2, 255};

}
void LightHandler::askForUpdate(){
  //std::cout << "asking: " << (int)light_map[0][0] << std::endl;
  needsUpdate.store(true, std::memory_order_relaxed);
}
void LightHandler::kill(){
  isRunning.store(false, std::memory_order_relaxed);
}

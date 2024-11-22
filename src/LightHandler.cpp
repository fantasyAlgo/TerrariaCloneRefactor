#include "include/LightHandler.hpp"
#include "include/Blocks.h"
#include "include/Player.hpp"
#include "include/Settings.hpp"
#include "include/TileRenderUtil.hpp"
#include <algorithm>
#include <atomic>
#include <iostream>
#include <mutex>
#include <raylib.h>

LightHandler::LightHandler()  : isRunning(false), needsUpdate(false){
  for (int i = 0; i < BLOCK_SCREEN_RATIO_X+1; i++)  
    for (int j = 0; j < BLOCK_SCREEN_RATIO_Y+1; j++) 
      light_map[i][j] = 255;
}
bool LightHandler::run(const Player &player, unsigned char map[MAP_WIDTH][MAP_HEIGHT]){
  isRunning.store(true, std::memory_order_relaxed);
  float xTile, yTile;
  while (true){
    //std::cout << "is running" << std::endl; 
    if (needsUpdate.load(std::memory_order_relaxed)){
      yTile = floor(player.pos.y)-((float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y)/2-2;
      xTile = floor(player.pos.x)-((float)SCREEN_WIDTH/(float)BLOCK_SIZE_X)/2;
      this->update({xTile, yTile}, map);
      needsUpdate.store(false, std::memory_order_release); // Reset after processing
    }
  }
  std::cout << "exploded" << std::endl;
  return true;
}
void LightHandler::update(Vector2 start_tile, unsigned char map[MAP_WIDTH][MAP_HEIGHT]){
  //int map_columns[MAP_WIDTH];
  isUpdating.store(true, std::memory_order_relaxed);
  int light_value;
  int highest_empty = -1;
  //std::cout << "is it updating?: " << start_tile.x << " " << start_tile.y << std::endl;

  for (int i = 0; i < BLOCK_SCREEN_RATIO_X+1; i++) {
    light_value = 255;
    highest_empty = -1;
    for (int j = 0; j < MAP_HEIGHT; j++) {
      if (!TileRenderUtil::isCollisionTileHelper(map, start_tile.x + i, j, map[(int)start_tile.x + i][j])){
        highest_empty = highest_empty == -1 ? j : highest_empty;
        light_value -= 30;
      }else if (highest_empty != -1) light_value -= 5;
      light_value = std::max(0, light_value);
      if (j-start_tile.y >= 0 && j-start_tile.y <= BLOCK_SCREEN_RATIO_Y)
        this->light_map[i][j-(int)start_tile.y] = light_value;
    }
  }
  for (int _ = 0; _ < 0; _++) {
    for (int i = 1; i < BLOCK_SCREEN_RATIO_X; i++) {
      for (int j = 1; j < BLOCK_SCREEN_RATIO_Y; j++) {
        this->light_map[i][j] = ((int)light_map[i-1][j-1] + (int)light_map[i-1][j] + (int)light_map[i-1][j+1] +
                                (int)light_map[i][j-1] + (int)light_map[i][j] + (int)light_map[i][j+1] +
                                (int)light_map[i+1][j-1] + (int)light_map[i+1][j] + (int)light_map[i+1][j+1])/9;
      }
    }
  }

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

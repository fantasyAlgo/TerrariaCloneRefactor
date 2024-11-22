#include "include/Game.hpp"
#include "include/Blocks.h"
#include "include/ChunkLoader.hpp"
#include "include/Player.hpp"
#include "include/Settings.hpp"
#include "include/Texture.hpp"
#include "include/TileRenderUtil.hpp"

#include <cmath>
#include <future>
#include <iostream>
#include <ostream>
#include <raylib.h>


Game::Game(){
  this->init();
}
void Game::init(){
  Textures::load();
  this->noise = ValueNoise1D();
  this->noise2d = PerlinNoise();
  isChunkLoaded[(int)this->player.getPos().x/CHUNK_SIZE ] = true;
  //std::cout << "ll: " << this->player.getPos().x/CHUNK_SIZE << std::endl;
  ChunkLoader::loadChunk(this->map, this->player.getPos().x/CHUNK_SIZE , this->noise, this->noise2d);
  this->light_thread = std::async(std::launch::async, &LightHandler::run, &lightHandler, std::ref(this->player), this->map);
}

void Game::update(float deltaTime){
  player.update(map, deltaTime);
  mouse_pos = GetMousePosition();
  ChunkLoader::loadNearbyChunks(this->isChunkLoaded, this->map, this->player.getPos().x/CHUNK_SIZE, this->noise, this->noise2d);
}
void Game::inputHandler(float deltaTime){
  this->player.inputHandler(deltaTime);
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    map[(int)mouse_tile.x][(int)mouse_tile.y] = STONE;
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    map[(int)mouse_tile.x][(int)mouse_tile.y] = 0;

}
void Game::render(){
  lightHandler.askForUpdate();
  DrawTexturePro(Textures::backgroundForest, {0,0, 1024, 838}, {0,0, SCREEN_WIDTH, SCREEN_HEIGHT}, {0,0}, 0, WHITE);
  Vector2 starting_point = {(floor(this->player.getPos().x) - this->player.getPos().x)*BLOCK_SIZE_X, 
                            (floor(this->player.getPos().y) - this->player.getPos().y)*BLOCK_SIZE_Y};
  // Making the grid lines (for debugging purposes)
  //for (int i = starting_point.x; i < SCREEN_WIDTH; i+=BLOCK_SIZE_X) DrawLine(i, 0, i, SCREEN_HEIGHT, BLACK);
  //for (int i = starting_point.y; i < SCREEN_HEIGHT; i+=BLOCK_SIZE_Y) DrawLine(0, i, SCREEN_WIDTH, i, BLACK);

  int yTile = floor(this->player.getPos().y)-((float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y)/2-2;
  int xTile = floor(player.getPos().x)-((float)SCREEN_WIDTH/(float)BLOCK_SIZE_X)/2; 
  mouse_tile = {(float)xTile + (mouse_pos.x-starting_point.x)/(float)BLOCK_SIZE_X, (float)yTile+2 + (mouse_pos.y-starting_point.y)/(float)BLOCK_SIZE_Y};

  for (int i = starting_point.x; i < SCREEN_WIDTH; i+=BLOCK_SIZE_X){
    yTile = floor(this->player.getPos().y)-((float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y)/2-2;
    for (int j = starting_point.y-BLOCK_SIZE_Y; j < SCREEN_HEIGHT; j+=BLOCK_SIZE_Y){
      yTile++;
      this->renderTile(i, j, xTile, yTile, starting_point);
      if (xTile == (int)mouse_tile.x && yTile == (int)mouse_tile.y)
        DrawRectangle(i, j, BLOCK_SIZE_X, BLOCK_SIZE_Y, {0, 0, 0, 125});

    }
    xTile++;
  }
  this->player.render();
}


void Game::renderTile(int i, int j, int xTile, int yTile, Vector2 starting_point){
  Vector2 tileAmbient;
  Rectangle tileRect;
  if (map[xTile][yTile] != 0 && map[xTile][yTile] < TREE_BRANCH){
    tileRect = TileRenderUtil::getTileP(TileRenderUtil::ambientBlock(map, xTile, yTile, map[xTile][yTile], this->noise), map[xTile][yTile]);
    tileRect.x += Textures::tileAtlas[map[xTile][yTile]]; // Add the atlas position
    DrawTexturePro(Textures::all_atlas, 
                   tileRect,
                   {(float)i, (float)j, BLOCK_SIZE_X+1, BLOCK_SIZE_Y+1}, {0,0}, 0, 
                   lightHandler.getLightValue((i-starting_point.x)/BLOCK_SIZE_X, (j-starting_point.y)/BLOCK_SIZE_Y, map[xTile][yTile]));
  }else if (map[xTile][yTile] >= TREE_BRANCH && map[xTile][yTile] != TREE_TOP){
    if (map[xTile][yTile] >= 202){
      tileRect = TileRenderUtil::getTile(map[xTile][yTile]-202, 0);
      tileRect.x += Textures::flowersAtlas; // Add the atlas position
    }else{
      tileRect = TileRenderUtil::getTileP(TileRenderUtil::treeAmbientTile(map, xTile, yTile, map[xTile][yTile], this->noise), map[xTile][yTile]);
      tileRect.x += Textures::tileAtlas[map[xTile][yTile]]; // Add the atlas position
    }
    DrawTexturePro(Textures::all_atlas, 
                   tileRect,
                   {(float)i, (float)j, BLOCK_SIZE_X, BLOCK_SIZE_Y+1}, {0,0}, 0, map[xTile][yTile] == WALL_DIRT ? GRAY : WHITE);

  }else if (map[xTile][yTile] != 0){
    DrawTexturePro(Textures::all_atlas, 
                   {(float)Textures::topTreeAtlas, 0, 82, 82},
                   {(float)i- BLOCK_SIZE_X*1.9f, (float)j-BLOCK_SIZE_Y*1.9f, BLOCK_SIZE_X*5, BLOCK_SIZE_Y*5}, {0,0}, 0, WHITE);

  }


}


Game::~Game(){
  std::cout << "I'm killing you" << std::endl;
  this->lightHandler.kill();
}


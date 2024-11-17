#include "include/Game.hpp"
#include "include/ChunkLoader.hpp"
#include "include/Player.hpp"
#include "include/Settings.hpp"
#include "include/Texture.hpp"

#include <iostream>
#include <raylib.h>


Game::Game(){
  this->init();
}
void Game::init(){
  Textures::load();
  this->noise = ValueNoise1D();
  this->noise2d = PerlinNoise();
  isChunkLoaded[(int)this->player.pos.x/CHUNK_SIZE] = true;
  ChunkLoader::loadChunk(this->map, this->player.pos.x/CHUNK_SIZE, this->noise, this->noise2d);
}
void Game::update(float deltaTime){
}
void Game::inputHandler(float deltaTime){
  this->player.inputHandler(deltaTime);
}
void Game::render(){
  this->player.render();
  Vector2 starting_point = {(float)((int)this->player.pos.x%BLOCK_SIZE_X), (float)((int)this->player.pos.y%BLOCK_SIZE_Y)};
  ChunkLoader::loadNearbyChunks(this->isChunkLoaded, this->map, this->player.pos.x/CHUNK_SIZE, this->noise, this->noise2d);

  //ChunkLoader::loadChunk(this->map, this->player.pos.x - CHUNK_SIZE/2, this->noise, this->noise2d);

  for (int i = starting_point.x; i < SCREEN_WIDTH; i+=BLOCK_SIZE_X) DrawLine(i, 0, i, SCREEN_HEIGHT, BLACK);
  for (int i = starting_point.y; i < SCREEN_HEIGHT; i+=BLOCK_SIZE_Y) DrawLine(0, i, SCREEN_WIDTH, i, BLACK);

  int xTile = player.pos.x-((float)SCREEN_WIDTH/(float)BLOCK_SIZE_X)-1; 
  std::cout << this->player.pos.x << " " <<  this->player.pos.y << " insane: " << xTile << std::endl;
  int yTile;
  for (int i = starting_point.x-BLOCK_SIZE_X; i < SCREEN_WIDTH; i+=BLOCK_SIZE_X){
    yTile = this->player.pos.y+((float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y)+2;
    //std::cout << "why tile: " << yTile << std::endl;
    for (int j = starting_point.y-BLOCK_SIZE_Y; j < SCREEN_HEIGHT; j+=BLOCK_SIZE_Y){
      yTile--;
      if (map[xTile][MAP_HEIGHT-yTile] != 0){
        DrawRectangle(i, j, BLOCK_SIZE_X, BLOCK_SIZE_Y, BLACK);
      }
    }
    xTile++;
  }

}


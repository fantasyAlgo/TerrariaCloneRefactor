#include "include/Game.hpp"
#include "include/ChunkLoader.hpp"
#include "include/Player.hpp"
#include "include/Settings.hpp"
#include "include/Texture.hpp"

#include <cmath>
#include <iostream>
#include <raylib.h>


Game::Game(){
  this->init();
}
void Game::init(){
  Textures::load();
  this->noise = ValueNoise1D();
  this->noise2d = PerlinNoise();
  isChunkLoaded[(int)this->player.getTile().x/CHUNK_SIZE ] = true;
  //std::cout << "ll: " << this->player.getTile().x/CHUNK_SIZE << std::endl;
  ChunkLoader::loadChunk(this->map, this->player.getTile().x/CHUNK_SIZE , this->noise, this->noise2d);
}

void Game::update(float deltaTime){
}
void Game::inputHandler(float deltaTime){
  this->player.inputHandler(deltaTime);
}
void Game::render(){
  this->player.render();
  Vector2 starting_point = {(floor(this->player.getPos().x) - this->player.getPos().x)*BLOCK_SIZE_X, 
                            (floor(this->player.getPos().y) - this->player.getPos().y)*BLOCK_SIZE_Y};
  ChunkLoader::loadNearbyChunks(this->isChunkLoaded, this->map, this->player.getTile().x/CHUNK_SIZE, this->noise, this->noise2d);

  //ChunkLoader::loadChunk(this->map, this->player.pos.x - CHUNK_SIZE/2, this->noise, this->noise2d);

  for (int i = starting_point.x; i < SCREEN_WIDTH; i+=BLOCK_SIZE_X) DrawLine(i, 0, i, SCREEN_HEIGHT, BLACK);
  for (int i = starting_point.y; i < SCREEN_HEIGHT; i+=BLOCK_SIZE_Y) DrawLine(0, i, SCREEN_WIDTH, i, BLACK);

  int yTile;
  int xTile = floor(player.getTile().x)-((float)SCREEN_WIDTH/(float)BLOCK_SIZE_X); 
  for (int i = starting_point.x; i < SCREEN_WIDTH; i+=BLOCK_SIZE_X){
    yTile = floor(this->player.getTile().y)-((float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y)-2;
    //std::cout << "why tile: " << yTile << std::endl;
    for (int j = starting_point.y-BLOCK_SIZE_Y; j < SCREEN_HEIGHT; j+=BLOCK_SIZE_Y){
      yTile++;
      if (map[xTile][yTile] != 0){
        DrawRectangle(i, j, BLOCK_SIZE_X, BLOCK_SIZE_Y, BLACK);
      }
    }
    xTile++;
  }


}

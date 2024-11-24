#include "include/Player.hpp"
#include "include/Blocks.h"
#include "include/Settings.hpp"
#include "include/Texture.hpp"
#include "include/TileRenderUtil.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <raylib.h>


Player::Player(){
  this->pos = {400, 250};
  selected_item = 0;
}
void Player::update(unsigned char map[][settings::MAP_HEIGHT], float deltaTime){
  if (this->force.x > 0 && (!TileRenderUtil::isCollisionTileHelper(map, floor(pos.x)+1, floor(pos.y)) || !TileRenderUtil::isCollisionTileHelper(map, floor(pos.x)+1, floor(pos.y)-1))){
    this->force.x = 0.000001f;
    this->animationFrame = 0;
  }else if (this->force.x < 0 && (!TileRenderUtil::isCollisionTileHelper(map, floor(pos.x), floor(pos.y)) || !TileRenderUtil::isCollisionTileHelper(map, floor(pos.x), floor(pos.y)-1))){
    this->force.x = -0.000001f;
    this->animationFrame = 0;
  }else if (this->isJumping == -1 && (this->force.x > 0 ? this->force.x > deltaTime*settings::PLAYER_SPEED/2 : this->force.x < -deltaTime*settings::PLAYER_SPEED/2))
    this->animationFrame += deltaTime*10.0f;

  this->isJumping += this->isJumping != -1 ? deltaTime : 0;
  if ((this->isJumping == -1 || this->isJumping > deltaTime*10) && !TileRenderUtil::isCollisionTileHelper(map, round(pos.x), floor(pos.y)+1)){
    this->force.y = 0;
    this->isJumping = -1;
  }
  if (!TileRenderUtil::isCollisionTileHelper(map, round(pos.x), floor(pos.y)-1))
    this->force.y = 0.2;

  if (!TileRenderUtil::isCollisionTileHelper(map, round(pos.x), round(pos.y)))
    this->force.y = -30;

  pos.y += deltaTime*this->force.y;
  pos.x += this->force.x;

  this->force.y += deltaTime*100;
  this->force.x *= 0.8;
}
void Player::render(){
  //std::cout << "offsets: " << (int)offset_block_x << " " << (int)offset_block_y << std::endl;
  int mid_pos_x = (settings::SCREEN_WIDTH/(2*settings::BLOCK_SIZE_X))*settings::BLOCK_SIZE_X + (int)settings::offset_block_x*settings::BLOCK_SIZE_X;
  int mid_pos_y = (settings::SCREEN_HEIGHT/(2*settings::BLOCK_SIZE_Y))*settings::BLOCK_SIZE_Y - (int)settings::offset_block_y*settings::BLOCK_SIZE_Y;
  Rectangle player_frame;
  if (this->isJumping == -1 && (this->force.x < GetFrameTime()*settings::PLAYER_SPEED/2 && this->force.x > -GetFrameTime()*settings::PLAYER_SPEED/2))
    player_frame = {(float)Textures::playerAtlasPos, 0, (this->force.x > 0 ? -1.0f : 1.0f)*20.0f, 30};
  else if (this->isJumping == -1)
    player_frame = {(float)Textures::playerAtlasPos + (6+(int)this->animationFrame%6)*20, 0, (this->force.x > 0 ? -1.0f : 1.0f)*20.0f, 30};
  else player_frame = {(float)Textures::playerAtlasPos + 5*20, 0, (this->force.x > 0 ? -1.0f : 1.0f)*20.0f, 30};

  DrawTexturePro(Textures::item_entities_atlas, 
                 player_frame,
                 {(float)mid_pos_x - settings::BLOCK_SIZE_X*0.25f, (float)mid_pos_y - settings::BLOCK_SIZE_Y*0.25f, 
                 1.5*settings::BLOCK_SIZE_X, 2.5*settings::BLOCK_SIZE_Y}, 
                 {0,0}, 0, WHITE);
}
void Player::inputHandler(float deltaTime){
  if (IsKeyDown(KEY_D)) this->force.x = deltaTime*settings::PLAYER_SPEED;//this->pos.x += deltaTime*PLAYER_SPEED;
  if (IsKeyDown(KEY_A)) this->force.x = -deltaTime*settings::PLAYER_SPEED;//this->pos.x -= deltaTime* PLAYER_SPEED;
  if (IsKeyDown(KEY_SPACE) && isJumping == -1){
    this->force.y = -30;
    this->isJumping = 0.0;//this->pos.y -= deltaTime*   PLAYER_SPEED;
  }

  if (IsKeyDown(KEY_UP)) this->force.y = -100;
  if (IsKeyDown(KEY_DOWN)) this->force.y = 100;
  if (IsKeyDown(KEY_LEFT)) this->force.x = -0.5f;
  if (IsKeyDown(KEY_RIGHT)) this->force.x = 0.5f;
  float wheelMove = GetMouseWheelMove();
  //std::cout << selected_item << " " << wheelMove << std::endl;
  if (wheelMove > 0) selected_item = std::min(settings::N_INVENTORY_COLUMNS-1, selected_item+1);
  if (wheelMove < 0) selected_item = std::max(0, selected_item-1);
}

Vector2 Player::getPos(){
  return this->pos;
}
PlayerItem Player::getInventoryItem(int i, int j){
  return inventory[i][j];
}


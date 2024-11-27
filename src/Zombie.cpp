#include "include/Zombie.hpp"
#include "include/Blocks.h"
#include "include/LightHandler.hpp"
#include "include/Settings.hpp"
#include "include/Texture.hpp"
#include "include/TileRenderUtil.hpp"
#include <iostream>
#include <raylib.h>

Zombie::Zombie(){
  this->pos = {400, 280};
  this->force = {0,0};
  this->animationFrame = 0;
  this->needsJump = false;
}
void Zombie::init(Vector2 *new_target){
  this->target = new_target;
}

void Zombie::update(unsigned char map[][settings::MAP_HEIGHT], float deltaTime){

  if (this->force.x > 0 && (!TileRenderUtil::isCollisionTileHelper(map, floor(pos.x)+1, floor(pos.y)) || !TileRenderUtil::isCollisionTileHelper(map, floor(pos.x)+1, floor(pos.y)-1))){
    this->force.x = 0.000001f;
    this->animationFrame = 0;
    this->needsJump = true;
  }else if (this->force.x < 0 && (!TileRenderUtil::isCollisionTileHelper(map, floor(pos.x), floor(pos.y)) || 
    !TileRenderUtil::isCollisionTileHelper(map, floor(pos.x), floor(pos.y)-1))){
    this->force.x = -0.000001f;
    this->animationFrame = 0;
    this->needsJump = true;
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
void Zombie::moveTowardsTarget(float deltaTime){
  if (this->pos.x < target->x) this->force.x = deltaTime*settings::PLAYER_SPEED*0.8f;
  if (this->pos.x > target->x) this->force.x = -deltaTime*settings::PLAYER_SPEED*0.8f;
  if (this->needsJump && isJumping == -1){
    this->force.y = -30;
    this->isJumping = 0.0;
    this->needsJump = false;
  }
}
void Zombie::render(LightHandler &lightHandler){
  int mid_pos_x = (settings::SCREEN_WIDTH/(2*settings::BLOCK_SIZE_X))*settings::BLOCK_SIZE_X + (int)settings::offset_block_x*settings::BLOCK_SIZE_X;
  int mid_pos_y = (settings::SCREEN_HEIGHT/(2*settings::BLOCK_SIZE_Y))*settings::BLOCK_SIZE_Y - (int)settings::offset_block_y*settings::BLOCK_SIZE_Y;
  Vector2 diff_pos = {target->x-pos.x, target->y-pos.y};
  int zombie_pos_x = mid_pos_x-diff_pos.x*settings::BLOCK_SIZE_X;
  int zombie_pos_y = mid_pos_y-diff_pos.y*settings::BLOCK_SIZE_Y;
  Rectangle tile_pos = {Textures::zombieAtlasPos+((int)animationFrame%4)*34.0f, 0, force.x > 0.0f ? -34.0f : 34.0f, 46.0f};
  //std::cout << pos.x << " " << pos.y << " || " << animationFrame << std::endl;
  //DrawRectangle(int posX, int posY, int width, int height, Color color)
  DrawTexturePro(Textures::item_entities_atlas, 
                 tile_pos,
                 {(float)zombie_pos_x -(float)settings::BLOCK_SIZE_X*0.25f, (float)zombie_pos_y- (float)settings::BLOCK_SIZE_Y*0.25f, 
                 1.5f*(float)settings::BLOCK_SIZE_X, 2.5f*(float)settings::BLOCK_SIZE_Y}, 
                 {0,0}, 0, lightHandler.getLightValue(zombie_pos_x/settings::BLOCK_SIZE_X, zombie_pos_y/settings::BLOCK_SIZE_Y, STONE));

  
}

Vector2 Zombie::getPos(){
  return this->pos;
}
void Zombie::setTarget(Vector2 *new_target){
  this->target = new_target;
}
Vector2 Zombie::getTarget(){
  return *this->target;
}


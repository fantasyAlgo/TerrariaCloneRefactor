#include "include/Player.hpp"
#include "include/Settings.hpp"
#include "include/TileRenderUtil.hpp"
#include <cmath>
#include <iostream>
#include <raylib.h>


Player::Player(){
  this->pos = {400, 250};
}
void Player::update(unsigned char map[][MAP_HEIGHT], float deltaTime){
  if (this->force.x > 0 && !TileRenderUtil::isCollisionTileHelper(map, floor(pos.x)+1, floor(pos.y)))
    this->force.x = 0.0f;
  else if (this->force.x < 0 && !TileRenderUtil::isCollisionTileHelper(map, floor(pos.x)-1, floor(pos.y)))
    this->force.x = 0.0f;

  pos.y += deltaTime*this->force.y;
  pos.x += this->force.x;

  if (!TileRenderUtil::isCollisionTileHelper(map, floor(pos.x), floor(pos.y)+1)){
    std::cout << "hello world" << std::endl;
    this->force.y = 0.0f;
    this->isJumping = false;
  }


  this->force.y += deltaTime*100;
  this->force.x *= 0.1;
}
void Player::render(){
  DrawRectangle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 40, BLACK);
}
void Player::inputHandler(float deltaTime){
  if (IsKeyDown(KEY_RIGHT)) this->force.x = deltaTime*PLAYER_SPEED;//this->pos.x += deltaTime*PLAYER_SPEED;
  if (IsKeyDown(KEY_LEFT)) this->force.x = -deltaTime*PLAYER_SPEED;//this->pos.x -= deltaTime* PLAYER_SPEED;
  if (IsKeyDown(KEY_SPACE) && !isJumping){
    this->force.y = -deltaTime*100000.0f;
    this->isJumping = true;//this->pos.y -= deltaTime*   PLAYER_SPEED;
  }
  //if (IsKeyDown(KEY_DOWN)) this->pos.y += deltaTime* PLAYER_SPEED;
}

Vector2 Player::getPos(){
  return this->pos;
}


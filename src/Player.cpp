#include "include/Player.hpp"
#include "include/Settings.hpp"
#include <raylib.h>


Player::Player(){
  this->pos = {400, 400};
}
void Player::update(){
  this->pos.x += 0.1;
}
void Player::render(){
  DrawRectangle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 40, BLACK);
}
void Player::inputHandler(float deltaTime){
  if (IsKeyDown(KEY_RIGHT)) this->pos.x += deltaTime*PLAYER_SPEED;
  if (IsKeyDown(KEY_LEFT)) this->pos.x -= deltaTime* PLAYER_SPEED;
  if (IsKeyDown(KEY_DOWN)) this->pos.y -= deltaTime*   PLAYER_SPEED;
  if (IsKeyDown(KEY_UP)) this->pos.y += deltaTime* PLAYER_SPEED;
}


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
  this->toolAnimation = 0;
  for (int i = 0; i < settings::N_INVENTORY_ROWS; i++) 
    for (int j = 0; j < settings::N_INVENTORY_COLUMNS+1; j++) 
      inventory[i][j] = {EMPTY, 1, EMPTY_TOOL, 0};
  inventory[settings::N_INVENTORY_ROWS-1][settings::N_INVENTORY_COLUMNS-1] = {TORCH, 1, EMPTY_TOOL, 10};
  inventory[0][0] = {EMPTY, 0, SWORD, 1};
  inventory[0][1] = {EMPTY, 0, PICKAXE, 1};
  inventory[0][2] = {EMPTY, 0, AXE, 1};
}
void Player::update(unsigned char map[][settings::MAP_HEIGHT], float deltaTime){
  if (this->force.x > 0 && (!TileRenderUtil::isCollisionTileHelper(map, floor(pos.x)+1, floor(pos.y)) || !TileRenderUtil::isCollisionTileHelper(map, floor(pos.x)+1, floor(pos.y)-1))){
    this->force.x = 0.000001f;
    this->animationFrame = 0;
  }else if (this->force.x < 0 && (!TileRenderUtil::isCollisionTileHelper(map, floor(pos.x), floor(pos.y)) || 
    !TileRenderUtil::isCollisionTileHelper(map, floor(pos.x), floor(pos.y)-1))){
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

  if (this->toolAnimation > -30)
    this->toolAnimation += deltaTime*300.0f;
  if (this->toolAnimation >= 90) this->toolAnimation = -30;

}
void Player::render(Color player_light){
  //std::cout << "offsets: " << (int)offset_block_x << " " << (int)offset_block_y << std::endl;
  int mid_pos_x = (settings::SCREEN_WIDTH/(2*settings::BLOCK_SIZE_X))*settings::BLOCK_SIZE_X + (int)settings::offset_block_x*settings::BLOCK_SIZE_X;
  int mid_pos_y = (settings::SCREEN_HEIGHT/(2*settings::BLOCK_SIZE_Y))*settings::BLOCK_SIZE_Y - (int)settings::offset_block_y*settings::BLOCK_SIZE_Y;
  Rectangle player_frame;
  if (this->toolAnimation > 30)
    player_frame = {(float)Textures::playerAtlasPos + (int)(1+(((toolAnimation+30.0f)/120.0f)* 4.0f))*20, 0, (this->force.x > 0 ? -1.0f : 1.0f)*20.0f, 30};
  else if (this->isJumping == -1 && (this->force.x < GetFrameTime()*settings::PLAYER_SPEED/2 && this->force.x > -GetFrameTime()*settings::PLAYER_SPEED/2))
    player_frame = {(float)Textures::playerAtlasPos, 0, (this->force.x > 0 ? -1.0f : 1.0f)*20.0f, 30};
  else if (this->isJumping == -1)
    player_frame = {(float)Textures::playerAtlasPos + (6+(int)this->animationFrame%6)*20, 0, (this->force.x > 0 ? -1.0f : 1.0f)*20.0f, 30};
  else player_frame = {(float)Textures::playerAtlasPos + 5*20, 0, (this->force.x > 0 ? -1.0f : 1.0f)*20.0f, 30};

  DrawTexturePro(Textures::item_entities_atlas, 
                 player_frame,
                 {(float)mid_pos_x - (float)settings::BLOCK_SIZE_X*0.25f, (float)mid_pos_y - (float)settings::BLOCK_SIZE_Y*0.25f, 
                 1.5f*(float)settings::BLOCK_SIZE_X, 2.5f*(float)settings::BLOCK_SIZE_Y}, 
                 {0,0}, 0, player_light);
}

void Player::renderTool(Color player_light){
  if (this->inventory[0][this->selected_item].puttable) return;
  if (this->inventory[0][this->selected_item].toolId == EMPTY_TOOL) return;
  auto inventoryItem = this->inventory[0][this->selected_item];
  int mid_pos_x = (settings::SCREEN_WIDTH/(2*settings::BLOCK_SIZE_X))*settings::BLOCK_SIZE_X + (int)settings::offset_block_x*settings::BLOCK_SIZE_X;
  int mid_pos_y = (settings::SCREEN_HEIGHT/(2*settings::BLOCK_SIZE_Y))*settings::BLOCK_SIZE_Y - (int)settings::offset_block_y*settings::BLOCK_SIZE_Y;
  Rectangle swordRect = {(float)Textures::toolAtlas[inventoryItem.toolId], 0, (force.x > 0?1.0f:-1.0f)*31, 31};

  DrawTexturePro(Textures::item_entities_atlas,  swordRect,
                 {(float)mid_pos_x + (float)settings::BLOCK_SIZE_X*(this->force.x > 0 ?0.8f : 0.0f), 
                 (float)mid_pos_y + (float)settings::BLOCK_SIZE_Y*0.8f, 

                 (float)settings::BLOCK_SIZE_X*1.2f, (float)settings::BLOCK_SIZE_Y*1.2f}, 
                 {force.x>0.0f? 0.0f : settings::BLOCK_SIZE_X, (float)settings::BLOCK_SIZE_Y}, (force.x>0?1.0f:-1.0f)*this->toolAnimation, player_light);
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
  if (IsKeyPressed(KEY_E)) this->isInventoryOpen = !this->isInventoryOpen;
  float wheelMove = GetMouseWheelMove();
  //std::cout << selected_item << " " << wheelMove << std::endl;
  if (wheelMove > 0) selected_item = std::min(settings::N_INVENTORY_COLUMNS-1, selected_item+1);
  if (wheelMove < 0) selected_item = std::max(0, selected_item-1);
  for (int i = 0; i < 9; i++) {
   if (IsKeyPressed(KEY_ONE + i))
       selected_item = i;
  }
}

void Player::addBlockToInventory(BlockType id){
  if (id == EMPTY || id == WALL_DIRT) return;
  int firstEmpty[2] = {-1, -1};
  for (int i = 0; i < settings::N_INVENTORY_ROWS; i++) {
    for (int j = 0; j < settings::N_INVENTORY_COLUMNS; j++) {
      if (firstEmpty[0] == -1 && this->inventory[i][j].id == EMPTY && this->inventory[i][j].toolId == EMPTY_TOOL){
        firstEmpty[0] = i;
        firstEmpty[1] = j;
      }
      if (this->inventory[i][j].id == id){
        this->inventory[i][j].amount += 1;
        return;
      }
    }
  }
  this->inventory[firstEmpty[0]][firstEmpty[1]] = {id, 1, EMPTY_TOOL, 1};
}
void Player::reduceSelectedBlock(){
  this->inventory[0][this->selected_item].amount -= 1;
  if (this->inventory[0][this->selected_item].amount <= 0) this->inventory[0][this->selected_item] = {EMPTY, 0, EMPTY_TOOL, 0};
}
void Player::initAction(){
  if (this->toolAnimation == -30)
    this->toolAnimation = -29.99;
}

void Player::swapShowInventory(Vector2 ps1, Vector2 ps2){
  auto tmp = this->inventory[(int)ps1.y][(int)ps1.x];
  this->inventory[(int)ps1.y][(int)ps1.x] = this->inventory[(int)ps2.y][(int)ps2.x];
  this->inventory[(int)ps2.y][(int)ps2.x] = tmp;

}

Vector2 Player::getPos(){
  return this->pos;
}
bool Player::getShowInventory(){
  return this->isInventoryOpen;
}
PlayerItem Player::getInventoryItem(int i, int j){
  return inventory[i][j];
}


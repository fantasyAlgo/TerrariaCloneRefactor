#include "include/Settings.hpp"

namespace settings {
 int SCREEN_WIDTH;
 int SCREEN_HEIGHT;
 int BLOCK_SIZE_X;
 int BLOCK_SIZE_Y;

 float BLOCK_SCREEN_RATIO_X;
 float BLOCK_SCREEN_RATIO_Y;
 bool offset_block_x;
 bool offset_block_y;

 float PLAYER_SPEED;

void init_settings(){

  SCREEN_WIDTH = 1920.0f *(0.75f);
  SCREEN_HEIGHT = 1080.0f *(0.75f);
  BLOCK_SIZE_X = 20;
  BLOCK_SIZE_Y = 20;

  BLOCK_SCREEN_RATIO_X = (float)SCREEN_WIDTH/(float)BLOCK_SIZE_X;
  BLOCK_SCREEN_RATIO_Y = (float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y;
  offset_block_x = SCREEN_WIDTH%BLOCK_SIZE_X != 0;
  offset_block_y = SCREEN_HEIGHT%BLOCK_SIZE_Y == 0;

  PLAYER_SPEED = 10;
}
void setWindowSettings(int width, int height){
  SCREEN_WIDTH = width; //*(0.75f);
  SCREEN_HEIGHT = height; //*(0.75f);

  BLOCK_SCREEN_RATIO_X = (float)SCREEN_WIDTH/(float)BLOCK_SIZE_X;
  BLOCK_SCREEN_RATIO_Y = (float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y;
  offset_block_x = SCREEN_WIDTH%BLOCK_SIZE_X != 0;
  offset_block_y = SCREEN_HEIGHT%BLOCK_SIZE_Y == 0;
}

void setBlockSettings(int block_x, int block_y){
  BLOCK_SIZE_X = block_x;
  BLOCK_SIZE_Y = block_y;

  BLOCK_SCREEN_RATIO_X = (float)SCREEN_WIDTH/(float)BLOCK_SIZE_X;
  BLOCK_SCREEN_RATIO_Y = (float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y;
  offset_block_x = SCREEN_WIDTH%BLOCK_SIZE_X != 0;
  offset_block_y = SCREEN_HEIGHT%BLOCK_SIZE_Y == 0;
}


}


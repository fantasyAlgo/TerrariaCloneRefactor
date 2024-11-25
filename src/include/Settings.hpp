#pragma once


namespace settings {

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int BLOCK_SIZE_X;
extern int BLOCK_SIZE_Y;

extern float BLOCK_SCREEN_RATIO_X;
extern float BLOCK_SCREEN_RATIO_Y;
extern bool offset_block_x;
extern bool offset_block_y;

extern float PLAYER_SPEED;

constexpr int BLOCK_CHUNK = 18;
constexpr int LIGHT_PASSES = 4;

constexpr int MAXINT = 2147483647;
constexpr int CAMERA_BLOCKS = 58;
constexpr int LIGHT_CAMERA_BLOCKS = 78;
constexpr long long MAP_WIDTH = 48*100;
constexpr int MAP_HEIGHT = 640;
constexpr int CHUNK_SIZE = 48;
constexpr int NOISE_VARIABILITY = 40;

constexpr int N_INVENTORY_ROWS  = 4;
constexpr int N_INVENTORY_COLUMNS  = 9;

void init_settings();
void setWindowSettings(int width, int height);
void setBlockSettings(int block_x, int block_y);
  
}


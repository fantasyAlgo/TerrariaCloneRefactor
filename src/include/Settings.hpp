#pragma once

constexpr int SCREEN_WIDTH = 1920.0f;//*(0.75f);
constexpr int SCREEN_HEIGHT = 1080.0f;//*(0.75f);
constexpr int BLOCK_SIZE_X = 25;
constexpr int BLOCK_SIZE_Y = 25;
constexpr float BLOCK_SCREEN_RATIO_X = (float)SCREEN_WIDTH/(float)BLOCK_SIZE_X;
constexpr float BLOCK_SCREEN_RATIO_Y = (float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y;

constexpr float PLAYER_SPEED = 10;
constexpr int BLOCK_CHUNK = 18;

constexpr int MAXINT = 2147483647;
constexpr int CAMERA_BLOCKS = 58;
constexpr int LIGHT_CAMERA_BLOCKS = 78;
constexpr long long MAP_WIDTH = 48*100;
constexpr int MAP_HEIGHT = 640;
constexpr int CHUNK_SIZE = 48;
constexpr int LIGHT_PASSES = 10;
constexpr int NOISE_VARIABILITY = 40;


constexpr bool offset_block_x = SCREEN_WIDTH%BLOCK_SIZE_X != 0;
constexpr bool offset_block_y = SCREEN_HEIGHT%BLOCK_SIZE_Y == 0;

//void init_settings();

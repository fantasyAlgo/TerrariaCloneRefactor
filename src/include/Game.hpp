#pragma once
#include "PerlinNoise.hpp"
#include "Player.hpp"
#include "Settings.hpp"
#include "raylib.h"
#include <unordered_map>

class Game {
private:
  Player player;
  ValueNoise1D noise; // 1d perlin noise (like yeah...)
  PerlinNoise noise2d; // 2d perlin noise (like yeah.)
  unsigned char map[MAP_WIDTH][MAP_HEIGHT]; // terraria map
  std::unordered_map<int, bool> isChunkLoaded; // i mean do i need to explain this?
public:
  Game();
  void init();
  void update(float deltaTime);
  void inputHandler(float deltaTime);
  void render();
};

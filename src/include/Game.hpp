#pragma once
#include "LightHandler.hpp"
#include "PerlinNoise.hpp"
#include "Player.hpp"
#include "Settings.hpp"
#include "raylib.h"
#include <future>
#include <unordered_map>
#include "imgui.h"

enum GameState {
  HOME,
  IN_GAME,
  MAIN_PAUSE,
};

class Game {
private:
  Player player;
  Vector2 mouse_tile;
  Vector2 mouse_pos;
  GameState state;

  ValueNoise1D noise; // 1d perlin noise (like yeah...)
  PerlinNoise noise2d; // 2d perlin noise (like yeah.)
  unsigned char map[MAP_WIDTH][MAP_HEIGHT]; // terraria map
  std::future<bool> light_thread;
  LightHandler lightHandler;
  std::unordered_map<int, bool> isChunkLoaded; // i mean do i need to explain this?

public:
  Game();
  ~Game();
  void init();
  void update(float deltaTime);
  void inputHandler(float deltaTime);
  void render();

  void renderUI();
  void renderInGameUI();
  void renderHomeGUI();
  void renderPauseGUI();

  void renderTile(int i, int j, int xTile, int yTile, Vector2 starting_point);
};

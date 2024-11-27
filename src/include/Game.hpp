#pragma once
#include "LightHandler.hpp"
#include "PerlinNoise.hpp"
#include "Player.hpp"
#include "Settings.hpp"
#include "Zombie.hpp"
#include "raylib.h"
#include <future>
#include <unordered_map>
#include "imgui.h"

enum GameState {
  HOME,
  OPTIONS,
  IN_GAME,
  MAIN_PAUSE,
};

class Game {
private:
  Player player;
  Vector2 mouse_tile;
  Vector2 mouse_pos;
  GameState state;
  bool is_running;
  float time;

  ValueNoise1D noise; // 1d perlin noise (like yeah...)
  PerlinNoise noise2d; // 2d perlin noise (like yeah.)
  unsigned char map[settings::MAP_WIDTH][settings::MAP_HEIGHT]; // terraria map
  std::future<bool> light_thread;
  LightHandler lightHandler;
  std::unordered_map<int, bool> isChunkLoaded; // i mean do i need to explain this? do i really have enough time for this, thy goddes of mine, descended upon heaven to save us from our sins?
  Zombie first_zombie;

public:
  Game();
  ~Game();
  void init();
  void update(float deltaTime);
  void inputHandler(float deltaTime);
  void render();

  void renderUI();
  void renderInGameUI();
  void renderOptionsUI();
  void renderHomeGUI();
  void renderPauseGUI();

  void renderTile(int i, int j, int xTile, int yTile, unsigned char type, Vector2 starting_point);

  bool getIsRunning();
};

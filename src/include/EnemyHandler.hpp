#pragma once

#include "LightHandler.hpp"
#include "Player.hpp"
#include "Zombie.hpp"
#include <raylib.h>
#include <vector>
class EnemyHandler {
private:
  std::vector<Zombie> zombies;
public:
  EnemyHandler();
  void addEnemy(Vector2 *player_pos);
  void update(unsigned char map[][settings::MAP_HEIGHT], Player &player, float deltaTime);
  void render(LightHandler &lightHandler);
};

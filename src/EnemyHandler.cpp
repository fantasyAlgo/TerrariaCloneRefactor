#include "include/EnemyHandler.hpp"
#include "include/Settings.hpp"
#include <algorithm>
#include <cstdlib>
#include <raylib.h>

EnemyHandler::EnemyHandler(){

}
void EnemyHandler::addEnemy(Vector2 *player_pos){
  Zombie zombie;
  zombie.init(player_pos, {player_pos->x - 10*((100 - rand()%200) > 0 ? 1 : -1), player_pos->y});
  zombies.push_back(zombie);
}
void EnemyHandler::update(unsigned char map[][settings::MAP_HEIGHT], Player &player, float deltaTime){
  for (auto &zombie : this->zombies) {
    zombie.update(map, deltaTime);
    zombie.moveTowardsTarget(deltaTime);
    if (player.isColliding(zombie))
      player.hit(zombie);
    if (player.isCollidingSwing(zombie)){
      zombie.hit(player.pos);
      player.hasHit = true;
    }
  }
  this->zombies.erase(std::remove_if(this->zombies.begin(), this->zombies.end(),
    [](const Zombie &zombie) {
        return zombie.life <= 0; // Condition for removal
    }), this->zombies.end());

}
void EnemyHandler::render(LightHandler &lightHandler){
  for (auto &zombie : this->zombies) {
    zombie.render(lightHandler);
  }
}


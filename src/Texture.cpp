#include "include/Texture.hpp"
#include "include/Blocks.h"
#include <raylib.h>

namespace Textures {

Texture2D all_atlas;
int tileAtlas[255];

int terrainAtlas;
int grassAtlas;
int stoneAtlas;

int oreAtlas;
int treeAtlas;
int flowersAtlas;
int torchAtlas;
int woodAtlas;
int waterAtlas = 0;

void load(){
  all_atlas = LoadTexture("src/Assets/spritesheet.png");
  terrainAtlas = 0;
  stoneAtlas = 288;
  grassAtlas = stoneAtlas + 1474;
  flowersAtlas = grassAtlas+288;
  woodAtlas = flowersAtlas + 810;
  torchAtlas = woodAtlas + 288;
  oreAtlas = torchAtlas + 132;
  treeAtlas = oreAtlas + 288;

  tileAtlas[DIRT] = terrainAtlas;
  tileAtlas[GRASS] = grassAtlas;
  tileAtlas[STONE] = stoneAtlas;
  tileAtlas[COPPER_ORE] = oreAtlas;
  tileAtlas[WALL_DIRT] = terrainAtlas;

  tileAtlas[SOURCE_WATER] = woodAtlas;
  tileAtlas[WATER_RIGHT] = woodAtlas;
  tileAtlas[WATER_LEFT] = woodAtlas;
  tileAtlas[WATER_DOWN] = woodAtlas;

  tileAtlas[TORCH] = terrainAtlas;
  tileAtlas[WOOD] = woodAtlas;
  tileAtlas[WORKBENCH] = woodAtlas;

  tileAtlas[TREE_TRUNK] = woodAtlas;

}

}


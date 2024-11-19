#include "include/Texture.hpp"
#include "include/Blocks.h"
#include <raylib.h>

namespace Textures {

Texture2D all_atlas;
Texture2D item_entities_atlas;
Texture2D backgroundForest;


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
  item_entities_atlas = LoadTexture("src/Assets/item_entity_atlas.png");
  backgroundForest = LoadTexture("src/Assets/Forest_background_7.png");

  grassAtlas = 0;
  stoneAtlas = 288;
  terrainAtlas = stoneAtlas+288;
  flowersAtlas = 288*3;
  woodAtlas = flowersAtlas + 810;
  torchAtlas = woodAtlas + 288;
  treeAtlas = torchAtlas + 288;
  oreAtlas = 4126-48-288;

  grassAtlas = 0;

  tileAtlas[DIRT] = terrainAtlas;
  tileAtlas[GRASS] = grassAtlas;
  tileAtlas[STONE] = stoneAtlas;
  tileAtlas[COPPER_ORE] = oreAtlas;
  tileAtlas[WALL_DIRT] = grassAtlas;

  tileAtlas[SOURCE_WATER] = woodAtlas;
  tileAtlas[WATER_RIGHT] = woodAtlas;
  tileAtlas[WATER_LEFT] = woodAtlas;
  tileAtlas[WATER_DOWN] = woodAtlas;

  tileAtlas[TORCH] = terrainAtlas;
  tileAtlas[WOOD] = woodAtlas;
  tileAtlas[WORKBENCH] = woodAtlas;

  tileAtlas[TREE_TRUNK] = treeAtlas;
  tileAtlas[TREE_BRANCH] = treeAtlas;
  tileAtlas[TREE_TOP] = treeAtlas;


}

}


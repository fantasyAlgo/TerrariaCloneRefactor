#include "include/Texture.hpp"
#include "include/Blocks.h"
#include <raylib.h>

namespace Textures {
// Atlases
Texture2D all_atlas;
Texture2D item_entities_atlas;

// Various images
Texture2D backgroundForest;
Texture2D logoImage;
Texture2D logoImageOptions;



int tileAtlas[255];
int toolAtlas[255];

int terrainAtlas;
int grassAtlas;
int stoneAtlas;

int oreAtlas;
int treeAtlas;
int flowersAtlas;
int torchAtlas;
int woodAtlas;
int waterAtlas;
int topTreeAtlas;

int playerAtlasPos;
void load(){
  all_atlas = LoadTexture("src/Assets/spritesheet.png");
  item_entities_atlas = LoadTexture("src/Assets/item_entity_atlas.png");
  backgroundForest = LoadTexture("src/Assets/Forest_background_7.png");
  logoImage = LoadTexture("src/Assets/NewPromoLogo.png");
  logoImageOptions = LoadTexture("src/Assets/optionsLogo.png");

  grassAtlas = 0;
  stoneAtlas = 288;
  terrainAtlas = stoneAtlas+288;
  flowersAtlas = 288*3;
  woodAtlas = flowersAtlas + 810;
  torchAtlas = woodAtlas + 288;
  treeAtlas = torchAtlas + 132;
  oreAtlas = 4126-48-288;
  topTreeAtlas = oreAtlas + 288;
  waterAtlas = topTreeAtlas + 246;

  grassAtlas = 0;
  playerAtlasPos = 326;

  tileAtlas[DIRT] = terrainAtlas;
  tileAtlas[GRASS] = grassAtlas;
  tileAtlas[STONE] = stoneAtlas;
  tileAtlas[COPPER_ORE] = oreAtlas;
  tileAtlas[WALL_DIRT] = grassAtlas;

  tileAtlas[SOURCE_WATER] = woodAtlas;
  tileAtlas[WATER_RIGHT] = waterAtlas;
  tileAtlas[WATER_LEFT] = waterAtlas;
  tileAtlas[WATER_DOWN] = waterAtlas;

  tileAtlas[TORCH] = torchAtlas;
  tileAtlas[WOOD] = woodAtlas;
  tileAtlas[WORKBENCH] = woodAtlas;

  tileAtlas[TREE_TRUNK] = treeAtlas;
  tileAtlas[TREE_BRANCH] = treeAtlas;
  tileAtlas[TREE_TOP] = topTreeAtlas;

  int swordImg = 40*3 + 32*2;
  int pickaxeImg =  40*3;
  int axeImg = 40*3 + 31;
  toolAtlas[SWORD] = swordImg;
  toolAtlas[PICKAXE] = pickaxeImg;
  toolAtlas[AXE] = axeImg;


}

}


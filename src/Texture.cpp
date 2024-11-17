#include "include/Texture.hpp"
#include <raylib.h>

namespace Textures {

Texture2D all_atlas;
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
}

}


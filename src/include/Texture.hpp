#pragma once
#include <raylib.h>

namespace Textures {

extern Texture2D all_atlas;
extern Texture2D item_entities_atlas;

extern Texture2D backgroundForest;
extern Texture2D logoImage;
extern Texture2D logoImageOptions;

extern int terrainAtlas;
extern int grassAtlas;
extern int stoneAtlas;

extern int oreAtlas;
extern int treeAtlas;
extern int flowersAtlas;
extern int torchAtlas;
extern int woodAtlas;
extern int topTreeAtlas;

extern int playerAtlasPos;
extern int zombieAtlasPos;

extern int tileAtlas[255];
extern int toolAtlas[255];

void load();

}


#include "include/TileRenderUtil.hpp"

#include "include/Blocks.h"
#include "include/PerlinNoise.hpp"
#include "include/Settings.hpp"
#include <iostream>
#include <raylib.h>
#include <utility>
#include <vector>

namespace TileRenderUtil {

bool isWater(unsigned char map[][MAP_HEIGHT], int i, int j){
    return map[i][j] == WATER_LEFT || map[i][j] == SOURCE_WATER || map[i][j] == WATER_RIGHT || map[i][j] == WATER_DOWN;
}
bool isWaterTile(int tile){
    return tile == WATER_LEFT || tile == SOURCE_WATER || tile == WATER_RIGHT || tile == WATER_DOWN;
}
bool isNotIn(BlockType type, std::vector<BlockType> types){
    for (auto t : types)
        if (t == type) return true;
    
    return false;
}

bool isCollisionTileHelper(unsigned char map[][MAP_HEIGHT], int i, int j, int type){
    if (i < 0 || j < 0 || i >= MAP_WIDTH || j >= MAP_HEIGHT)
        return true;
    if (type == WALL_DIRT) return map[i][j] == EMPTY;
    return map[i][j] == TORCH || map[i][j] == 0 || (map[i][j] > TREE_TOP && map[i][j] < 254) || map[i][j] == WALL_DIRT || TileRenderUtil::isWater(map, i, j) 
            || map[i][j] == TREE_TRUNK || map[i][j] == TREE_BRANCH || map[i][j] == TREE_TOP || map[i][j] == WORKBENCH;
}

Color addLightGreen(Color color, int value){
    return {color.r, (unsigned char)(color.g+value), color.b, color.a};
}
Vector2 ambientBlock(unsigned char map[][MAP_HEIGHT], int i, int j, int type, ValueNoise1D  &noise1d){
    if (type == 200)
        return {0, 0};
    if (type == (int)WORKBENCH)
        return {map[i-1][j] == WORKBENCH ? 1.0f : 0.0f, 0.0f};
    if (isWaterTile(type)){
        return {0,0};
        if (isWater(map, i+1, j) && isWater(map, i, j+1) && isWater(map, i-1, j) && isWater(map, i, j-1)) return {1, 3};

        else if (!isWater(map, i+1, j) && isWater(map, i, j+1) &&          !isWater(map, i-1, j) && !isWater(map, i, j-1)) return {1, 1};
        else if (!isWater(map, i+1, j) && isWater(map, i, j+1) &&          !isWater(map, i-1, j) && isWater(map, i, j-1)) return {1, 2};
        else if (!isWater(map, i+1, j) && !isWater(map, i, j+1) &&         !isWater(map, i-1, j) && isWater(map, i, j-1)) return {1, 2};
        else if (!isWater(map, i+1, j) && (true|| !isWater(map, i,j+1)) && isWater(map, i-1, j) &&  isWater(map, i, j-1)) return {2, 1};
        else if (isWater(map, i+1, j) && (true||  !isWater(map, i,j+1)) && !isWater(map, i-1, j) && isWater(map, i, j-1)) return {0, 1};
        else if (isWater(map, i+1, j) && (true||  !isWater(map, i,j+1)) && isWater(map, i-1, j) &&  isWater(map, i, j-1)) return {1, 3};
        else if (!isWater(map, i+1, j) && (true|| isWater(map, i,j+1)) &&  isWater(map, i-1, j) &&  !isWater(map, i, j-1)) return {2, 0};
        else if (isWater(map, i+1, j) && (true||  isWater(map, i,j+1)) &&  !isWater(map, i-1, j) && !isWater(map, i, j-1)) return {0, 0};
        else if (isWater(map, i+1, j) && (true||  isWater(map, i,j+1)) &&  isWater(map, i-1, j) &&  !isWater(map, i, j-1)) return {1, 0};
        else if (isWater(map, i+1, j)) return {1, 3};
        return {0, 0};
    }

    auto isCollisionTile = [type, map](int i, int j) -> bool {return TileRenderUtil::isCollisionTileHelper(map, i, j, type);};
    //std::cout << "noise: " << noise1d.eval(50.0f + ((float)i)/10.0f) << std::endl;
    if (!isCollisionTile(i+1,j) && !isCollisionTile(i,j+1) && !isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {3, 0};
    else if (!isCollisionTile(i+1,j) && isCollisionTile(i,j+1) && !isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {1, 0};
    else if (!isCollisionTile(i+1,j) && isCollisionTile(i,j+1) && !isCollisionTile(i-1,j) && !isCollisionTile(i,j-1))
        return {1, 2};
    else if (isCollisionTile(i+1,j) && !isCollisionTile(i,j+1) && !isCollisionTile(i-1,j) && !isCollisionTile(i,j-1))
        return {4, 0};
    else if (!isCollisionTile(i+1,j) && !isCollisionTile(i,j+1) && isCollisionTile(i-1,j) && !isCollisionTile(i,j-1))
        return {0,0};
    else if (!isCollisionTile(i+1,j) && !isCollisionTile(i,j+1) && isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {2, 3};
    else if (isCollisionTile(i+1,j) && !isCollisionTile(i,j+1) && !isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {3, 3};
    else if (isCollisionTile(i+1,j) && isCollisionTile(i,j+1) && !isCollisionTile(i-1,j) && !isCollisionTile(i,j-1))
        return {3, 4};
    else if (!isCollisionTile(i+1,j) && isCollisionTile(i,j+1) && isCollisionTile(i-1,j) && !isCollisionTile(i,j-1))
        return {2, 4};
    else if (isCollisionTile(i+1,j) && !isCollisionTile(i,j+1) && isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {6, 5};
    else if (isCollisionTile(i+1,j) && isCollisionTile(i,j+1) && !isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {12, 0};
    else if (!isCollisionTile(i+1,j) && isCollisionTile(i,j+1) && isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {9, 0};
    else if (isCollisionTile(i+1,j) && !isCollisionTile(i,j+1) && isCollisionTile(i-1,j) && !isCollisionTile(i,j-1))
        return {5, 1};
    else if (isCollisionTile(i+1,j) && isCollisionTile(i,j+1) && isCollisionTile(i-1,j) && !isCollisionTile(i,j-1))
        return {6, 3};
    else if (isCollisionTile(i+1,j) && isCollisionTile(i,j+1) && isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {9, 3};
    return {2, 1};
}

Vector2 treeAmbientTile(unsigned char map[][MAP_HEIGHT], int i, int j, int type, ValueNoise1D  &noise1d){
  if (type == TREE_TRUNK){
    if (map[i][j-3] == 201)
      return {0, 0};
    return {0,0};// noise1d.eval((i+j)/25)*5};
  }else if (type == TREE_BRANCH){
    if (map[i-1][j] == TREE_TRUNK)
      return {4, 3};
    //return {4, noise1d.eval((i+j)/25)*3};
    return {3, 0};
  }
  return {0,0};
}

bool isTileWall(int x, int y, ValueNoise1D noise){
  return y <= ((float)MAP_HEIGHT/2 - (noise.eval(((float)x)/NOISE_VARIABILITY))*MAP_HEIGHT/4)+10;
}
Rectangle getTile(int x, int y, int type){
    if (type == TREE_BRANCH || type == TREE_TRUNK){ return {(float)x*22, (float)y*BLOCK_CHUNK, BLOCK_CHUNK, BLOCK_CHUNK}; }
    return {(float)x*BLOCK_CHUNK, (float)y*BLOCK_CHUNK, BLOCK_CHUNK, BLOCK_CHUNK};
}
Rectangle getTileP(Vector2 pos, int type, int time){
    if (type == TORCH) return {(float)pos.x*22, (float)pos.y*22, 22, 22}; 
    if (type == TREE_TRUNK || type == TREE_BRANCH) return {(float)pos.x*22, (float)pos.y*22, BLOCK_CHUNK, BLOCK_CHUNK}; 
    if (type == (int)WORKBENCH) return {pos.x*22.0f, 0, 11, 20};
    
    if (isWaterTile(type))
        return {pos.x*16.0f, pos.y*16.0f+80.0f*(1.0f+(float)(time%150)/10.0f), 16, 16};
    return {(float)pos.x*BLOCK_CHUNK, (float)pos.y*BLOCK_CHUNK, BLOCK_CHUNK-1, BLOCK_CHUNK-1};
}
Rectangle getTileWithSize(int x, int y, float size){
    return {(float)x*BLOCK_CHUNK, (float)y*BLOCK_CHUNK, size, size};
}


}
namespace TileUpdateUtil{

bool hasSource(unsigned char map[][MAP_HEIGHT], int iTile, int jTile){
    switch (map[iTile][jTile]){
        case WATER_LEFT:
            return TileRenderUtil::isWater(map, iTile+1, jTile);
        case WATER_RIGHT:
            return TileRenderUtil::isWater(map, iTile-1, jTile);
        case WATER_DOWN:
            return TileRenderUtil::isWater(map, iTile, jTile-1);
        case SOURCE_WATER:
            return true;
        default:
            return false;
    }
}
void removeUnSourcedWater(unsigned char map[][MAP_HEIGHT], int iTile, int jTile, ValueNoise1D noise){
    if (!hasSource(map, iTile, jTile)){
        if (jTile > (MAP_HEIGHT/2 - (noise.eval(((float)(iTile))/NOISE_VARIABILITY))*MAP_HEIGHT/4)+10)
            map[iTile][jTile] = WALL_DIRT;
        else map[iTile][jTile] = EMPTY;
    }
}
void updateWater(unsigned char map[][MAP_HEIGHT], int iTile, int jTile){
    if (!hasSource(map, iTile, jTile)) return;
    if (TileRenderUtil::isCollisionTileHelper(map, iTile, jTile + 1) && !TileRenderUtil::isWater(map, iTile, jTile+1)){
        map[iTile][jTile+1] = WATER_DOWN;
    }else if (!TileRenderUtil::isCollisionTileHelper(map, iTile, jTile + 1) && !TileRenderUtil::isWater(map, iTile, jTile+1)){
        if (TileRenderUtil::isCollisionTileHelper(map, iTile+1, jTile) && !TileRenderUtil::isWater(map, iTile+1, jTile)){
            map[iTile+1][jTile] = WATER_RIGHT;
        }
        if (TileRenderUtil::isCollisionTileHelper(map, iTile - 1, jTile) && !TileRenderUtil::isWater(map, iTile-1, jTile)){
            map[iTile-1][jTile] = WATER_LEFT;
        }
    }
}
void updateTree(unsigned char map[][MAP_HEIGHT], int iTile, int jTile){
    if (map[iTile][jTile] != TREE_TRUNK && map[iTile][jTile] != TREE_TOP) return;
    if (map[iTile][jTile] == TREE_TOP && map[iTile][jTile+3] == EMPTY){
        map[iTile][jTile] = EMPTY;
    }
    if (map[iTile][jTile] != TREE_TOP && map[iTile][jTile+1] == EMPTY){
        map[iTile][jTile] = EMPTY;
        if (map[iTile+1][jTile] == TREE_BRANCH)
            map[iTile+1][jTile] = EMPTY;
        if (map[iTile-1][jTile] == TREE_BRANCH)
            map[iTile-1][jTile] = EMPTY;
    }
}
int followUpTree(unsigned char map[][MAP_HEIGHT], int iTile, int jTile){
    int i = iTile;
    int j = jTile;
    int nTrunks = 0;
    while (map[i][j] == TREE_TRUNK){
        j--;
        nTrunks += map[i-1][j] == TREE_BRANCH;
        nTrunks += map[i+1][j] == TREE_BRANCH;
        nTrunks++;
    }
    return nTrunks+1;
}

double distanceTiles(int iTile, int jTile, int iTile2, int jTile2){
    return sqrt((double)(iTile- iTile2)*(iTile- iTile2) + (double)(jTile - jTile2)*(jTile - jTile2));
}


}

#include "include/TileRenderUtil.hpp"

#include "include/Blocks.h"
#include "include/PerlinNoise.hpp"
#include "include/Settings.hpp"
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
    return map[i][j] == 0 || (map[i][j] > TREE_TOP && map[i][j] < 254) || map[i][j] == WALL_DIRT || TileRenderUtil::isWater(map, i, j) 
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
    auto isCollisionTile = [type, map](int i, int j) -> bool {return isCollisionTileHelper(map, i, j, type);};
    //std::cout << "noise: " << noise1d.eval(50.0f + ((float)i)/10.0f) << std::endl;
    if (!isCollisionTile(i+1,j) && !isCollisionTile(i,j+1) && !isCollisionTile(i-1,j) && isCollisionTile(i,j-1))
        return {1+noise1d.eval(50.0f + ((float)i)/10.0f)*3, 0};
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
    if (type == 200){
        if (map[i][j-3] == 201)
            return {0, 0};
        return {0, noise1d.eval((i+j)/25)*5};
    }else if (type == 199){
        if (map[i-1][j] == 200)
            return {4, 3+noise1d.eval((i+j)/25)*3};
        else return {3, 0+noise1d.eval((i+j)/25)*3};
    }
    return {0,0};
}

}
namespace TileUpdateUtil{

bool hasSource(int iTile, int jTile);
void removeUnSourcedWater(int iTile, int jTile, ValueNoise1D noise);
void updateWater(int iTile, int jTile);
void updateTree(int iTile, int jTile);
int followUpTree(int iTile, int jTile);
double distanceTiles(int iTile, int jTile, int iTile2, int jTile2);

}

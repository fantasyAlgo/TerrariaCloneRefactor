#include "Blocks.h"
#include "PerlinNoise.hpp"
#include "Settings.hpp"
#include <raylib.h>

namespace TileRenderUtil {

bool isWater(unsigned char map[][settings::MAP_HEIGHT], int i, int j);
bool isWaterTile(int tile);
bool isNotIn(BlockType type, std::vector<BlockType> types);
bool isCollisionTileHelper(unsigned char map[][settings::MAP_HEIGHT], int i, int j, int type=1);
Color addLightGreen(Color color, int value);
Vector2 ambientBlock(unsigned char map[][settings::MAP_HEIGHT], int i, int j, int type, ValueNoise1D  &noise1d);
Vector2 treeAmbientTile(unsigned char map[][settings::MAP_HEIGHT], int i, int j, int type, ValueNoise1D  &noise1d);

bool isTileWall(int x, int y, ValueNoise1D noise);
Rectangle getTile(int x, int y, int type = -1);
Rectangle getTileP(Vector2 pos, int type = 0, int time = 1);
Rectangle getTileWithSize(int x, int y, float size);

}
namespace TileUpdateUtil{

bool hasSource(unsigned char map[][settings::MAP_HEIGHT], int iTile, int jTile);
void removeUnSourcedWater(unsigned char map[][settings::MAP_HEIGHT], int iTile, int jTile, ValueNoise1D noise);
void updateWater(unsigned char map[][settings::MAP_HEIGHT], int iTile, int jTile);
void updateTree(unsigned char map[][settings::MAP_HEIGHT], int iTile, int jTile);
int followUpTree(unsigned char map[][settings::MAP_HEIGHT], int iTile, int jTile);
double distanceTiles(int iTile, int jTile, int iTile2, int jTile2);

}

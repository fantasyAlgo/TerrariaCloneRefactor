#pragma once
#include "Blocks.h"
#include "PerlinNoise.hpp"
#include "Settings.hpp"
#include <cstdlib>
#include <unordered_map>

namespace ChunkLoader {

void makeTree(unsigned char map[][settings::MAP_HEIGHT], int i, int j, int height);
void loadChunk(unsigned char map[][settings::MAP_HEIGHT], int current_chunk, ValueNoise1D &noise, PerlinNoise &noise2D);
void loadNearbyChunks(std::unordered_map<int, bool> &isChunkLoaded, unsigned char map[][settings::MAP_HEIGHT], 
                      int current_chunk, ValueNoise1D &noise, PerlinNoise &noise2D);
}

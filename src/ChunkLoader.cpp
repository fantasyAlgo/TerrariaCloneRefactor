#include "include/PerlinNoise.hpp"

#include "include/Blocks.h"
#include "include/PerlinNoise.hpp"
#include "include/Settings.hpp"
#include <cstdlib>
#include <unordered_map>

namespace ChunkLoader {

void makeTree(unsigned char map[][settings::MAP_HEIGHT], int i, int j, int height){
    int randV; 
    for (int k = 1; k < height; k++){
        map[i][j-k] = TREE_TRUNK;
        randV = rand()%100;
        if (randV > 80) map[i-1][j-k] = TREE_BRANCH;
        else if (randV > 70) map[i+1][j-k] = TREE_BRANCH;
    }
    map[i][j-height-2] = TREE_TOP;
}

void loadChunk(unsigned char map[][settings::MAP_HEIGHT], int current_chunk, ValueNoise1D &noise, PerlinNoise &noise2D){
  int start_chunk = current_chunk*settings::CHUNK_SIZE;
  float yNoise;
  float noise2DValue;
  float ore2Dnoise;
  float waterNoise;
  bool grassUpPlaced;
  int depthHollow = 0;
  float treeProb;
  int iTreePlace = -50;

  float flowerProb;
  int iFlowerPlace = -50;
  for (int i = start_chunk; i < start_chunk+settings::CHUNK_SIZE; i++){
    yNoise = ((int)(settings::MAP_HEIGHT/2) - (noise.eval(((float)i)/settings::NOISE_VARIABILITY))*settings::MAP_HEIGHT/4);
    grassUpPlaced = false;
    depthHollow = 0;
    flowerProb = (i - iFlowerPlace > 2) ? rand() % 100 : 0;
    treeProb = ((i - iTreePlace) > 4) ? rand() % 100 : 0;
    for (int j = 0; j < settings::MAP_HEIGHT; j++){
      //map[i][j] = 0;
      noise2DValue = (1+noise2D.eval({((float)i)/25, ((float)j)/25, 1.0}))/2;
      ore2Dnoise = (1+noise2D.eval({((float)i)/50, ((float)j)/50, 53.0}))/2;
      waterNoise = (1+noise2D.eval({((float)i)/50, ((float)j)/50, 2.0}))/2;
      //std::cout << "noise2D: " << (1+noise2D.eval({((float)i)/50, ((float)j)/50, 1.0}))/2 << std::endl;
      if (map[i][j] == 199) continue;
      if (j > yNoise+1 && noise2DValue > 0.45){
        depthHollow++;
        if (grassUpPlaced && depthHollow > 7 && (ore2Dnoise > 0.66 || (noise2DValue < 0.47 && ore2Dnoise > 0.58))){
          map[i][j] = COPPER_ORE;
          continue;
        }
        map[i][j] = !grassUpPlaced ? DIRT : depthHollow < 16 ? GRASS : STONE;
        if (!grassUpPlaced && flowerProb > 80 && j <= yNoise + 10){
            map[i][j-1] = 202+rand()%45;
            iFlowerPlace = i;
        }
        if (!grassUpPlaced) grassUpPlaced = true;
      } else if (j > yNoise && noise2DValue > 0.45){
        map[i][j] = DIRT;
        grassUpPlaced = true;
        //std::cout << "Dirt: " << i << ", " << j << std::endl;
        if (treeProb > 80 && (i - iTreePlace) > 4){
            ChunkLoader::makeTree(map, i, j, rand() % 20 + 5);
            iTreePlace = i;
        }else if (flowerProb > 30 && j <= yNoise + 10){
            map[i][j-1] = 202+rand()%45;
            iFlowerPlace = i;
        }
          
      } else{
        if (j > yNoise + 10)
            map[i][j] = WALL_DIRT;
        else map[i][j] = EMPTY;
        if (waterNoise > 0.63 && j > yNoise+10 && noise2DValue < 0.47)
            map[i][j] = SOURCE_WATER;
      }
    }
  }

}

void loadNearbyChunks(std::unordered_map<int, bool> &isChunkLoaded, unsigned char map[][settings::MAP_HEIGHT], int current_chunk, ValueNoise1D &noise, PerlinNoise &noise2D){
  if (isChunkLoaded.find(current_chunk+1) == isChunkLoaded.end()){
    isChunkLoaded[current_chunk+1] = true;
    ChunkLoader::loadChunk(map, current_chunk+1, noise, noise2D);
  }
  if (isChunkLoaded.find(current_chunk-1) == isChunkLoaded.end()){
    isChunkLoaded[current_chunk-1] = true;
    ChunkLoader::loadChunk(map, current_chunk-1, noise, noise2D);
  }
}
}


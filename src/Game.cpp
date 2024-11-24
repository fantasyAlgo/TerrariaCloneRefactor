#include "include/Game.hpp"
#include "include/Blocks.h"
#include "include/ChunkLoader.hpp"
#include "include/Player.hpp"
#include "include/Settings.hpp"
#include "include/Texture.hpp"
#include "include/TileRenderUtil.hpp"
#include "imgui.h"
#include "rlImGui.h"

#include <cmath>
#include <future>
#include <iostream>
#include <ostream>
#include <raylib.h>


Game::Game(){
  this->init();
}
void Game::init(){
  Textures::load();
  this->noise = ValueNoise1D();
  this->noise2d = PerlinNoise();
  isChunkLoaded[(int)this->player.getPos().x/CHUNK_SIZE ] = true;
  //std::cout << "ll: " << this->player.getPos().x/CHUNK_SIZE << std::endl;
  ChunkLoader::loadChunk(this->map, this->player.getPos().x/CHUNK_SIZE , this->noise, this->noise2d);
  this->light_thread = std::async(std::launch::async, &LightHandler::run, &lightHandler, std::ref(this->player), this->map);
  this->state = HOME;
  this->is_running = true;
}

void Game::update(float deltaTime){
  if (this->state == GameState::HOME) return;
  player.update(map, deltaTime);
  mouse_pos = GetMousePosition();
  ChunkLoader::loadNearbyChunks(this->isChunkLoaded, this->map, this->player.getPos().x/CHUNK_SIZE, this->noise, this->noise2d);
}
void Game::inputHandler(float deltaTime){
  this->player.inputHandler(deltaTime);
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    map[(int)mouse_tile.x][(int)mouse_tile.y] = player.getInventoryItem(0, player.selected_item).id;
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    map[(int)mouse_tile.x][(int)mouse_tile.y] = 0;

}
void Game::render(){
  if (this->state == GameState::HOME) return;
  lightHandler.askForUpdate();
  float player_background_y = (player.pos.y/MAP_HEIGHT/4)*838;
  //std::cout << "player: " << player.pos.y << std::endl;
  DrawTexturePro(Textures::backgroundForest, {0,player_background_y, 1024, SCREEN_HEIGHT/3}, {0,0, SCREEN_WIDTH, SCREEN_HEIGHT}, {0,0}, 0, WHITE);
  Vector2 starting_point = {(floor(this->player.getPos().x) - this->player.getPos().x)*BLOCK_SIZE_X, 
                            (floor(this->player.getPos().y) - this->player.getPos().y)*BLOCK_SIZE_Y};

  // Making the grid lines (for debugging purposes)
  //for (int i = starting_point.x; i < SCREEN_WIDTH; i+=BLOCK_SIZE_X) DrawLine(i, 0, i, SCREEN_HEIGHT, BLACK);
  //for (int i = starting_point.y; i < SCREEN_HEIGHT; i+=BLOCK_SIZE_Y) DrawLine(0, i, SCREEN_WIDTH, i, BLACK);

  int yTile = floor(this->player.getPos().y)-((float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y)/2-2;
  int xTile = floor(player.getPos().x)-((float)SCREEN_WIDTH/(float)BLOCK_SIZE_X)/2; 
  mouse_tile = {(float)xTile + (mouse_pos.x-starting_point.x)/(float)BLOCK_SIZE_X, (float)yTile+2 + (mouse_pos.y-starting_point.y)/(float)BLOCK_SIZE_Y};

  for (int i = starting_point.x; i < SCREEN_WIDTH; i+=BLOCK_SIZE_X){
    yTile = floor(this->player.getPos().y)-((float)SCREEN_HEIGHT/(float)BLOCK_SIZE_Y)/2-2;
    for (int j = starting_point.y-BLOCK_SIZE_Y; j < SCREEN_HEIGHT; j+=BLOCK_SIZE_Y){
      yTile++;
      if (map[xTile][yTile] == TORCH){
        this->renderTile(i, j, xTile, yTile, TileRenderUtil::isTileWall(xTile, yTile, noise) ? EMPTY : WALL_DIRT, starting_point);
        this->renderTile(i, j, xTile, yTile, TORCH, starting_point);
      }else this->renderTile(i, j, xTile, yTile, map[xTile][yTile], starting_point);
      if (xTile == (int)mouse_tile.x && yTile == (int)mouse_tile.y)
        DrawRectangle(i, j, BLOCK_SIZE_X, BLOCK_SIZE_Y, {0, 0, 0, 125});
    }
    xTile++;
  }
  this->player.render();
}


void Game::renderUI(){
  switch (this->state) {
    case GameState::IN_GAME:
      this->renderInGameUI();
      break;
    case GameState::HOME:
      this->renderHomeGUI();
      break;
    case GameState::MAIN_PAUSE:
      this->renderPauseGUI();
      break;
  }

}

void Game::renderInGameUI(){
  Vector2 slotSize = {35, 35};
  Vector2 imageSize = {30, 30};

  ImVec2 vec = ImVec2(SCREEN_WIDTH/256, slotSize.y-SCREEN_HEIGHT/32);
  ImGui::SetNextWindowPos(vec);
  ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));

  for (int i = 0; i < N_INVENTORY_COLUMNS; i++) {
    ImVec2 slotPos = ImGui::GetCursorScreenPos();
    ImU32 slotColor = (i == player.selected_item) ? IM_COL32(77, 128, 230, 255) : IM_COL32(50, 50, 50, 255);

    // Draw background for the slot
    ImGui::GetWindowDrawList()->AddRectFilled(slotPos, ImVec2(slotPos.x + slotSize.x, slotPos.y + slotSize.y), slotColor, 7);
    ImGui::SameLine();
    if (player.getInventoryItem(0, i).id < LAST_BLOCK) {
      // Get UV coordinates for the current block type
      Rectangle uv;
      if (player.getInventoryItem(0, i).id == EMPTY)
        uv = {0, 1000, BLOCK_CHUNK-1, BLOCK_CHUNK-1};
      else uv = {(float)Textures::tileAtlas[player.getInventoryItem(0, i).id], 0, BLOCK_CHUNK-1, BLOCK_CHUNK-1};
      // Compute position of the item image within the slot
      ImVec2 imagePos = ImVec2(
          slotPos.x + (slotSize.x - imageSize.x) * 0.5f,
          slotPos.y + (slotSize.y - imageSize.y) * 0.5f
      );
      // Use rlImGui to set the cursor position
      ImGui::SetCursorScreenPos(imagePos);
      // Use ImGui::Image to draw the texture with UV coordinates
      // Pass the raylib Texture2D's OpenGL texture ID
      rlImGuiImageRect(&Textures::all_atlas, imageSize.x, imageSize.y, uv);
    }
    // Draw item count in the center of the slot if there are items
    if (player.getInventoryItem(0, i).amount > 0) {
      std::string countText = std::to_string(player.getInventoryItem(0, i).amount);
      ImVec2 textSize = ImGui::CalcTextSize(countText.c_str());
      ImVec2 textPos = ImVec2(
          slotPos.x + (slotSize.x - textSize.x) * 0.5f,
          slotPos.y + (slotSize.y - textSize.y) * 0.5f
      );
      ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), countText.c_str());
    }
    if (i < N_INVENTORY_COLUMNS - 1) 
      ImGui::SameLine();
  }

  ImGui::PopStyleVar();
  ImGui::End();
}
void Game::renderHomeGUI(){
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); // Green button
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f)); // Green hover effect
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Green active effect
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // White text
  //
  ImVec2 bottomSize = ImVec2(SCREEN_WIDTH/4 , 50);
  float start_height = SCREEN_HEIGHT/3;
  float offset = 50;
  // Set window to full screen
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
  
  // Set window flags to make it fixed and fullscreen
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
  ImGui::Begin("Fullscreen Window", nullptr, windowFlags);
  rlImGuiImageSize(&Textures::backgroundForest, SCREEN_WIDTH, SCREEN_HEIGHT);
  // Add UI elements here
  ImGui::SetCursorPos(ImVec2(SCREEN_WIDTH/2-SCREEN_WIDTH/5, start_height-offset*4));
  rlImGuiImageSize(&Textures::logoImage, 486.0f*(3.0f/2.0f), 142.0f*(3.0f/2.0f));
  //ImGui::Text("Terraria", ImVec2(SCREEN_WIDTH/3, SCREEN_HEIGHT/10));
  ImGui::SetWindowFontScale(2.0f);
  ImGui::SetCursorPos(ImVec2(SCREEN_WIDTH/2 - SCREEN_WIDTH/8, start_height+offset));
  if (ImGui::Button("Play", bottomSize)) 
    this->state = GameState::IN_GAME;
  ImGui::SetCursorPos(ImVec2(SCREEN_WIDTH/2 - SCREEN_WIDTH/8, start_height+offset*3));
  if (ImGui::Button("Exit", bottomSize))
    this->is_running = false;
  ImGui::End();
  ImGui::PopStyleColor(4); // Pop all three colors we pushed
}
void Game::renderPauseGUI(){}


void Game::renderTile(int i, int j, int xTile, int yTile, unsigned char type, Vector2 starting_point){
  if (type == EMPTY) return;
  Vector2 tileAmbient;
  Rectangle tileRect;
  if (type == TORCH){
    tileRect = TileRenderUtil::getTileP({0,0}, TORCH);
    tileRect.x += Textures::tileAtlas[type]; // Add the atlas position
    DrawTexturePro(Textures::all_atlas, 
                   tileRect,
                   {(float)i, (float)j, BLOCK_SIZE_X, BLOCK_SIZE_Y}, {0,0}, 0, 
                   lightHandler.getLightValue((i-starting_point.x)/BLOCK_SIZE_X, (j-starting_point.y)/BLOCK_SIZE_Y, type));
  }else if (type != 0 && type < TREE_BRANCH){
    tileRect = TileRenderUtil::getTileP(TileRenderUtil::ambientBlock(map, xTile, yTile, type, this->noise), type);
    tileRect.x += Textures::tileAtlas[type]; // Add the atlas position
    DrawTexturePro(Textures::all_atlas, 
                   tileRect,
                   {(float)i, (float)j, BLOCK_SIZE_X+1, BLOCK_SIZE_Y+1}, {0,0}, 0, 
                   lightHandler.getLightValue((i-starting_point.x)/BLOCK_SIZE_X, (j-starting_point.y)/BLOCK_SIZE_Y, type));
  }else if (type >= TREE_BRANCH && type != TREE_TOP){
    if (type >= 202){
      tileRect = TileRenderUtil::getTile(type-202, 0);
      tileRect.x += Textures::flowersAtlas; // Add the atlas position
    }else{
      tileRect = TileRenderUtil::getTileP(TileRenderUtil::treeAmbientTile(map, xTile, yTile, type, this->noise), type);
      tileRect.x += Textures::tileAtlas[type]; // Add the atlas position
    }
    DrawTexturePro(Textures::all_atlas, 
                   tileRect,
                   {(float)i, (float)j, BLOCK_SIZE_X, BLOCK_SIZE_Y+1}, {0,0}, 0, type == WALL_DIRT ? GRAY : WHITE);

  }else if (type != 0){
    DrawTexturePro(Textures::all_atlas, 
                   {(float)Textures::topTreeAtlas, 0, 82, 82},
                   {(float)i- BLOCK_SIZE_X*1.9f, (float)j-BLOCK_SIZE_Y*1.9f, BLOCK_SIZE_X*5, BLOCK_SIZE_Y*5}, {0,0}, 0, WHITE);

  }

}

bool Game::getIsRunning(){
  return this->is_running;
}

Game::~Game(){
  std::cout << "I'm killing you" << std::endl;
  this->lightHandler.kill();
}


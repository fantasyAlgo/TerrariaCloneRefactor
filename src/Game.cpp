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
#include <sched.h>
#include <string>


Game::Game(){
  this->init();
}
void Game::init(){
  Textures::load();
  this->noise = ValueNoise1D();
  this->noise2d = PerlinNoise();
  isChunkLoaded[(int)this->player.getPos().x/settings::CHUNK_SIZE ] = true;
  //std::cout << "ll: " << this->player.getPos().x/settings::CHUNK_SIZE << std::endl;
  ChunkLoader::loadChunk(this->map, this->player.getPos().x/settings::CHUNK_SIZE , this->noise, this->noise2d);
  this->light_thread = std::async(std::launch::async, &LightHandler::run, &lightHandler, std::ref(this->player), this->map);
  this->state = HOME;
  this->is_running = true;
}

void Game::update(float deltaTime){
  if (this->state != GameState::IN_GAME) return;
  player.update(map, deltaTime);
  mouse_pos = GetMousePosition();
  ChunkLoader::loadNearbyChunks(this->isChunkLoaded, this->map, this->player.getPos().x/settings::CHUNK_SIZE, this->noise, this->noise2d);
}
void Game::inputHandler(float deltaTime){
  if (IsKeyPressed(KEY_ESCAPE)){
    if (this->state == HOME) this->is_running = false;
    if (this->state == IN_GAME || this->state == OPTIONS) this->state = GameState::HOME;
  }
  if (this->state != IN_GAME) return;
  this->player.inputHandler(deltaTime);
  int mouse_x = (settings::SCREEN_WIDTH/2 - GetMouseX());
  int mouse_y = (settings::SCREEN_HEIGHT/2 - GetMouseY());
  if (sqrt(mouse_x*mouse_x + mouse_y*mouse_y) > 200) return;

  unsigned char mapTile = map[(int)mouse_tile.x][(int)mouse_tile.y];
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
    if ((mapTile == EMPTY || mapTile == WALL_DIRT) && player.getInventoryItem(0, player.selected_item).id != EMPTY){
      map[(int)mouse_tile.x][(int)mouse_tile.y] = player.getInventoryItem(0, player.selected_item).id;
      player.reduceSelectedBlock();
    }
  }
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
    if (player.getInventoryItem(0, player.selected_item).toolId != EMPTY_TOOL)
      this->player.initAction();
    if (player.getInventoryItem(0, player.selected_item).toolId == SWORD || player.getInventoryItem(0, player.selected_item).puttable) return;
    player.addBlockToInventory((BlockType)mapTile);
    map[(int)mouse_tile.x][(int)mouse_tile.y] = !TileRenderUtil::isTileWall(mouse_tile.x, mouse_tile.y, noise) ? WALL_DIRT : EMPTY;
  }

}
void Game::render(){
  if (this->state != IN_GAME) return;
  lightHandler.askForUpdate();
  float player_background_y = (player.pos.y/settings::MAP_HEIGHT/4)*838;
  //std::cout << "player: " << player.pos.y << std::endl;
  DrawTexturePro(Textures::backgroundForest, {0,player_background_y, 1024, (float)settings::SCREEN_HEIGHT/3}, 
                 {0,0, (float)settings::SCREEN_WIDTH, (float)settings::SCREEN_HEIGHT}, {0,0}, 0, WHITE);
  Vector2 starting_point = {(floor(this->player.getPos().x) - this->player.getPos().x)*settings::BLOCK_SIZE_X, 
                            (floor(this->player.getPos().y) - this->player.getPos().y)*settings::BLOCK_SIZE_Y};

  // Making the grid lines (for debugging purposes)
  //for (int i = starting_point.x; i < settings::SCREEN_WIDTH; i+=BLOCK_SIZE_X) DrawLine(i, 0, i, SCREEN_HEIGHT, BLACK);
  //for (int i = starting_point.y; i < SCREEN_HEIGHT; i+=BLOCK_SIZE_Y) DrawLine(0, i, settings::SCREEN_WIDTH, i, BLACK);

  int yTile = floor(this->player.getPos().y)-((float)settings::SCREEN_HEIGHT/(float)settings::BLOCK_SIZE_Y)/2-2;
  int xTile = floor(player.getPos().x)-((float)settings::SCREEN_WIDTH/(float)settings::BLOCK_SIZE_X)/2; 
  mouse_tile = {(float)xTile + (mouse_pos.x-starting_point.x)/(float)settings::BLOCK_SIZE_X, 
                (float)yTile+2 + (mouse_pos.y-starting_point.y)/(float)settings::BLOCK_SIZE_Y};

  for (int i = starting_point.x; i < settings::SCREEN_WIDTH; i+=settings::BLOCK_SIZE_X){
    yTile = floor(this->player.getPos().y)-((float)settings::SCREEN_HEIGHT/(float)settings::BLOCK_SIZE_Y)/2-2;
    for (int j = starting_point.y-settings::BLOCK_SIZE_Y; j < settings::SCREEN_HEIGHT; j+=settings::BLOCK_SIZE_Y){
      yTile++;
      if (map[xTile][yTile] == TORCH){
        this->renderTile(i, j, xTile, yTile, TileRenderUtil::isTileWall(xTile, yTile, noise) ? EMPTY : WALL_DIRT, starting_point);
        this->renderTile(i, j, xTile, yTile, TORCH, starting_point);
      }else this->renderTile(i, j, xTile, yTile, map[xTile][yTile], starting_point);
      if (xTile == (int)mouse_tile.x && yTile == (int)mouse_tile.y){
        //std::cout << "tile light: " << (i-starting_point.x)/settings::BLOCK_SIZE_X << " " << (j-starting_point.y)/settings::BLOCK_SIZE_Y << 
        //  "| light value: " << (int)lightHandler.getLightValue((i-starting_point.x)/settings::BLOCK_SIZE_X, (j-starting_point.y)/settings::BLOCK_SIZE_Y, STONE).r << std::endl;
        DrawRectangle(i, j, settings::BLOCK_SIZE_X, settings::BLOCK_SIZE_Y, {0, 0, 0, 125});
      }
    }
    xTile++;
  }
  Color player_color = lightHandler.getLightValue(settings::BLOCK_SCREEN_RATIO_X/2, settings::BLOCK_SCREEN_RATIO_Y/2, STONE);
  this->player.render(player_color);
  this->player.renderTool(player_color);
}


void Game::renderUI(){
  switch (this->state) {
    case GameState::IN_GAME:
      this->renderInGameUI();
      break;
    case GameState::HOME:
      this->renderHomeGUI();
      break;
    case GameState::OPTIONS:
      this->renderOptionsUI();
      break;
    case GameState::MAIN_PAUSE:
      this->renderPauseGUI();
      break;
  }

}

void Game::renderInGameUI(){
  Vector2 slotSize = {35, 35};
  Vector2 imageSize = {30, 30};

  ImVec2 vec = ImVec2(settings::SCREEN_WIDTH/256, slotSize.y-settings::SCREEN_HEIGHT/32);
  ImGui::SetNextWindowPos(vec);
  ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));
  //if (player.getShowInventory()) ImGui::ShowDemoWindow();  
  for (int j = 0; j < (player.getShowInventory() ? settings::N_INVENTORY_ROWS : 1); j++) { 
    float rowY = vec.y + j * (slotSize.y + ImGui::GetStyle().ItemSpacing.y + 5.0f);
    ImGui::SetCursorScreenPos(ImVec2(vec.x, rowY));
    for (int i = 0; i < settings::N_INVENTORY_COLUMNS; i++) {
      PlayerItem inventoryItem = player.getInventoryItem(j, i);
      //std::cout << i << " " << j << std::endl;
      ImVec2 slotPos = ImGui::GetCursorScreenPos();
      ImU32 slotColor = (i == player.selected_item && j == 0) ? IM_COL32(77, 128, 230, 255) : IM_COL32(50, 50, 50, 255);

      // Draw background for the slot
      ImGui::GetWindowDrawList()->AddRectFilled(slotPos, ImVec2(slotPos.x + slotSize.x, slotPos.y + slotSize.y), slotColor, 7);
      ImGui::SameLine();
      if (inventoryItem.id < LAST_BLOCK) {
        // Get UV coordinates for the current block type
        Rectangle uv;
        if (inventoryItem.id == EMPTY && inventoryItem.toolId == EMPTY_TOOL)
          uv = {0, 1000, settings::BLOCK_CHUNK-1, settings::BLOCK_CHUNK-1};
        else {
          if (inventoryItem.puttable)
            uv = {(float)Textures::tileAtlas[inventoryItem.id], 0, settings::BLOCK_CHUNK-1, settings::BLOCK_CHUNK-1};
          else {
            uv = {(float)Textures::toolAtlas[inventoryItem.toolId], 0, 32, 32};
          }
        }
        // Compute position of the item image within the slot
        ImVec2 imagePos = ImVec2(
            slotPos.x + (slotSize.x - imageSize.x) * 0.5f,
            slotPos.y + (slotSize.y - imageSize.y) * 0.5f
        );
        // Use rlImGui to set the cursor position
        ImGui::SetCursorScreenPos(imagePos);
        // Use ImGui::Image to draw the texture with UV coordinates
        // Pass the raylib Texture2D's OpenGL texture ID
        if (inventoryItem.puttable)
          rlImGuiImageRect(&Textures::all_atlas, imageSize.x, imageSize.y, uv);
        rlImGuiImageRect(&Textures::item_entities_atlas, imageSize.x, imageSize.y, uv);
      }
      ImGui::SetCursorScreenPos(slotPos);
      ImGui::PushID(i * 100 + j);
      ImGui::InvisibleButton("slot", {slotSize.x, slotSize.y});
      //std::cout << "that is why thou shouldth doth thus!" << std::endl;
      if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)){
        player.slotIndices[0] = j;
        player.slotIndices[1] = i;
        ImGui::SetDragDropPayload("DND_DEMO_CELL", &player.slotIndices, sizeof(player.slotIndices));
        ImGui::Text("Dragging slot (%d, %d)", j, i);
        ImGui::EndDragDropSource();
      }
      if (ImGui::BeginDragDropTarget()){
        //std::cout << "fair" << std::endl;
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL");
        if (payload != nullptr){
          IM_ASSERT(payload->DataSize == sizeof(player.slotIndices));
          int* payload_n = (int*) payload->Data;
          std::cout << "bob: " << payload_n[0] << " " << payload_n[1] << std::endl;
          player.swapShowInventory({(float)payload_n[1], (float)payload_n[0]}, {(float)i,(float)j});
        }
        ImGui::EndDragDropTarget();
      }

      ImGui::PopID();
      // Draw item count in the center of the slot if there are items
      if (player.getInventoryItem(j, i).amount > 0) {
        std::string countText = std::to_string(player.getInventoryItem(0, i).amount);
        ImVec2 textSize = ImGui::CalcTextSize(countText.c_str());
        ImVec2 textPos = ImVec2(
            slotPos.x + (slotSize.x - textSize.x) * 0.5f,
            slotPos.y + (slotSize.y - textSize.y) * 0.5f
        );
        ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), countText.c_str());

      }
      if (i < settings::N_INVENTORY_COLUMNS-1)
        ImGui::SetCursorScreenPos(ImVec2(slotPos.x + slotSize.x + ImGui::GetStyle().ItemSpacing.x, slotPos.y));
    }
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
  ImVec2 bottomSize = ImVec2(settings::SCREEN_WIDTH/4 , 50);
  float start_height = settings::SCREEN_HEIGHT/3;
  float offset = 50;
  // Set window to full screen
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT));
  
  // Set window flags to make it fixed and fullscreen
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
  ImGui::Begin("Fullscreen Window", nullptr, windowFlags);
  rlImGuiImageSize(&Textures::backgroundForest, settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT);
  // Add UI elements here
  ImGui::SetCursorPos(ImVec2(settings::SCREEN_WIDTH/2-settings::SCREEN_WIDTH/5, start_height-offset*4));
  rlImGuiImageSize(&Textures::logoImage, 486.0f*(3.0f/2.0f), 142.0f*(3.0f/2.0f));
  //ImGui::Text("Terraria", ImVec2(settings::SCREEN_WIDTH/3, SCREEN_HEIGHT/10));
  ImGui::SetWindowFontScale(2.0f);
  ImGui::SetCursorPos(ImVec2(settings::SCREEN_WIDTH/2 - settings::SCREEN_WIDTH/8, start_height+offset));
  if (ImGui::Button("Play", bottomSize)) 
    this->state = GameState::IN_GAME;
  ImGui::SetCursorPos(ImVec2(settings::SCREEN_WIDTH/2 - settings::SCREEN_WIDTH/8, start_height+offset*2.5));
  if (ImGui::Button("Options", bottomSize)) 
    this->state = GameState::OPTIONS;
  ImGui::SetCursorPos(ImVec2(settings::SCREEN_WIDTH/2 - settings::SCREEN_WIDTH/8, start_height+offset*4));
  if (ImGui::Button("Exit", bottomSize))
    this->is_running = false;
  ImGui::End();
  ImGui::PopStyleColor(4); // Pop all three colors we pushed
}


void Game::renderOptionsUI(){
  // Settings
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); // Green button
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f)); // Green hover effect
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Green active effect
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // White text

  ImVec2 bottomSize = ImVec2(settings::SCREEN_WIDTH/4 , 50);
  float start_height = settings::SCREEN_HEIGHT/3;
  float offset = 50;
  // Set window to full screen
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT));
  
  // Set window flags to make it fixed and fullscreen
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
  ImGui::Begin("Fullscreen Window", nullptr, windowFlags);
  rlImGuiImageSize(&Textures::backgroundForest, settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT);
  // Add UI elements here
  ImGui::SetCursorPos(ImVec2(settings::SCREEN_WIDTH/2-settings::SCREEN_WIDTH/5, start_height-offset*4));
  rlImGuiImageSize(&Textures::logoImageOptions , 486.0f*(3.0f/2.0f), 142.0f*(3.0f/2.0f));
  //ImGui::Text("Terraria", ImVec2(settings::SCREEN_WIDTH/3, SCREEN_HEIGHT/10));

  static int block_size = settings::BLOCK_SIZE_X;
  static int screen_width_x = settings::SCREEN_WIDTH;
  static int screen_width_y = settings::SCREEN_HEIGHT;
  auto make_slider = [start_height, offset](std::string name, int &var, float start, float end, char n_prev){
    ImGui::SetNextItemWidth(settings::SCREEN_WIDTH/4); // Set slider width to 200 pixels
    ImGui::SetCursorPos(ImVec2(settings::SCREEN_WIDTH/2-settings::SCREEN_WIDTH/8 , start_height+offset*(1 + 1.5f*(float)(n_prev-48))));
    std::string slider_label = name + "##" + std::to_string(n_prev); // Unique ID using ## separator
    ImGui::SliderInt(slider_label.c_str(), &var, start, end);
  };

  ImGui::SetWindowFontScale(2.0f);
  make_slider((std::string)": Block size", block_size, 10.0f, 30.0f, '0');
  make_slider((std::string)": Screen width", screen_width_x, 100, 1920, '1');
  make_slider((std::string)": Screen height", screen_width_y, 400, 1080, '2');
  ImGui::SetCursorPos(ImVec2(settings::SCREEN_WIDTH/2-settings::SCREEN_WIDTH/8 , start_height+offset*(1 + 1.5f*3.0f)));
  if (ImGui::Button("Apply", bottomSize)){
    if (screen_width_x != settings::SCREEN_WIDTH || screen_width_y != settings::SCREEN_HEIGHT)
      SetWindowSize(screen_width_x, screen_width_y);
    settings::setWindowSettings(screen_width_x, screen_width_y);
    settings::setBlockSettings(block_size, block_size);
  }
  ImGui::SetCursorPos(ImVec2(settings::SCREEN_WIDTH/2 - settings::SCREEN_WIDTH/8 , start_height+offset*7.0f));
  if (ImGui::Button("Exit", bottomSize)){
    this->state = GameState::HOME;
  }

    

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
                   {(float)i, (float)j, (float)settings::BLOCK_SIZE_X, (float)settings::BLOCK_SIZE_Y}, {0,0}, 0, 
                   lightHandler.getLightValue((i-starting_point.x)/settings::BLOCK_SIZE_X, (j-starting_point.y)/settings::BLOCK_SIZE_Y, type));
  }else if (type != 0 && type < TREE_BRANCH){
    tileRect = TileRenderUtil::getTileP(TileRenderUtil::ambientBlock(map, xTile, yTile, type, this->noise), type);
    tileRect.x += Textures::tileAtlas[type]; // Add the atlas position
    DrawTexturePro(Textures::all_atlas, 
                   tileRect,
                   {(float)i, (float)j, (float)settings::BLOCK_SIZE_X*1.2f, (float)settings::BLOCK_SIZE_Y*1.2f}, {0,0}, 0, 
                   lightHandler.getLightValue((i-starting_point.x)/settings::BLOCK_SIZE_X, (j-starting_point.y)/settings::BLOCK_SIZE_Y, type));
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
                   {(float)i, (float)j, (float)settings::BLOCK_SIZE_X*1.2f, (float)settings::BLOCK_SIZE_Y*1.2f+1}, {0,0}, 0, type == WALL_DIRT ? GRAY : WHITE);

  }else if (type != 0){
    DrawTexturePro(Textures::all_atlas, 
                   {(float)Textures::topTreeAtlas, 0, 82, 82},
                   {(float)i- settings::BLOCK_SIZE_X*2.3f, (float)j-(float)settings::BLOCK_SIZE_Y*1.9f, 
                   (float)settings::BLOCK_SIZE_X*6, (float)settings::BLOCK_SIZE_Y*5}, {0,0}, 0, WHITE);

  }

}

bool Game::getIsRunning(){
  return this->is_running;
}

Game::~Game(){
  std::cout << "I'm killing you" << std::endl;
  this->lightHandler.kill();
}


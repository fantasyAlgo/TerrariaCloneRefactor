#include "include/Game.hpp"
#include "include/Settings.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"


int main(void){
  settings::init_settings();
  InitWindow(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT, "my new terraria clone");
  SetExitKey(0);
  rlImGuiSetup(true);
  Game game;
  float deltaTime;
  bool showDemoWindow = true;
  float sliderValue = 0;
  //SetTargetFPS(60);
  while (game.getIsRunning()){ 
    deltaTime = GetFrameTime();
    game.inputHandler(deltaTime);
    game.update(deltaTime);
    BeginDrawing();
      ClearBackground({126, 197, 222, 255});
      game.render();
      DrawFPS(settings::SCREEN_WIDTH-50, 10);
      rlImGuiBegin();
        game.renderUI();
      rlImGuiEnd();
    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();        // Close window and OpenGL context

  return 0;
}

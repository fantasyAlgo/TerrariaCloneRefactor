#include "include/Game.hpp"
#include "include/Settings.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"


int main(void){
  //init_settings();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "my new terraria clone");
  rlImGuiSetup(true);
  Game game;
  float deltaTime;
  bool showDemoWindow = true;
  float sliderValue = 0;
  //SetTargetFPS(60);
  while (game.getIsRunning() && !WindowShouldClose()){ 
    deltaTime = GetFrameTime();
    game.inputHandler(deltaTime);
    game.update(deltaTime);
    BeginDrawing();
      ClearBackground({126, 197, 222, 255});
      game.render();
      DrawFPS(SCREEN_WIDTH-50, 10);
      rlImGuiBegin();
        game.renderUI();
      rlImGuiEnd();
    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();        // Close window and OpenGL context

  return 0;
}

#include "include/Game.hpp"
#include "include/Settings.hpp"
#include "raylib.h"


int main(void){
  //init_settings();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "my new terraria clone");
  Game game;
  float deltaTime;
  //SetTargetFPS(60);
  while (!WindowShouldClose()){ 
    deltaTime = GetFrameTime();
    game.inputHandler(deltaTime);
    game.update(deltaTime);
    BeginDrawing();
      ClearBackground({126, 197, 222, 255});
      game.render();
      DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();        // Close window and OpenGL context

  return 0;
}

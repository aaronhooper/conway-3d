#include "raylib.h"

typedef enum Cell {
  CELL_DEAD = 0,
  CELL_ALIVE,
} Cell;

#define CELL_COLOR_DEAD CLITERAL(Color){0, 0, 0, 0}
#define CELL_COLOR_ALIVE CLITERAL(Color){255, 255, 255, 255}

#define WORLD_WIDTH 100
#define WORLD_HEIGHT 100
#define SIDE_LENGTH 5.0f

typedef struct World {
  Cell grid[WORLD_HEIGHT][WORLD_WIDTH];
} World;

int main(void) {
  InitWindow(800, 600, "conway's gmae of life");
  Camera3D camera = {0};
  camera.position = (Vector3){10.0f, 400.0f, 10.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 0.0f, 1.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  World world = {0};
  for (int i = 0; i < WORLD_HEIGHT; i++) {
    for (int j = 0; j < WORLD_HEIGHT; j++) {
      world.grid[i][j] = GetRandomValue(0, 1);
    }
  }

  DisableCursor();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    UpdateCamera(&camera, CAMERA_FREE);
    BeginDrawing();
    BeginMode3D(camera);

    ClearBackground(BLACK);

    for (int i = 0; i < WORLD_HEIGHT; i++) {
      for (int j = 0; j < WORLD_HEIGHT; j++) {
        Vector3 position = {
            .x = j * SIDE_LENGTH,
            .y = 0.0f,
            .z = i * SIDE_LENGTH,
        };

        Vector3 size = {
            .x = SIDE_LENGTH,
            .y = SIDE_LENGTH,
            .z = SIDE_LENGTH,
        };

        if (world.grid[i][j]) {
          DrawCubeWiresV(position, size, CELL_COLOR_ALIVE);
        }
      }
    }

    EndMode3D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

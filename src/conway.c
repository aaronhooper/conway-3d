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

typedef Cell Buffer[WORLD_HEIGHT][WORLD_WIDTH];

typedef struct World {
  short current;
  Buffer a;
  Buffer b;
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
  world.current = 0;
  for (int i = 0; i < WORLD_HEIGHT; i++) {
    for (int j = 0; j < WORLD_HEIGHT; j++) {
      world.a[i][j] = GetRandomValue(0, 1);
    }
  }

  DisableCursor();
  SetTargetFPS(60);

  float acc = 0.0f;
  float threshold = 1 / 10.0f;
  bool enable_update = true;

  while (!WindowShouldClose()) {
    acc += GetFrameTime();

    if (acc > threshold) {
      acc -= threshold;
      enable_update = true;
    } else {
      enable_update = false;
    }

    if (enable_update) {
      // update world
      for (int i = 0; i < WORLD_HEIGHT; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
          int nalive = 0;
          Buffer *prevbuffer = world.current == 0 ? &world.a : &world.b;

          // get num of cell neighbors that are alive
          for (int z = -1; z <= 1; z++) {
            for (int w = -1; w <= 1; w++) {
              if (z == 0 && w == 0) {
                continue;
              }

              if (i + z < 0 || i + z >= WORLD_HEIGHT) {
                continue;
              }

              if (j + w < 0 || j + w >= WORLD_WIDTH) {
                continue;
              }

              // if neighbor is alive, increment the count
              if ((*prevbuffer)[i + z][j + w] == CELL_ALIVE) {
                nalive++;
              }
            }
          }

          /**
           * This one here fucked me up.
           * It turns out that this:
           *
           * `*buffer[i][j]`
           *
           * means something completely different to this:
           *
           * `(*buffer)[i][j]`
           */
          int cellstate = (*prevbuffer)[i][j];

          if (cellstate == CELL_ALIVE && nalive < 2) {
            cellstate = CELL_DEAD;
          } else if (cellstate == CELL_ALIVE && 2 <= nalive && nalive <= 3) {

          } else if (cellstate == CELL_ALIVE && nalive > 3) {
            cellstate = CELL_DEAD;
          } else if (cellstate == CELL_DEAD && nalive == 3) {
            cellstate = CELL_ALIVE;
          }

          Buffer *nextbuffer = world.current == 0 ? &world.b : &world.a;
          (*nextbuffer)[i][j] = cellstate;
        }
      }

      // update done, flip buffer
      world.current ^= 1;
    }

    UpdateCamera(&camera, CAMERA_FREE);
    BeginDrawing();
    BeginMode3D(camera);

    ClearBackground(BLACK);

    Buffer *buffer = world.current == 0 ? &world.a : &world.b;

    // draw world
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

        // draw cell from current buffer if alive
        if ((*buffer)[i][j] == CELL_ALIVE) {
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

#include "raylib.h"

typedef enum Cell {
  CELL_DEAD = 0,
  CELL_ALIVE,
} Cell;

#define CELL_COLOR_ALIVE YELLOW
#define BACKGROUND_COLOR BLACK

#define WORLD_WIDTH 200
#define WORLD_HEIGHT 200
#define SIDE_LENGTH 1.0f

typedef Cell Buffer[WORLD_HEIGHT][WORLD_WIDTH];

typedef struct World {
  short current;
  Buffer a;
  Buffer b;
} World;

#define GAME_FRAMERATE 120
#define CONWAY_FRAMERATE 10

#define PLAYER_MOVEMENT_SPEED 100.0f
#define MOUSE_MOVEMENT_SPEED 5.0f

int main(void) {
  InitWindow(800, 600, "conway's gmae of life");
  Camera3D camera = {0};
  camera.position = (Vector3){1.0f, 1.0f, -100.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
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
  SetTargetFPS(GAME_FRAMERATE);

  float acc = 0.0f;
  float threshold = 1.0f / CONWAY_FRAMERATE;
  bool update_enabled = true;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    acc += dt;

    if (acc > threshold) {
      acc -= threshold;
      update_enabled = true;
    } else {
      update_enabled = false;
    }

    // only update cell buffer when we pass the threshold
    if (update_enabled) {
      for (int i = 0; i < WORLD_HEIGHT; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
          Buffer *prevbuffer = world.current == 0 ? &world.a : &world.b;
          int nalive = 0;

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

          // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules
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

    // handle camera movement
    Vector3 movement = {0};
    if (IsKeyDown(KEY_W)) {
      movement.x += PLAYER_MOVEMENT_SPEED * dt;
    }
    if (IsKeyDown(KEY_S)) {
      movement.x -= PLAYER_MOVEMENT_SPEED * dt;
    }
    if (IsKeyDown(KEY_A)) {
      movement.y -= PLAYER_MOVEMENT_SPEED * dt;
    }
    if (IsKeyDown(KEY_D)) {
      movement.y += PLAYER_MOVEMENT_SPEED * dt;
    }
    if (IsKeyDown(KEY_SPACE)) {
      movement.z += PLAYER_MOVEMENT_SPEED * dt;
    }
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
      movement.z -= PLAYER_MOVEMENT_SPEED * dt;
    }

    // handle mouse movement
    Vector3 rotation = {0};
    Vector2 delta = GetMouseDelta();
    if (delta.x != 0 || delta.y != 0) {
      // not handling side (z) rotation
      rotation.x += delta.x * MOUSE_MOVEMENT_SPEED * dt;
      rotation.y += delta.y * MOUSE_MOVEMENT_SPEED * dt;
    }

    UpdateCameraPro(&camera, movement, rotation, 0);

    BeginDrawing();
    BeginMode3D(camera);

    ClearBackground(BACKGROUND_COLOR);

    Buffer *buffer = world.current == 0 ? &world.a : &world.b;

    // draw world
    for (int i = 0; i < WORLD_HEIGHT; i++) {
      for (int j = 0; j < WORLD_HEIGHT; j++) {
        Vector3 position = {
            .x = j * SIDE_LENGTH,
            .y = i * SIDE_LENGTH,
            .z = 0.0f,
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

    DrawGrid(50, 10);

    EndMode3D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

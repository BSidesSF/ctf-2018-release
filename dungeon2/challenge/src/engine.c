#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

#include "engine.h"
#include "player.h"
#include "map.h"
#include "raycaster.h"

const struct timespec FRAME_DELAY = {0, 90000000L};

ENGINE * createEngine(
  int rows,
  int cols,
  char *mapStr,
  int mapSize,
  int playerStartR,
  int playerStartC,
  int interfaceHeight,
  INTERACTION_FUNCTION interactionFn,
  USE_ITEM_FUNCTION useItemFn) {
    // setup screen
    initscr();
    start_color();
    initColors();
    cbreak();
    noecho();
    curs_set(FALSE);

    WINDOW * parentWindow = newwin(rows, cols, 0, 0);
    WINDOW * graphicsWindow = subwin(parentWindow, rows - interfaceHeight, cols, 0, 0);
    WINDOW * textWindow = subwin(parentWindow, interfaceHeight, cols, rows - interfaceHeight, 0);

    //resizeterm(WIDTH, HEIGHT);

    struct Player * player = (struct Player *) calloc(1, sizeof(struct Player));
    player->x = playerStartR;
    player->y = playerStartC;
    player->direction = 0;
    player->cameraPlaneX = 0;
    player->cameraPlaneY = .66; // Seems to be about the optimal view

    struct Interface * interface = (struct Interface *) calloc(1, sizeof(struct Interface));

    struct Map *map =  calloc(1, sizeof(struct Map));
    map->map = mapStr;
    map->size = mapSize;

    ENGINE * engine = (ENGINE * ) malloc(sizeof(ENGINE));
    engine->rows = rows;
    engine->cols = cols;
    engine->player = player;
    engine->map = map;
    engine->interface = interface;
    engine->interactionFn = interactionFn;
    engine->useItemFn = useItemFn;
    interface->window = textWindow;
    interface->height = interfaceHeight;
    engine->parentWindow = parentWindow;
    engine->mainWindow = graphicsWindow;
    engine->renderMode = RENDER_COLOR;
    engine->moveMode = MOVE_MODE_REGULAR;
    engine->gameState = STATE_PLAYING;

    return engine;
  }

void renderFrame(ENGINE *engine){
  // werase(engine->mainWindow);
  wclear(engine->mainWindow);
  raycast(engine->player, engine->map, engine->mainWindow, engine->cols, engine->rows, engine->renderMode);
  box(engine->mainWindow, 0, 0);
  wrefresh(engine->mainWindow);
}

bool canMove(ENGINE *engine, short direction){
  double distance = 1.0;
  double finalX = engine->player-> x + cos(engine->player->direction) * distance * direction;
  double finalY = engine->player-> y + sin(engine->player->direction) * distance * direction;
  char nextPosition = getPositionInMap(engine->map, (int) round(finalY), (int) round(finalX));
  if ( !(nextPosition == MAP_OPEN_SPACE || nextPosition > MAP_MARKER_MIN))
    return false;
  return true;
}

void walkAnimation(ENGINE *engine, short direction){
  double moveSpeed = .1;
  double distance = 1.0;
  double finalX = engine->player-> x + cos(engine->player->direction) * distance * direction;
  double finalY = engine->player-> y + sin(engine->player->direction) * distance * direction;
  if (engine->moveMode == MOVE_MODE_REGULAR) {
    for (double i = 0; i <(int) floor(distance/moveSpeed); i+=1){
      walk(engine->player, moveSpeed, direction);
      renderFrame(engine);
      nanosleep(&FRAME_DELAY, NULL);
    }
  }
  engine->player->x = finalX;
  engine->player->y = finalY;
}

void rotationAnimation(ENGINE *engine, double radians, int direction){
  double moveSpeed = 0.15;
  double finalDirection = engine->player->direction + (radians * direction);
  //make sure to keep the camera plane up to date. It needs to be perpendicular to the direction
  double finalCameraX = engine->player->cameraPlaneX * cos(radians * direction) - engine->player->cameraPlaneY * sin(radians * direction);
  double finalCameraY = engine->player->cameraPlaneX * sin(radians * direction) +  engine->player->cameraPlaneY * cos(radians * direction);

  if (engine->moveMode == MOVE_MODE_REGULAR) {
    for (double i=0; i < (int) floor(fabs(radians/moveSpeed)); i+=1){
      rotate(engine->player, moveSpeed, direction);
      renderFrame(engine);
      nanosleep(&FRAME_DELAY, NULL);
    }
  }
  engine->player->direction = finalDirection;
  engine->player->cameraPlaneX = finalCameraX;
  engine->player->cameraPlaneY = finalCameraY;
}

void deathAnimation(ENGINE *engine) {
  wclear(engine->mainWindow);
  death(engine->mainWindow, engine->rows, engine->cols, engine->renderMode, "You have died. Game Over");
  wrefresh(engine->mainWindow);
}

void gameLoop(ENGINE *engine){
  // this is required to prevent a 1 frame delay. No idea why
  nodelay(stdscr, TRUE);
  getch();
  nodelay(stdscr, FALSE);
  updateInterface(engine->interface);
  renderFrame(engine);

  while (engine->gameState != STATE_END){
    if (engine->gameState == STATE_DEAD) {
      deathAnimation(engine);
      getch();
      engine->gameState = STATE_END;
      break;
    }
    char input = getch();
    switch(input) {
      case 'w':
      if (canMove(engine, PLAYER_FORWARDS))
        walkAnimation(engine, PLAYER_FORWARDS);
      engine->interactionFn(engine, INTERACTION_TYPE_WALK_FORWARD);
      break;
      case 's':
      if (canMove(engine, PLAYER_BACKWARDS))
        walkAnimation(engine, PLAYER_BACKWARDS);
      engine->interactionFn(engine, INTERACTION_TYPE_WALK_BACK);
      break;
      case 'a':
      rotationAnimation(engine, M_PI/2.0, PLAYER_COUNTER_CLOCKWISE);
      engine->interactionFn(engine, INTERACTION_TYPE_TURN_LEFT);
      break;
      case 'd':
      rotationAnimation(engine, M_PI/2.0, PLAYER_CLOCKWISE);
      engine->interactionFn(engine, INTERACTION_TYPE_TURN_RIGHT);;
      break;
      case 'l':
      engine->interactionFn(engine, INTERACTION_TYPE_LOOK);
      break;
      case 't':
      engine->interactionFn(engine, INTERACTION_TYPE_TALK);
      break;
      case '1':
      if (engine->interface->inventory[0] != NULL)
        engine->useItemFn(engine, engine->interface->inventory[0], 0);
      break;
      case '2':
      if (engine->interface->inventory[1] != NULL)
        engine->useItemFn(engine, engine->interface->inventory[1], 1);
      break;
      case '?':
      if (engine->renderMode == RENDER_COLOR)
      engine->renderMode = RENDER_CHAR;
      else
      engine->renderMode = RENDER_COLOR;
      break;
      case '!':
      if (engine->moveMode == MOVE_MODE_REGULAR)
      engine->moveMode = MOVE_MODE_INSTANT;
      else
      engine->moveMode = MOVE_MODE_REGULAR;
      break;
      default:
      break;
    }
    renderFrame(engine);
    updateInterface(engine->interface);
  }
}

void shutdown(ENGINE *engine){
  delwin(engine->interface->window);
  delwin(engine->mainWindow);
  delwin(engine->parentWindow);
  free(engine->player);
  free(engine->interface);
  free(engine->map);
  free(engine);
  endwin();
}

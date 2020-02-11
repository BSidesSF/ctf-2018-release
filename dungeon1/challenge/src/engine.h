#ifndef ENGINE_H
#define ENGINE_H

typedef unsigned short GAME_STATE;
#define STATE_PLAYING 1
#define STATE_DEAD 2
#define STATE_END 3

typedef unsigned short INTERACTION;

#define INTERACTION_TYPE_WALK_FORWARD 0
#define INTERACTION_TYPE_WALK_BACK 1
#define INTERACTION_TYPE_TURN_LEFT 2
#define INTERACTION_TYPE_TURN_RIGHT 3
#define INTERACTION_TYPE_LOOK 4
#define INTERACTION_TYPE_TALK 5
#define INTERACTION_TYPE_ITEM 6

#include <ncurses.h>
#include <time.h>

#include "player.h"
#include "map.h"
#include "raycaster.h"
#include "interface.h"

extern const struct timespec FRAME_DELAY;

typedef int MOVE_MODE;
#define MOVE_MODE_REGULAR 0
#define MOVE_MODE_INSTANT 1

typedef struct Engine {
  int rows;
  int cols;
  WINDOW *parentWindow;
  WINDOW *mainWindow;
  MOVE_MODE moveMode;
  RENDER_MODE renderMode;
  GAME_STATE gameState;
  struct Player *player;
  struct Map *map;
  struct Interface *interface;
  void (*interactionFn)(struct Engine *, INTERACTION);
  void (*useItemFn)(struct Engine *, char *, int);
} ENGINE;

typedef void (*INTERACTION_FUNCTION)(ENGINE *, INTERACTION);
typedef void (*USE_ITEM_FUNCTION)(ENGINE *, char *item, int itemIndex);

// create a new game engine
ENGINE * createEngine(int rows, int cols,
  char *mapStr,
  int mapSize, int playerStartR, int playerStartC,
  int interfaceHeight,
  INTERACTION_FUNCTION interactionFn,
  USE_ITEM_FUNCTION useItemFn
);

// run the game loop
void gameLoop(ENGINE *engine);

// movement
void rotationAnimation(ENGINE *engine, double radians, int direction);
void walkAnimation(ENGINE *engine, short direction);

// shutdown the game
void shutdown(ENGINE *engine);
#endif

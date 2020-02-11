
#ifndef RAYCAST_H_   /* Include guard */
#define RAYCAST_H_

#include <ncurses.h>

#include "player.h"
#include "map.h"

// Color definitions
#define WALL_RED 1
#define WALL_GREEN 2
#define WALL_YELLOW 3
#define WALL_CYAN 4
#define FLOOR 5
#define BLACK 6
#define END_TEXT 7

// shorthand for color pairs
#define ATTR_ALL_BLACK COLOR_PAIR(BLACK)
#define ATTR_WALL_RED COLOR_PAIR(WALL_RED)
#define ATTR_WALL_GREEN COLOR_PAIR(WALL_GREEN)
#define ATTR_WALL_YELLOW COLOR_PAIR(WALL_YELLOW)
#define ATTR_WALL_CYAN COLOR_PAIR(WALL_CYAN)
#define ATTR_FLOOR COLOR_PAIR(FLOOR)
#define ATTR_END_TEXT COLOR_PAIR(END_TEXT)

// Render modes
#define RENDER_COLOR 0
#define RENDER_CHAR 1

typedef int RENDER_MODE;

void initColors();
void raycast (struct Player *player, struct Map *map, WINDOW *window, int width, int height, RENDER_MODE mode);
void death(WINDOW *window, int rows, int cols, RENDER_MODE mode, char *message);

#endif

#include <string.h>
#include <ncurses.h>
#include <math.h>

#include "player.h"
#include "raycaster.h"
#include "map.h"

void initColors() {
  init_pair(WALL_RED, COLOR_RED, COLOR_RED);
  init_pair(WALL_GREEN, COLOR_GREEN, COLOR_GREEN);
  init_pair(WALL_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(WALL_CYAN, COLOR_CYAN, COLOR_CYAN);
  init_pair(FLOOR, COLOR_BLUE, COLOR_BLACK);
  init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
  init_pair(END_TEXT, COLOR_WHITE, COLOR_RED);
}

int getWallColor(char character){
  if (character == '*')
  return ATTR_WALL_RED;
  if (character == 'X')
  return ATTR_WALL_GREEN;
  if (character == '-')
  return ATTR_WALL_YELLOW;
  if (character == '&')
  return ATTR_WALL_CYAN;
  if (character  == 'E')
  return ATTR_ALL_BLACK;
  return ATTR_WALL_YELLOW;
}

void death(WINDOW *window, int rows, int cols, RENDER_MODE mode, char* message) {
  int ypos = rows/2;
  int xpos = cols/2 - strlen(message)/2;
  if (mode == RENDER_COLOR) {
    wbkgd(window , COLOR_PAIR(1));
    wattron(window, ATTR_END_TEXT);
    mvwprintw(window, ypos, xpos, message);
    wattroff(window, ATTR_END_TEXT);
  } else {
    mvwprintw(window, ypos, xpos, message);
  }
}

void drawColumn(WINDOW *window, int column, double colHeight, int screenHeight, char character, RENDER_MODE mode){
  int color = getWallColor(character);
  int colTop = (int) round(screenHeight/2.0 - colHeight/2.0);
  int colBottom = (int) round(screenHeight/2.0 + colHeight/2.0);
  if (mode == RENDER_COLOR)
  wattron(window, color);
  for (int i = colTop; i < screenHeight; i++){
    if (i == colBottom) {
      if (mode == RENDER_COLOR)
      wattroff(window, color);
      color = ATTR_FLOOR;
      character = '.';
      if (mode == RENDER_COLOR)
      wattron(window, color);
    }
    mvwaddch(window, i, column, character);
  }
  if (mode == RENDER_COLOR)
  wattroff(window, color);
}

void raycast(struct Player *player, struct Map *map, WINDOW *window, int width, int height, RENDER_MODE mode){
  for (int x = 0; x < width; x++){
    //calculate ray position and direction
    double cameraX = 2 * x / ((double) (width)) - 1; //x-coordinate in camera space
    // cast rays from the middle of the cell
    double rayPosX = player->x + .5;
    double rayPosY = player->y + .5;
    double rayDirX = cos(player->direction) + player->cameraPlaneX * cameraX;
    double rayDirY = sin(player->direction) + player->cameraPlaneY * cameraX;

    int mapX = (int) rayPosX;
    int mapY = (int) rayPosY;

    //length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

    //length of ray from one x or y-side to next x or y-side
    double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
    double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
    double perpWallDist;

    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    char wall = ' '; // the character in the map to draw the wall
    int hit = false; //was there a wall hit?
    bool outside = false; // are we outside the map?
    int side; //was a NS or a EW wall hit?

    //calculate step and initial sideDist
    if (rayDirX < 0) {
      stepX = -1;
      sideDistX = (rayPosX - mapX) * deltaDistX;
    }
    else {
      stepX = 1;
      sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
    }
    if (rayDirY < 0) {
      stepY = -1;
      sideDistY = (rayPosY - mapY) * deltaDistY;
    }
    else {
      stepY = 1;
      sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
    }
    //perform DDA
    while (!hit && !outside) {
      //jump to next map square, OR in x-direction, OR in y-direction
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      }
      else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      if (mapX < 0 || mapY < 0 || mapY > map->size || mapY > map->size) {
        outside = true;
      }
      //Check if ray has hit a wall
      wall = getPositionInMap(map, mapY, mapX);
      if (wall != MAP_OPEN_SPACE && wall < MAP_MARKER_MIN)
      hit = true;
    }

    //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
    if (hit) {
      if (side == 0) perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
      else           perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

      //Calculate height of line to draw on screen
      int lineHeight = (int)(height / perpWallDist);
      drawColumn(window, x, lineHeight, height, wall, mode);
    }
  }
}

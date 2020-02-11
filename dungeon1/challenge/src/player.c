#include <math.h>
#include "player.h"

void walk(struct Player *player, double distance, int direction){
  player->x += cos(player->direction) * direction * distance;
  player->y += sin(player->direction) * direction * distance;
}

void rotate(struct Player *player, double radians, int direction){
  double deltaDir = radians * direction;
  player->direction += deltaDir;
  double oldPlaneX = player->cameraPlaneX;
  player->cameraPlaneX = player->cameraPlaneX * cos(deltaDir) - player->cameraPlaneY * sin(deltaDir);
  player->cameraPlaneY = oldPlaneX * sin(deltaDir) + player->cameraPlaneY * cos(deltaDir);
}

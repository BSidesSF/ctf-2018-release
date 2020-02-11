#ifndef PLAYER_H_   /* Include guard */
#define PLAYER_H_

#define PLAYER_FORWARDS 1
#define PLAYER_BACKWARDS -1
#define PLAYER_CLOCKWISE 1
#define PLAYER_COUNTER_CLOCKWISE -1

struct Player {
  double x;
  double y;
  double direction;
  double cameraPlaneX;
  double cameraPlaneY;
};

void walk(struct Player *player, double distance, int direction);
void rotate(struct Player *player, double radians, int direction);

#endif

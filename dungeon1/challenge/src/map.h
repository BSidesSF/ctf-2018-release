#ifndef MAP_H_/* Include guard */
#define MAP_H_

#define MAP_OPEN_SPACE ' '

// These map markers are used for player interactions, and are
// not walls. Everything above this is a map marker
#define MAP_MARKER_MIN 96

struct Map {
  int size;
  char *map;
};

char getPositionInMap(struct Map *map, int y, int x);
void setPositionInMap(struct Map *map, int y, int x, char newChar);


#endif

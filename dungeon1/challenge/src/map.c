#include "map.h"

char getPositionInMap(struct Map *map, int y, int x) {
  return map->map[map->size * y + x];
}

void setPositionInMap(struct Map *map, int y, int x, char newChar){
  if (y > map->size || y < 0 || x < 0 || x > map->size) {
    return;
  }
  map->map[map->size * y + x] = newChar;
}

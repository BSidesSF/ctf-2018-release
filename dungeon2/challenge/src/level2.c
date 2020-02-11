#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "engine.h"

#define ROWS 40
#define COLS 100
#define MIN_SWITCH 'a'
#define MAX_SWITCH 'j'

const int mapSize = 20;
const char map2[] = ""\
"**********E*********"\
"*      XX zXX      *"\
"*       XXXX       *"\
"* ----   rr    ----*"\
"*       &   &      *"\
"** s******m******  *"\
"*    ---ssass-X    *"\
"*    --XsbcdsXX    *"\
"*    ---sefgs-X    *"\
"*    --XshijsXX    *"\
"*ssss---sssss-X    *"\
"* *******X*X*X**** *"\
"* ----   *   ----- *"\
"*      --*--       *"\
"* ----   *   ----- *"\
"* X&X&   *   X&X&X *"\
"*      X&*&X       *"\
"*&X&X&       X&X&X&*"\
"*&&&&&&&&&&&&&&&&&&&";

char * flag = NULL;
char * map;
ENGINE * engine;
unsigned char switches[MAX_SWITCH - MIN_SWITCH + 1 ];

char * readFlag() {
  FILE * pFile;
  long lSize;
  size_t result;

  pFile = fopen("/flag", "rb" );
  fseek(pFile , 0 , SEEK_END);
  lSize = ftell(pFile);
  char * f = (char *) malloc(sizeof(char) * lSize + 1);
  rewind (pFile);
  result = fread(f, 1, lSize, pFile);
  f[lSize] = '\0';
  fclose (pFile);
  return f;
}

bool unlock() {
  unsigned char w = switches[0];
  if (w == 0)
  return false;
  for (int i=1; i < sizeof(switches); i+=1){
    w ^= (w << 5);
    w ^= (w >> 3);
    w ^= (w << 7);
    if (switches[i] != w % 9)
    return false;
  }
  return true;
}

void checkInteraction(ENGINE *engine, INTERACTION interactionType) {
  clearMessage(engine->interface);
  char marker = getPositionInMap(engine->map, engine->player->y, engine->player->x);
  switch (interactionType){
    case INTERACTION_TYPE_WALK_FORWARD:
    case INTERACTION_TYPE_WALK_BACK:
    if (marker >= MIN_SWITCH && marker <= MAX_SWITCH){
      if (marker == 'b')
      addMessage(engine->interface, "As you step forward, you hear a mechanical click.");
      else if (marker%5 == 0)
      addMessage(engine->interface, "* Click *");
      else if (marker%5 == 1)
      addMessage(engine->interface, "Another click");
      else if (marker%5 == 2)
      addMessage(engine->interface, "Something on the floor clicks");
      else if (marker%5 == 3)
      addMessage(engine->interface, "You stepped on another panel on the floor");
      else
      addMessage(engine->interface, "Yet another mechanical click from the floor.");

      if (switches[marker - MIN_SWITCH] < 100)
      switches[marker - MIN_SWITCH] += 1;
      if (unlock()) {
        addMessage(engine->interface, "You hear the sound of gears grinding, then a loud thud.");
        setPositionInMap(engine->map,  2, 9, MAP_OPEN_SPACE);
        setPositionInMap(engine->map,  2, 10, MAP_OPEN_SPACE);
      }
    }
    else if (marker == 's') {
      addMessage(engine->interface, "Suddenly giant spikes emerge from the floor");
      addMessage(engine->interface, "It looks like this is the end for you...");
      engine->gameState = STATE_DEAD;
    }
    else if (marker == 'm') {
      addMessage(engine->interface, "There is another sign on the wall. \"Beware...\" it starts");
    }
    else if (marker == 't') {
      addMessage(engine->interface, "You feel a draft coming from the wall");
    }
    else if (marker == 'z') {
      addMessage(engine->interface, "You've found the stairs leading deeping into the dungeon");
      flag = readFlag();
      addMessage(engine->interface, flag);
    }
    break;
    case INTERACTION_TYPE_LOOK:
    if (marker >= MIN_SWITCH && marker <= MAX_SWITCH){
      addMessage(engine->interface, "You look down to the ground");
      addMessage(engine->interface, "You appear to be standing on a large metal panel");
    }
    else if (marker == 't') {
      addMessage(engine->interface, "You inspect the wall.");
      addMessage(engine->interface, "There appears to be a small opening at the base of the wall.");
      addMessage(engine->interface, "Certainly not enough room for you to make it through though.");
    }
    else if (marker == 'm') {
      addMessage(engine->interface, "Beware! A nefarious challenge awaits you");
      addMessage(engine->interface, "Only the cunning and clever will make it out alive");
      addMessage(engine->interface, "Solve the puzzle, and avoid the traps.");
    }
    else
    addMessage(engine->interface, "You are surrounded by rock and darkness. Nothing interesting here.");
    break;
    case INTERACTION_TYPE_TALK:
    if (marker >= MIN_SWITCH && marker <= MAX_SWITCH){
      addMessage(engine->interface, "You ask the metal panel what it's deal is.");
      addMessage(engine->interface, "It doesn't respond.");
      addMessage(engine->interface, "You might have offended it.");
    }
    else
    addMessage(engine->interface, "Its so empty here. You wonder if anyone will hear your voice again.");
    break;
    default:
    break;
  }
}

void useItem(ENGINE *engine, char * item, int itemIndex) {
}

void clean(){
  if (flag != NULL)
    free(flag);
  free(map);
  shutdown(engine);
}

int main(){
  memset(switches, 0, sizeof switches);
  int s = strlen(map2) + 1;
  map = (char *) calloc(s, sizeof(char));
  strncpy(map, map2, s);
  engine = createEngine(ROWS, COLS, map, mapSize, 1, 1, 6, &checkInteraction, &useItem);
  atexit(clean);
  signal(SIGTERM, exit);
  gameLoop(engine);
}

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

#define MAP_SIZE 20
const char map1[] = ""\
"********************"\
"X  ab c dd&     *  X"\
"X*** **  d&     i  X"\
"Xee --*  ---ii--*  X"\
"Xee             *  X"\
"X------- XXXX  X*  X"\
"X     **pX X    *  X"\
"X  &  ** X&&&&&&&  X"\
"X  &              -X"\
"X  -----&& *****   X"\
"X        X     &X  X"\
"X---&  * XX&&&j&   X"\
"-   &      &       X"\
"-------------------X"\
"Xff    gggXX-**   -X"\
"X----------   *   -X"\
"X--    &  X-------*X"\
"X  &  &&    &    XXX"\
"X  &&--&&----------X"\
"XzX&&----   - -    X"\
"*E******************";

const unsigned char map2[] = "\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xc1\xb9\xb9\xf8\xfb\xb9\xfa\xb9\xfd\xfd\xbf\xb9\xb9\xb9\xb9\xb9\xb3\xb9\xb9\xc1\xc1\xb3\xb3\xb3\xb9\xb3\xb3\xb9\xb9\xfd\xbf\xb9\xb9\xb9\xb9\xb9\xf0\xb9\xb9\xc1\xc1\xfc\xfc\xb9\xb4\xb4\xb3\xb9\xb9\xb4\xb4\xb4\xf0\xf0\xb4\xb4\xb3\xb9\xb9\xc1\xc1\xfc\xfc\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb3\xb9\xb9\xc1\xc1\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb9\xc1\xc1\xc1\xc1\xb9\xb9\xc1\xb3\xb9\xb9\xc1\xc1\xb9\xb9\xb9\xb9\xb9\xb3\xb3\xe9\xc1\xb9\xc1\xb9\xb9\xb9\xb9\xb3\xb9\xb9\xc1\xc1\xb9\xb9\xbf\xb9\xb9\xb3\xb3\xb9\xc1\xbf\xbf\xbf\xbf\xbf\xbf\xbf\xb9\xb9\xc1\xc1\xb9\xb9\xbf\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb4\xc1\xc1\xb9\xb9\xb4\xb4\xb4\xdf\xdf\xbf\xbf\xb9\xb3\xb3\xb3\xb3\xb3\xb9\xb9\xb9\xc1\xc1\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xc1\xb9\xb9\xb9\xb9\xb9\xbf\xc1\xb9\xb9\xc1\xc1\xb4\xb4\xdf\xbf\xb9\xb9\xb3\xb9\xc1\xc1\xbf\xbf\xbf\xf3\xbf\xb9\xb9\xb9\xc1\xb4\xb9\xb9\xb9\xbf\xb9\xb9\xb9\xb9\xb9\xb9\xbf\xb9\xb9\xb9\xb9\xb9\xb9\xb9\xc1\xb4\xdf\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xdf\xb4\xb4\xc1\xc1\xff\xff\xb9\xb9\xb9\xb9\xfe\xfe\xfe\xc1\xc1\xdf\xb3\xb3\xb9\xb9\xb9\xdf\xc1\xc1\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xb4\xdf\xb9\xb9\xb9\xb3\xb9\xb9\xb9\xdf\xc1\xc1\xdf\xdf\xb9\xb9\xb9\xb9\xbf\xb9\xb9\xc1\xdf\xb4\xdf\xb4\xb4\xdf\xb4\xb3\xc1\xc1\xb9\xb9\xbf\xb9\xb9\xbf\xbf\xb9\xb9\xb9\xb9\xbf\xb9\xb9\xb9\xb9\xc1\xc1\xc1\xc1\xb9\xb9\xbf\xbf\xdf\xdf\xbf\xbf\xb4\xb4\xdf\xb4\xdf\xb4\xdf\xb4\xb4\xb4\xc1\xc1\xe3\xc1\xbf\xbf\xdf\xdf\xdf\xdf\xb9\xb9\xb9\xb4\xb9\xb4\xb9\xb9\xb9\xb9\xc1\xb3\xdc\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3\xb3";

char *map;
char *flag = NULL;
ENGINE * engine;

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

void checkInteraction(ENGINE *engine, INTERACTION interactionType){
  clearMessage(engine->interface);
  char marker = getPositionInMap(engine->map, engine->player->y, engine->player->x);
  switch (interactionType){
    case INTERACTION_TYPE_WALK_FORWARD:
    case INTERACTION_TYPE_WALK_BACK:
    if (marker == 'a'){
      addMessage(engine->interface, "Immediately after entering, you hear a thunderous boom");
      addMessage(engine->interface, "The wall ceiling behind you collapse into a pile of rubble");
      addMessage(engine->interface, "You barely avoid the falling rock, but you are now trapped");
    }
    else if (marker == 'b')
    addMessage(engine->interface, "The air here is thick and damp.");
    else if (marker == 'c')
    addMessage(engine->interface, "You feel a light breeze. This dungeon is large and cavernous.");
    else if (marker == 'd'){
      addMessage(engine->interface, "There is a skeleton lying on the ground. It has clearly been here for while.");
      addMessage(engine->interface, "The bones of its fingers are curled around a piece of parchment.");
      addMessage(engine->interface, "Press (L) to read the note.");
    }
    else if (marker == 'e'){
      addMessage(engine->interface, "There is a another skeleton on the ground.");
      addMessage(engine->interface, "There some paper lying on the ground next to it.");
    }
    else if (marker == 'f') {
      addMessage(engine->interface, "You feel a cold breeze.");
    }
    else if (marker == 'g') {
      addMessage(engine->interface, "This hallway is dark and constricting.");
      addMessage(engine->interface, "You suddenly worry that you might never make it out.");
    }
    else if (marker == 'g') {
      addMessage(engine->interface, "This hallway is dark and constricting.");
      addMessage(engine->interface, "You suddenly worry that you might never make it out.");
    }
    else if (marker == 'i') {
      addMessage(engine->interface, "You stare into room with three skeletons.");
      addMessage(engine->interface, "Likely an exploration party that never made it out.");
    }
    else if (marker == 'j') {
      addMessage(engine->interface, "These hallways meander aimlessly. You could easily get lost here.");
    }
    else if (marker == 'h') {
      addMessage(engine->interface, "You stare into room with three corpses.");
      addMessage(engine->interface, "Likely an exploration party that never made it out.");
    }
    else if (marker == 'p') {
      addMessage(engine->interface, "There's an object lying on the ground.");
    }
    else if (marker == 'z') {
      addMessage(engine->interface, "You've found the stairs leading deeping into the dungeon");
      flag = readFlag();
      addMessage(engine->interface, flag);
    }
    break;
    case INTERACTION_TYPE_LOOK:
    if (marker == 'd') {
      addMessage(engine->interface, "The note reads: I've been trapped in this maze for weeks.");
      addMessage(engine->interface, "This dungeon is a labyrenth. I've expored every passageway ten times,");
      addMessage(engine->interface, "but still can't find where I entered. I wish I had a map.");
    }
    else if (marker == 'e') {
      addMessage(engine->interface, "The note reads: I ran into a another group of explorers today.");
      addMessage(engine->interface, "They told me that some of the stone walls are more brittle than others");
      addMessage(engine->interface, "and could be broken. No help to me. I can't tell any of these apart.");
    }
    else if (marker == 'f') {
      addMessage(engine->interface, "You examine the walls for cracks, but find nothing. This rock seems solid");
    }
    else if (marker == 'i') {
      addMessage(engine->interface, "You examine the corpses.");
      addMessage(engine->interface, "You find nothing of value - they were probably already looted");
    }
    else if (marker == 'p'){
      addMessage(engine->interface, "It appears to be mining tool. You pick it up");
      addItem(engine->interface, "Feeble pickaxe");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    else
    addMessage(engine->interface, "You are surrounded by rock and darkness. Nothing interesting here.");
    break;
    case INTERACTION_TYPE_TALK:
    if (marker == 'd'){
      addMessage(engine->interface, "You ask the skeleton for directions, ");
      addMessage(engine->interface, "but are unsurprised when it does not respond.");
      addMessage(engine->interface, "You ask yourself why you tried to ask a corpse for directions.");
    }
    else if (marker == 'p') {
      addMessage(engine->interface, "You try talking to the object, but hear only silence.");
      addMessage(engine->interface, "It appears to be dead, inanimate, or at least not responsive");
    }
    else
    addMessage(engine->interface, "You start speaking but trail off. No one is around to hear.");
    break;
    default:
    break;
  }
}

void useItem(ENGINE *engine, char * item, int itemIndex) {
  clearMessage(engine->interface);
  if (strcmp(item, "Feeble pickaxe") == 0) {
    int forwardX = round(engine->player-> x + cos(engine->player->direction) * PLAYER_FORWARDS);
    int forwardY = round(engine->player-> y + sin(engine->player->direction) * PLAYER_FORWARDS);
    char position = getPositionInMap(engine->map, forwardY, forwardX);
    unsigned char p2 = map2[forwardY * MAP_SIZE + forwardX];
    switch(position) {
      case MAP_OPEN_SPACE:
      addMessage(engine->interface, "You swing the pickaxe. It strikes nothing.");
      break;
      default:
      // If we hit a wall or a marker
      if (position < MAP_MARKER_MIN) {
        if ((position ^ p2 ^ 0x99) != 0) {
          addMessage(engine->interface, "You swing the pickaxe against the rock. The rock crumbles apart");
          setPositionInMap(engine->map, forwardY, forwardX, MAP_OPEN_SPACE);
        } else {
        addMessage(engine->interface, "You swing the pickaxe, and it breaks");
        addMessage(engine->interface, "Well that is a bummer");
        removeItem(engine->interface, itemIndex);
        }
      }
      break;
    }
  }
}

void clean(){
  shutdown(engine);
  free(map);
  if (flag != NULL)
    free(flag);
}

int main(){
  int s = strlen(map1) + 1;
  map = (char *) calloc(s, sizeof(char));
  strncpy(map, map1, s);
  engine = createEngine(ROWS, COLS, map, MAP_SIZE, 2, 1, 6, &checkInteraction, &useItem);
  atexit(clean);
  signal(SIGTERM, exit);
  gameLoop(engine);
}

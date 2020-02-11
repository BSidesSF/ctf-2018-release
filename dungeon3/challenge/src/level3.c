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
#define MAX_STEPS 100
#define NULL_STEP 255

#define MAP_SIZE 20
const char map3[] = ""\
"--------------------"\
"*   s   X   o      *"\
"*   s      XXnXX   *"\
"*XXa&XX&XXpXX XX-ss*"\
"*s  &EEEEE EEmEE-Xs*"\
"*Xsb&E   E EE EE-ss*"\
"*X  &E   EqEElEE-ss*"\
"*   &E   E EE EE- &*"\
"*c&&&E   E EEkEE- s*"\
"*  *&E   EzEE EE-  *"\
"* **&E   E EE EE-X *"\
"* ss&&   EEEEjEE-  *"\
"*d  && &&&&&& &&& -*"\
"*X   *s*   i   s*  *"\
"*---e- -h   --sss  *"\
"*&&  X X XXXXXXXXXX*"\
"*&& f   g ssssssss *"\
"* --------         *"\
"*  sssssss         *"\
"*&&&&&&&&&&&&&&&&&&&";

ENGINE * engine;
unsigned char steps[MAX_STEPS];
unsigned int stepIndex = -1;

void readFlag(char *buffer) {
  FILE * pFile;
  long lSize;
  size_t result;

  pFile = fopen("/flag", "rb" );
  fseek(pFile , 0 , SEEK_END);
  lSize = ftell(pFile);
  rewind (pFile);
  result = fread(buffer, 1, lSize, pFile);
  buffer[lSize] = '\0';
  fclose (pFile);
}

bool hasHourglass(ENGINE *engine){
  if ( (engine->interface->inventory[0] != NULL && strcmp(engine->interface->inventory[0], "Sands of time") == 0) || 
      (engine->interface->inventory[1] != NULL && strcmp(engine->interface->inventory[1], "Sands of time") == 0)) {
    return true;
  }
  return false;
}

void useHourglass(ENGINE *engine){
  switch (steps[stepIndex]) {
  case INTERACTION_TYPE_WALK_FORWARD:
    walkAnimation(engine, PLAYER_BACKWARDS);
    break;
  case INTERACTION_TYPE_WALK_BACK:
    walkAnimation(engine, PLAYER_FORWARDS);
    break;
  case INTERACTION_TYPE_TURN_RIGHT:
    rotationAnimation(engine, M_PI/2.0, PLAYER_COUNTER_CLOCKWISE);
    break;
  case INTERACTION_TYPE_TURN_LEFT:
    rotationAnimation(engine, M_PI/2.0, PLAYER_CLOCKWISE);
    break;
  case NULL_STEP:
    addMessage(engine->interface, "Nothing happened.");
    return;
  }
  steps[stepIndex] = NULL_STEP;
  stepIndex = (stepIndex - 1) % MAX_STEPS;
}

void checkInteraction(ENGINE *engine, INTERACTION interactionType) {
  clearMessage(engine->interface);
  if (interactionType == INTERACTION_TYPE_WALK_FORWARD ||
       interactionType == INTERACTION_TYPE_WALK_BACK ||
       interactionType == INTERACTION_TYPE_TURN_LEFT || 
       interactionType == INTERACTION_TYPE_TURN_RIGHT)
  {  
    stepIndex++;
    stepIndex %= MAX_STEPS;
    steps[stepIndex] = interactionType;
  }
    
  char marker = getPositionInMap(engine->map, engine->player->y, engine->player->x);
  switch (interactionType){
    case INTERACTION_TYPE_WALK_FORWARD:
    case INTERACTION_TYPE_WALK_BACK:
    if (marker == 'a') {
      addMessage(engine->interface, "You hear a thundering voice...");
      addMessage(engine->interface, "\"Who dares to enter my temple?\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'b') {
      addMessage(engine->interface, "\"What is it you seek, traveler?\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'c') {
      addMessage(engine->interface, "\"Is it wealth you seek?\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'd') {
      addMessage(engine->interface, "\"You will find none here.\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'e') {
      addMessage(engine->interface, "\"Turn back!!\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'f') {
      addMessage(engine->interface, "\"There is nothing for you here!\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'g') {
      addMessage(engine->interface, "\"Leave this place at once!\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'h') {
      addMessage(engine->interface, "\"You are persistent... what could it be that you desire?\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'i') {
      addMessage(engine->interface, "\"Turn back! Before it is too late!\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'j') {
      addMessage(engine->interface, "\"Careful, one misstep and that will be the end of you.\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'k') {
      addMessage(engine->interface, "\"Ah... I know what it is that you seek.\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'l') {
      addMessage(engine->interface, "\"You will be dissapointed...\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'm') {
      addMessage(engine->interface, "\"There are no answers here...\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'o') {
      addMessage(engine->interface, "\"You may never make it out of this maze\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'p') {
      addMessage(engine->interface, "\"The item you seek is not here.\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 'q') {
      addMessage(engine->interface, "\"But perhaps you will find something of use.\"");
      setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    if (marker == 's') {
      addMessage(engine->interface, "Suddenly giant spikes emerge from the floor");
      addMessage(engine->interface, "It looks like this is the end for you...");
      engine->gameState = STATE_DEAD;
      return;
    }
    else if (marker == 'z') {
      addMessage(engine->interface, "There is an item on a pedestal next to you");
      addMessage(engine->interface, "It appears to be some sort of hourglass");
      addMessage(engine->interface, "It looks ornate and ancient");
    }
    break;
    case INTERACTION_TYPE_LOOK:
    if (marker == 'z') {
        addMessage(engine->interface, "You pick up the hourglass.");
        addItem(engine->interface, "Sands of time");
        setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    else {
        addMessage(engine->interface, "You are surrounded by rock and darkness. Nothing interesting here.");
    }
    break;
    case INTERACTION_TYPE_TALK:
    addMessage(engine->interface, "Its so empty here. You wonder if anyone will hear your voice again.");
    break;
    default:
    break;
  }
}

void useItem(ENGINE *engine, char * item, int itemIndex) {
  if (strcmp(item, "Sands of time") == 0) {
    clearMessage(engine->interface);
    addMessage(engine->interface, "You tipped the hourglass");
    useHourglass(engine);
  }
}

void clean(){
  shutdown(engine);
}

int main(){
  memset(steps, NULL_STEP, sizeof(steps));
  char buffer[50];
  memset(buffer, 0x39, sizeof(buffer));
  readFlag(buffer);
  char map[MAP_SIZE * MAP_SIZE + 1];
  int s = strlen(map3) + 1;
  strncpy(map, map3, s);
  engine = createEngine(ROWS, COLS, map, MAP_SIZE, 1, 1, 6, &checkInteraction, &useItem);
  atexit(clean);
  signal(SIGTERM, exit);
  gameLoop(engine);
}

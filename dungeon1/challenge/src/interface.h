#ifndef INTERFACE_H_   /* Include guard */
#define INTERFACE_H_

#include <ncurses.h>

#define INTERFACE_MESSAGES_SIZE 3
#define INTERFACE_INVENTORY_SIZE 2

struct Interface {
  WINDOW * window;
  int height;
  char * messages[INTERFACE_MESSAGES_SIZE];
  char * inventory[INTERFACE_INVENTORY_SIZE];
};

void updateInterface(struct Interface *textbox);
void addMessage(struct Interface *interface, char *message);
void clearMessage(struct Interface *interface);
void addItem(struct Interface *interface, char *item);
void removeItem(struct Interface *interface, int index);
#endif

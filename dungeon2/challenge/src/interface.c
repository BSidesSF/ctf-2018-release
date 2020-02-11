#include <ncurses.h>
#include "interface.h"

void updateInterface(struct Interface *interface){
  WINDOW *window = interface->window;
  wclear(window);
  wattron(window, A_BOLD);
  box(window, 0, 0);
  wattroff(window, A_BOLD);

  // if there are any messages, show those
  bool hasMessages = false;
  for (int i = 0; i < INTERFACE_MESSAGES_SIZE; i++) {
    if (interface->messages[i] != NULL) {
      mvwprintw(window, 1 + i, 2, interface->messages[i]);
      hasMessages = true;
    }
  }
  mvwprintw(window, interface->height - 2, 2, ">> ");

  if (!hasMessages) {
    wattron(window, A_BOLD);
    mvwprintw(window, 1, 2, "Commands");
    wattroff(window, A_BOLD);
    mvwprintw(window, 2, 2, "(L)ook   (W/A/S/D) Move");
    mvwprintw(window, 3, 2, "(T)alk   (1/2) Use item");
    wattron(window, A_BOLD);
    mvwprintw(window, 1, 37, "Inventory");
    wattroff(window, A_BOLD);
    mvwprintw(window, 2, 37, "1) ");
    if (interface->inventory[0] != NULL) {
      mvwprintw(window, 2, 40, interface->inventory[0]);
    }
    mvwprintw(window, 3, 37, "2) ");
    if (interface->inventory[1] != NULL) {
      mvwprintw(window, 3, 40, interface->inventory[1]);
    }
  }
  wrefresh(window);
}

void addMessage(struct Interface *interface, char *message) {
  // shift everything down
  for (int i = 0; i < INTERFACE_MESSAGES_SIZE; i++){
    if (i == INTERFACE_MESSAGES_SIZE - 1) {
      interface->messages[i] = message;
      return;
    }
    interface->messages[i] = interface->messages[i + 1];
  }
  updateInterface(interface);
}

void addItem(struct Interface *interface, char *item) {
  // This would be a good place to put a vulnerability
  for (int i = 0; i<INTERFACE_INVENTORY_SIZE; i++) {
    if (interface->inventory[i] == NULL) {
      interface->inventory[i] = item;
      return;
    }
  }
}

void removeItem(struct Interface *interface, int index) {
  interface->inventory[index] = NULL;
}

void clearMessage(struct Interface *interface){
  for (int i = 0; i < INTERFACE_MESSAGES_SIZE; i++){
    interface->messages[i] = NULL;
  }
}

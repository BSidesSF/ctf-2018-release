#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>

#define disable_buffering(_fd) setvbuf(_fd, NULL, _IONBF, 0)
#define clear() printf("\033[H\033[J")

#define BOARD_WIDTH 136
#define BOARD_HEIGHT 64
#define PRIZE_COUNT 16

#define CODE_LENGTH 1024

#define MAX_WIND 200
#define GRAVITY (-9.8)
#define SCALE (5000 / 80)
#define TIME_SCALE (0.4)

typedef enum {
  SCORE_LEFT,
  SCORE_RIGHT
} score_state_t;

static int delay = 50000;

#define RAD(x) (x*3.1415926/180)
void draw_board(double wind, uint8_t board[BOARD_WIDTH][BOARD_HEIGHT], uint8_t prizes[PRIZE_COUNT]) {
  ssize_t i, j;

  clear();

  printf("Wind: %f\n\n", wind);

  for(i = 0; i < BOARD_HEIGHT; i++) {
    printf("    |");
    for(j = 0; j < BOARD_WIDTH; j++) {
      printf("%c", board[j][i] ? 'o' : ' ');
    }
    printf("\n");
  }

  printf("    |");
  for(j = 0; j < BOARD_WIDTH; j++) {
    printf("-");
  }
  printf("\n");

  printf("    |");
  printf("  YOU  |");
  for(j = 0; j < (BOARD_WIDTH / 8) - 1; j++) {
    printf("   %x   |", prizes[j]);
  }
  printf("\n");
}

void clear_board(uint8_t board[BOARD_WIDTH][BOARD_HEIGHT]) {
  ssize_t i, j;

  for(i = 0; i < BOARD_HEIGHT; i++) {
    for(j = 0; j < BOARD_WIDTH; j++) {
      board[j][i] = 0;
    }
  }
}

void swap(uint8_t *a, uint8_t *b) {
  uint8_t c = *a;
  *a = *b;
  *b = c;
}

void shuffle_prizes(uint8_t prizes[PRIZE_COUNT]) {
  ssize_t i;

  for(i = 0; i < PRIZE_COUNT; i++) {
    swap(&prizes[i], &prizes[rand() % PRIZE_COUNT]);
  }
}

double get_wind() {
  return (double)((rand() % (MAX_WIND * 2)) - MAX_WIND);
}

double prompt(char *str, double min, double max) {
  for(;;) {
    char in[16];
    double result;

    printf("%s", str);
    fgets(in, 15, stdin);
    in[15] = 0;

    result = atoi(in);
    if(result >= min && result <= max) {
      return result;
    }

    printf("Please choose a value between %f and %f!\n", min, max);
    printf("\n");
  }
}

int8_t shoot(uint8_t board[BOARD_WIDTH][BOARD_HEIGHT], double wind, uint8_t prizes[PRIZE_COUNT], double power, double angle) {
  double v0_v = sin(RAD(angle)) * power;
  double v0_h = cos(RAD(angle)) * power;

  printf("Initial velocity (v) = %f, (h) = %f\n", v0_v, v0_h);

  double t;

  for(t = 0; t < 10000; t += TIME_SCALE) {
    double p_v = (((0.5 * pow(t, 2) * GRAVITY) + (t * v0_v)) / SCALE);
    double p_h = (((0.5 * pow(t, 2) * (wind / 50)) + (t * v0_h)) / SCALE) + 4;

    if(p_v < 0) {
      int result = ((int) p_h / 8) - 1;
      if(result < 0 || result >= PRIZE_COUNT) {
        return -1;
      }
      return prizes[result];
    }

    if(p_v > 0 && p_v < BOARD_HEIGHT) {
      if(p_h > 0 && p_h < BOARD_WIDTH) {
        board[(int)p_h][BOARD_HEIGHT - (int)p_v - 1] = 1;
      }
    }
    draw_board(wind, board, prizes);

    usleep(delay);
  }

  return -1;
}

int main(int argc, char *argv[]) {
  ssize_t i;
  uint8_t *scores = mmap(NULL, CODE_LENGTH, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
  ssize_t score_position = 0;
  score_state_t score_state = SCORE_LEFT;
  srand(time(NULL));

  disable_buffering(stdin);
  disable_buffering(stdout);
  disable_buffering(stderr);

  for(;;) {
    uint8_t board[BOARD_WIDTH][BOARD_HEIGHT];
    uint8_t prizes[PRIZE_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    double wind = get_wind();

    clear_board(board);
    shuffle_prizes(prizes);
    draw_board(wind, board, prizes);

    /* TODO: Display score */
    printf("Your current program:\n");
    for(i = 0; i < score_position; i++) {
      printf("%x ", scores[i]);
    }
    if(score_state == SCORE_RIGHT) {
      printf("%x?", scores[score_position] >> 4);
    }
    printf("\n");
    double power = prompt("Power (0-1000) --> ", 0, 1000);
    double angle = prompt("Angle (0-360) --> ", 0, 360);

    int8_t result = shoot(board, wind, prizes, power, angle);
    if(result < 0) {
      printf("You missed! Time to run the code you've built!\n");
      printf("\n");
      break;
    }

    if(score_state == SCORE_LEFT) {
      scores[score_position] = (result & 0x0F) << 4;
      score_state = SCORE_RIGHT;
    } else {
      scores[score_position] |= result & 0x0F;
      score_state = SCORE_LEFT;
      score_position++;
    }

    printf("Congratulations! You hit the nibble %x!\n", result);
    printf("Added to the code! Miss a shot to run!\n");
    printf("\n");
    printf("<press enter>\n");
    getchar();
  }

  alarm(60);

  asm("call *%0\n" : :"r"(scores));

  return 0;
}

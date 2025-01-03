#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "game/snake.h"

void interruptHandler(int sig)
{
  printf("\033[?25h"); // Show the cursor
  exit(0);
}

int main()
{
  signal(SIGINT, interruptHandler);

  printf("\033[2J");   // Clear the console
  printf("\033[?25l"); // Hide the cursor

  startGame();

  printf("\033[?25h"); // Show the cursor

  return 0;
}

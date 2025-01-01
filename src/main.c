#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <conio.h>
#include "renderer/renderer.h"

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

  struct ViewState view;
  initializeViewState(&view, 60, 30);

  drawViewBorder(&view);

  int x = 10;
  int y = 10;
  addChar(&view, x, y, L'X');

  while (1)
  {
    if (_kbhit())
    {
      char ch = _getch();
      switch (ch)
      {
      case 'a':
      case 'A':
        clearChar(&view, x, y);
        x -= 1;
        addChar(&view, x, y, L'X');
        break;
      case 'd':
      case 'D':
        clearChar(&view, x, y);
        x += 1;
        addChar(&view, x, y, L'X');
        break;
      }
    }

    render(&view);
    usleep(1000);
  }

  freeViewState(&view);

  return 0;
}

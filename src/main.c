#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <conio.h>
#include <locale.h>
#include "renderer/renderer.h"

void interruptHandler(int sig)
{
  printf("\033[?25h"); // Show the cursor
  exit(0);
}

int main()
{
  setlocale(LC_ALL, ""); // Set the locale to support wide characters

  signal(SIGINT, interruptHandler);
  printf("\033[2J");   // Clear the console
  printf("\033[?25l"); // Hide the cursor

  struct RenderBuffer buffer;
  initRenderBuffer(&buffer, 80, 40, true);

  wchar_t shapeStr[] = L"┌──────────────────┐\n"
                       L"│                  │\n"
                       L"└──────────────────┘";
  struct Shape shape = addShape(&buffer, shapeStr, 30, 35);

  while (1)
  {
    if (_kbhit())
    {
      char ch = _getch();
      switch (ch)
      {
      case 'w':
      case 'W':
        translateShape(&buffer, &shape, 0, -1);
        break;
      case 'a':
      case 'A':
        translateShape(&buffer, &shape, -1, 0);
        break;
      case 's':
      case 'S':
        translateShape(&buffer, &shape, 0, 1);
        break;
      case 'd':
      case 'D':
        translateShape(&buffer, &shape, 1, 0);
        break;
      }
    }

    render(&buffer);
  }

  freeRenderBuffer(&buffer);
  freeShape(&shape);
  printf("\033[?25h"); // Show the cursor

  return 0;
}

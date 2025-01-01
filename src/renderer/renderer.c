#include <stdlib.h>
#include <stdio.h>
#include "renderer.h"

void initializeViewState(struct ViewState *view, int width, int height)
{
  view->width = width;
  view->height = height;
  view->cells = (wchar_t *)malloc(width * height * sizeof(wchar_t));

  for (int i = 0; i < view->width * view->height; i++)
  {
    view->cells[i] = L' ';
  }
}

void freeViewState(struct ViewState *view)
{
  free(view->cells);
}

void addChar(struct ViewState *view, int x, int y, wchar_t c)
{
  if (x < 0 || x >= view->width || y < 0 || y >= view->height)
    return;

  view->cells[y * view->width + x] = c;
}

void clearChar(struct ViewState *view, int x, int y)
{
  addChar(view, x, y, L' ');
}

void drawViewBorder(struct ViewState *view)
{
  for (int x = 0; x < view->width; x++)
    addChar(view, x, 0, L'─');

  for (int x = 0; x < view->width; x++)
    addChar(view, x, view->height - 1, L'─');

  for (int y = 0; y < view->height; y++)
    addChar(view, 0, y, L'│');

  for (int y = 0; y < view->height; y++)
    addChar(view, view->width - 1, y, L'│');

  addChar(view, 0, 0, L'┌');
  addChar(view, view->width - 1, 0, L'┐');
  addChar(view, 0, view->height - 1, L'└');
  addChar(view, view->width - 1, view->height - 1, L'┘');
}

void render(struct ViewState *view)
{
  printf("\033[H"); // Move the cursor to the top-left corner of the console

  for (int i = 0; i < view->width * view->height; i++)
  {
    if (i % view->width == 0)
    {
      printf("\n");
    }

    printf("%c", view->cells[i]);
  }
}

#ifndef RENDERER_H
#define RENDERER_H

#include <wchar.h>

struct ViewState
{
  int width;
  int height;
  wchar_t *cells;
};

void initializeViewState(struct ViewState *view, int width, int height);
void freeViewState(struct ViewState *view);
void addChar(struct ViewState *view, int x, int y, wchar_t c);
void clearChar(struct ViewState *view, int x, int y);
void drawViewBorder(struct ViewState *view);
void render(struct ViewState *view);

#endif

#ifndef RENDERER_H
#define RENDERER_H

#include <wchar.h>
#include <stdbool.h>

struct RenderBuffer
{
  int width;
  int height;
  wchar_t *chars;
};

struct RenderUnit
{
  wchar_t c;
  int x;
  int y;
};

struct Shape
{
  struct RenderUnit *units;
  int unitCount;
};

void initRenderBuffer(struct RenderBuffer *buffer, int width, int height, bool border);
void freeRenderBuffer(struct RenderBuffer *buffer);

struct Shape addShape(struct RenderBuffer *buffer, const wchar_t *shapeStr, int x, int y);
void translateShape(struct RenderBuffer *buffer, struct Shape *shape, int dx, int dy);
void freeShape(struct Shape *shape);

void render(struct RenderBuffer *buffer);

#endif

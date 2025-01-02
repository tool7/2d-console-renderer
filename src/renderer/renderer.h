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

struct RenderPixel
{
  wchar_t c;
  int x;
  int y;
};

struct RenderShape
{
  struct RenderPixel *pixels;
  int pixelCount;
};

struct RenderBuffer createRenderBuffer(int width, int height, bool border);
void clearRenderBuffer(struct RenderBuffer *buffer, bool border);
bool inBounds(struct RenderBuffer *buffer, int x, int y, bool border);
void addChar(struct RenderBuffer *buffer, int x, int y, wchar_t c);
void clearChar(struct RenderBuffer *buffer, int x, int y);
struct RenderShape addShape(struct RenderBuffer *buffer, const wchar_t *shapeStr, int x, int y);
void translateShape(struct RenderBuffer *buffer, struct RenderShape *shape, int dx, int dy);

void freeRenderBuffer(struct RenderBuffer *buffer);
void freeShape(struct RenderShape *shape);

void render(struct RenderBuffer *buffer);

#endif

#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>

extern const char *RED;
extern const char *GREEN;
extern const char *YELLOW;
extern const char *BLUE;
extern const char *MAGENTA;
extern const char *CYAN;
extern const char *DIMMED;
extern const char *BLINKING;
extern const char *RESET;

struct RenderPixel
{
  char c;
  const char *colorEscapeCode;
  int x;
  int y;
};

struct RenderBuffer
{
  int width;
  int height;
  struct RenderPixel *pixels;
};

struct RenderShape
{
  struct RenderPixel *pixels;
  int pixelCount;
};

struct RenderBuffer createRenderBuffer(int width, int height, bool border);
void clearRenderBuffer(struct RenderBuffer *buffer, bool border);
bool inBounds(struct RenderBuffer *buffer, int x, int y, bool border);
void addChar(struct RenderBuffer *buffer, int x, int y, char c);
void addColoredChar(struct RenderBuffer *buffer, int x, int y, char c, const char *colorEscapeCode);
void clearChar(struct RenderBuffer *buffer, int x, int y);
struct RenderShape addShape(struct RenderBuffer *buffer, const char *shapeStr, int x, int y);
void translateShape(struct RenderBuffer *buffer, struct RenderShape *shape, int dx, int dy);

void freeRenderBuffer(struct RenderBuffer *buffer);
void freeShape(struct RenderShape *shape);

void render(struct RenderBuffer *buffer);

#endif

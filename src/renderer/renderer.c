#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "renderer.h"

const char *RED = "\033[31m";
const char *GREEN = "\033[32m";
const char *YELLOW = "\033[33m";
const char *BLUE = "\033[34m";
const char *MAGENTA = "\033[35m";
const char *CYAN = "\033[36m";
const char *DIMMED = "\033[2m";
const char *BLINKING = "\033[5m";
const char *RESET = "\033[0m";

bool inBounds(struct RenderBuffer *buffer, int x, int y, bool border)
{
  if (border)
    return x > 0 && x < buffer->width - 1 && y > 0 && y < buffer->height - 1;
  else
    return x >= 0 && x < buffer->width && y >= 0 && y < buffer->height;
}

void addChar(struct RenderBuffer *buffer, int x, int y, char c)
{
  if (!inBounds(buffer, x, y, false))
    return;

  buffer->pixels[y * buffer->width + x].c = c;
}

void addColoredChar(struct RenderBuffer *buffer, int x, int y, char c, const char *colorEscapeCode)
{
  if (!inBounds(buffer, x, y, false))
    return;

  buffer->pixels[y * buffer->width + x].c = c;
  buffer->pixels[y * buffer->width + x].colorEscapeCode = colorEscapeCode;
}

void clearChar(struct RenderBuffer *buffer, int x, int y)
{
  addChar(buffer, x, y, ' ');
}

void _addBorder(struct RenderBuffer *buffer)
{
  for (int x = 0; x < buffer->width; x++)
    addColoredChar(buffer, x, 0, '-', DIMMED);
  for (int x = 0; x < buffer->width; x++)
    addColoredChar(buffer, x, buffer->height - 1, '-', DIMMED);
  for (int y = 0; y < buffer->height; y++)
    addColoredChar(buffer, 0, y, '|', DIMMED);
  for (int y = 0; y < buffer->height; y++)
    addColoredChar(buffer, buffer->width - 1, y, '|', DIMMED);

  addColoredChar(buffer, 0, 0, '+', DIMMED);
  addColoredChar(buffer, buffer->width - 1, 0, '+', DIMMED);
  addColoredChar(buffer, 0, buffer->height - 1, '+', DIMMED);
  addColoredChar(buffer, buffer->width - 1, buffer->height - 1, '+', DIMMED);
}

struct RenderBuffer createRenderBuffer(int width, int height, bool border)
{
  struct RenderBuffer buffer;
  buffer.width = width;
  buffer.height = height;
  buffer.pixels = (struct RenderPixel *)malloc(width * height * sizeof(struct RenderPixel));

  for (int i = 0; i < buffer.width * buffer.height; i++)
  {
    buffer.pixels[i].x = i % buffer.width;
    buffer.pixels[i].y = i / buffer.width;
    buffer.pixels[i].c = ' ';
    buffer.pixels[i].colorEscapeCode = RESET;
  }

  if (border)
    _addBorder(&buffer);

  return buffer;
}

void clearRenderBuffer(struct RenderBuffer *buffer, bool border)
{
  for (int i = 0; i < buffer->width * buffer->height; i++)
  {
    buffer->pixels[i].c = ' ';
  }

  if (border)
    _addBorder(buffer);
}

void freeRenderBuffer(struct RenderBuffer *buffer)
{
  free(buffer->pixels);
}

struct RenderShape addShape(struct RenderBuffer *buffer, const char *shapeStr, int x, int y)
{
  struct RenderShape shape;
  shape.pixelCount = 0;

  for (int i = 0; shapeStr[i] != '\0'; i++)
  {
    if (shapeStr[i] != '\n')
    {
      shape.pixelCount++;
    }
  }

  shape.pixels = (struct RenderPixel *)malloc(shape.pixelCount * sizeof(struct RenderPixel));

  int pixelIndex = 0;
  int xOffset = 0;
  int yOffset = 0;
  for (int i = 0; shapeStr[i] != '\0'; i++)
  {
    if (shapeStr[i] != '\n')
    {
      shape.pixels[pixelIndex].c = shapeStr[i];
      shape.pixels[pixelIndex].x = x + xOffset;
      shape.pixels[pixelIndex].y = y + yOffset;
      pixelIndex++;
      xOffset++;
    }
    else
    {
      xOffset = 0;
      yOffset++;
    }
  }

  for (int i = 0; i < shape.pixelCount; i++)
  {
    addChar(buffer, shape.pixels[i].x, shape.pixels[i].y, shape.pixels[i].c);
  }

  return shape;
}

void translateShape(struct RenderBuffer *buffer, struct RenderShape *shape, int dx, int dy)
{
  // Checking if new coordinates are within the buffer bounds
  for (int i = 0; i < shape->pixelCount; i++)
  {
    int newX = shape->pixels[i].x + dx;
    int newY = shape->pixels[i].y + dy;

    if (newX <= 0 || newX >= buffer->width - 1 || newY <= 0 || newY >= buffer->height - 1)
      return;
  }

  for (int i = 0; i < shape->pixelCount; i++)
  {
    clearChar(buffer, shape->pixels[i].x, shape->pixels[i].y);
  }

  for (int i = 0; i < shape->pixelCount; i++)
  {
    shape->pixels[i].x += dx;
    shape->pixels[i].y += dy;
    addChar(buffer, shape->pixels[i].x, shape->pixels[i].y, shape->pixels[i].c);
  }
}

void freeShape(struct RenderShape *shape)
{
  free(shape->pixels);
}

void render(struct RenderBuffer *buffer)
{
  printf("\033[H"); // Move the cursor to the top-left corner of the console

  for (int i = 0; i < buffer->width * buffer->height; i++)
  {
    if (i % buffer->width == 0)
    {
      printf("\n");
    }

    printf("%s%c%s", buffer->pixels[i].colorEscapeCode, buffer->pixels[i].c, RESET);
  }
}

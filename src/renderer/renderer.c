#include <stdlib.h>
#include <stdio.h>
#include "renderer.h"

// ======== Private functions ========
void _addChar(struct RenderBuffer *buffer, int x, int y, wchar_t c)
{
  if (x < 0 || x >= buffer->width || y < 0 || y >= buffer->height)
    return;

  buffer->chars[y * buffer->width + x] = c;
}

void _clearChar(struct RenderBuffer *buffer, int x, int y)
{
  _addChar(buffer, x, y, L' ');
}

void _addBorder(struct RenderBuffer *buffer)
{
  for (int x = 0; x < buffer->width; x++)
    _addChar(buffer, x, 0, L'─');

  for (int x = 0; x < buffer->width; x++)
    _addChar(buffer, x, buffer->height - 1, L'─');

  for (int y = 0; y < buffer->height; y++)
    _addChar(buffer, 0, y, L'│');

  for (int y = 0; y < buffer->height; y++)
    _addChar(buffer, buffer->width - 1, y, L'│');

  _addChar(buffer, 0, 0, L'┌');
  _addChar(buffer, buffer->width - 1, 0, L'┐');
  _addChar(buffer, 0, buffer->height - 1, L'└');
  _addChar(buffer, buffer->width - 1, buffer->height - 1, L'┘');
}

// ======== Public functions ========
void initRenderBuffer(struct RenderBuffer *buffer, int width, int height, bool border)
{
  buffer->width = width;
  buffer->height = height;
  buffer->chars = (wchar_t *)malloc(width * height * sizeof(wchar_t));

  for (int i = 0; i < buffer->width * buffer->height; i++)
  {
    buffer->chars[i] = L' ';
  }

  if (border)
  {
    _addBorder(buffer);
  }
}

void freeRenderBuffer(struct RenderBuffer *buffer)
{
  free(buffer->chars);
}

struct Shape addShape(struct RenderBuffer *buffer, const wchar_t *shapeStr, int x, int y)
{
  struct Shape shape;
  shape.unitCount = 0;

  for (int i = 0; shapeStr[i] != L'\0'; i++)
  {
    if (shapeStr[i] != L'\n')
    {
      shape.unitCount++;
    }
  }

  shape.units = (struct RenderUnit *)malloc(shape.unitCount * sizeof(struct RenderUnit));

  int unitIndex = 0;
  int xOffset = 0;
  int yOffset = 0;
  for (int i = 0; shapeStr[i] != L'\0'; i++)
  {
    if (shapeStr[i] != L'\n')
    {
      shape.units[unitIndex].c = shapeStr[i];
      shape.units[unitIndex].x = x + xOffset;
      shape.units[unitIndex].y = y + yOffset;
      unitIndex++;
      xOffset++;
    }
    else
    {
      xOffset = 0;
      yOffset++;
    }
  }

  for (int i = 0; i < shape.unitCount; i++)
  {
    _addChar(buffer, shape.units[i].x, shape.units[i].y, shape.units[i].c);
  }

  return shape;
}

void freeShape(struct Shape *shape)
{
  free(shape->units);
}

void translateShape(struct RenderBuffer *buffer, struct Shape *shape, int dx, int dy)
{
  // Checking if new coordinates are within the buffer bounds
  for (int i = 0; i < shape->unitCount; i++)
  {
    int newX = shape->units[i].x + dx;
    int newY = shape->units[i].y + dy;

    if (newX <= 0 || newX >= buffer->width - 1 || newY <= 0 || newY >= buffer->height - 1)
      return;
    }

  for (int i = 0; i < shape->unitCount; i++)
    _clearChar(buffer, shape->units[i].x, shape->units[i].y);

  for (int i = 0; i < shape->unitCount; i++)
  {
    shape->units[i].x += dx;
    shape->units[i].y += dy;
    _addChar(buffer, shape->units[i].x, shape->units[i].y, shape->units[i].c);
  }
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

    putwchar(buffer->chars[i]);
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <unistd.h>

#include "snake.h"
#include "../renderer/renderer.h"

#define MAX_SNAKE_LENGTH 100

const bool MAP_BORDER = true;

enum Direction
{
  UP,
  LEFT,
  DOWN,
  RIGHT
};

struct Snake
{
  struct RenderPixel *body;
  enum Direction direction;
  int length;
};

void updateSnakePosition(struct RenderBuffer *buffer, struct Snake *snake)
{
  for (int i = snake->length - 1; i > 0; i--)
  {
    snake->body[i].x = snake->body[i - 1].x;
    snake->body[i].y = snake->body[i - 1].y;
  }

  switch (snake->direction)
  {
  case UP:
    snake->body[0].y--;
    break;
  case LEFT:
    snake->body[0].x--;
    break;
  case DOWN:
    snake->body[0].y++;
    break;
  case RIGHT:
    snake->body[0].x++;
    break;
  }

  if (!inBounds(buffer, snake->body[0].x, snake->body[0].y, MAP_BORDER))
  {
    if (snake->body[0].x < 0)
    {
      snake->body[0].x = buffer->width - 1;
    }
    else if (snake->body[0].x >= buffer->width)
    {
      snake->body[0].x = 0;
    }

    if (snake->body[0].y < 0)
    {
      snake->body[0].y = buffer->height - 1;
    }
    else if (snake->body[0].y >= buffer->height)
    {
      snake->body[0].y = 0;
    }
  }
}

void addSnakeToRenderBuffer(struct RenderBuffer *buffer, struct Snake *snake)
{
  for (int i = 0; i < snake->length; i++)
  {
    char c = ' ';
    if (i == 0) // Head
      c = 'O';
    else if (i == snake->length - 1) // Tail
    {
      switch (snake->direction)
      {
      case UP:
        c = 'v';
        break;
      case LEFT:
        c = '>';
        break;
      case DOWN:
        c = '^';
        break;
      case RIGHT:
        c = '<';
        break;
      }
    }
    else // Body
      c = '#';

    addChar(buffer, snake->body[i].x, snake->body[i].y, c);
  }
}

void startGame()
{
  // int score = 0;

  struct RenderBuffer buffer = createRenderBuffer(80, 40, MAP_BORDER);
  struct Snake snake = {
      .body = (struct RenderPixel *)malloc(MAX_SNAKE_LENGTH * sizeof(struct RenderPixel)),
      .length = 5,
      .direction = RIGHT};

  for (int i = 0; i < snake.length; i++)
  {
    snake.body[i].x = 40 + i;
    snake.body[i].y = 20;
  }

  while (1)
  {
    clearRenderBuffer(&buffer, MAP_BORDER);

    if (_kbhit())
    {
      char ch = _getch();
      switch (ch)
      {
      case 'w':
      case 'W':
        if (snake.direction != DOWN)
          snake.direction = UP;
        break;
      case 'a':
      case 'A':
        if (snake.direction != RIGHT)
          snake.direction = LEFT;
        break;
      case 's':
      case 'S':
        if (snake.direction != UP)
          snake.direction = DOWN;
        break;
      case 'd':
      case 'D':
        if (snake.direction != LEFT)
          snake.direction = RIGHT;
        break;
      }
    }

    updateSnakePosition(&buffer, &snake);
    addSnakeToRenderBuffer(&buffer, &snake);
    render(&buffer);

    usleep(1000);
  }

  freeRenderBuffer(&buffer);
}
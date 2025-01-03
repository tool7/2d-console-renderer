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
  struct RenderPixel body[MAX_SNAKE_LENGTH];
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
    if (i == 0)
      c = 'O'; // Head
    else
      c = '#'; // Tail

    addChar(buffer, snake->body[i].x, snake->body[i].y, c);
  }
}

void addFoodToRenderBuffer(struct RenderBuffer *buffer, struct RenderPixel *food)
{
  addChar(buffer, food->x, food->y, '@');
}

struct RenderPixel spawnFoodOnRandomPosition(struct RenderBuffer *buffer)
{
  int x = rand() % (buffer->width - 2) + 1;
  int y = rand() % (buffer->height - 2) + 1;

  return (struct RenderPixel){.x = x, .y = y};
}

bool checkIfSnakeAteFood(struct Snake *snake, struct RenderPixel *food)
{
  return snake->body[0].x == food->x && snake->body[0].y == food->y;
}

bool checkIfSnakeHitItself(struct Snake *snake)
{
  for (int i = 1; i < snake->length; i++)
  {
    if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y)
      return true;
  }

  return false;
}

void startGame()
{
  int score = 0;

  struct RenderBuffer buffer = createRenderBuffer(60, 30, MAP_BORDER);
  struct Snake snake = {
      .body = {},
      .length = 10,
      .direction = LEFT};

  // INFO: Make sure that initial snake position corresponds to initial direction
  for (int i = 0; i < snake.length; i++)
  {
    snake.body[i].x = 40 + i;
    snake.body[i].y = 20;
  }

  struct RenderPixel food = spawnFoodOnRandomPosition(&buffer);

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

    if (checkIfSnakeHitItself(&snake))
    {
      printf("\nGame Over! Score: %d\n", score);
      break;
    }

    if (checkIfSnakeAteFood(&snake, &food))
    {
      score++;
      snake.length++;

      if (snake.length >= MAX_SNAKE_LENGTH)
      {
        printf("\nYou won! Score: %d\n", score);
        break;
      }

      food = spawnFoodOnRandomPosition(&buffer);
    }

    addSnakeToRenderBuffer(&buffer, &snake);
    addFoodToRenderBuffer(&buffer, &food);
    render(&buffer);

    usleep(10000);
  }

  freeRenderBuffer(&buffer);
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <unistd.h>

#include "snake.h"
#include "../renderer/renderer.h"

#define GAME_LOOP_DELAY 10000
#define MAP_BORDER true
#define MAP_WIDTH 60
#define MAP_HEIGHT 30
#define SNAKE_INITIAL_POSITION_X 40
#define SNAKE_INITIAL_POSITION_Y 20
#define SNAKE_INITIAL_LENGTH 10
#define SNAKE_MAX_LENGTH 100

enum Direction
{
  UP,
  LEFT,
  DOWN,
  RIGHT
};

struct Snake
{
  struct RenderPixel body[SNAKE_MAX_LENGTH];
  enum Direction direction;
  int length;
};

void handleInput(struct Snake *snake)
{
  if (_kbhit())
  {
    char ch = _getch();
    switch (ch)
    {
    case 'w':
    case 'W':
      if (snake->direction != DOWN)
        snake->direction = UP;
      break;
    case 'a':
    case 'A':
      if (snake->direction != RIGHT)
        snake->direction = LEFT;
      break;
    case 's':
    case 'S':
      if (snake->direction != UP)
        snake->direction = DOWN;
      break;
    case 'd':
    case 'D':
      if (snake->direction != LEFT)
        snake->direction = RIGHT;
      break;
    }
  }
}

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

    addColoredChar(buffer, snake->body[i].x, snake->body[i].y, c, GREEN);
  }
}

void addFoodToRenderBuffer(struct RenderBuffer *buffer, struct RenderPixel *food)
{
  char *blinking = strdup(BLINKING);
  const char *blinkingYellowColorCode = strcat(blinking, YELLOW);
  addColoredChar(buffer, food->x, food->y, '@', blinkingYellowColorCode);
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

  struct RenderBuffer buffer = createRenderBuffer(MAP_WIDTH, MAP_HEIGHT, MAP_BORDER);
  struct Snake snake = {
      .body = {},
      .length = SNAKE_INITIAL_LENGTH,
      .direction = LEFT};

  // INFO: Make sure that initial snake position corresponds to initial direction
  for (int i = 0; i < snake.length; i++)
  {
    snake.body[i].x = SNAKE_INITIAL_POSITION_X + i;
    snake.body[i].y = SNAKE_INITIAL_POSITION_Y;
  }

  struct RenderPixel food = spawnFoodOnRandomPosition(&buffer);

  while (1)
  {
    clearRenderBuffer(&buffer, MAP_BORDER);
    handleInput(&snake);
    updateSnakePosition(&buffer, &snake);

    if (checkIfSnakeHitItself(&snake))
    {
      printf("\n%sGame Over! Score: %d%s\n", YELLOW, score, RESET);
      break;
    }

    if (checkIfSnakeAteFood(&snake, &food))
    {
      score++;
      snake.length++;

      if (snake.length >= SNAKE_MAX_LENGTH)
      {
        printf("\n%sYou won! Score: %d%s\n", YELLOW, score, RESET);
        break;
      }

      food = spawnFoodOnRandomPosition(&buffer);
    }

    addSnakeToRenderBuffer(&buffer, &snake);
    addFoodToRenderBuffer(&buffer, &food);
    render(&buffer);

    usleep(GAME_LOOP_DELAY);
  }

  freeRenderBuffer(&buffer);
}
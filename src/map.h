#ifndef MAP_H
#define MAP_H

#include "player.h"

#define MAP_MIN_SIZE 5
#define MAP_SIZE_RANGE 26
#define OBSTACLE_MIN_CHANCE 15
#define OBSTACLE_CHANCE_RANGE 16

#define STEP_STAMINA_COST 1
#define FLOOR_STAMINA_COST 5
#define FLOOR_TIME_COST 1
#define BASE_GOLD_MULTIPLIER 10

enum CellType {
  CELL_EMPTY = '.',
  CELL_TREE = 'T',
  CELL_ROCK = 'R',
  CELL_CAMP = '^',
  CELL_HERO = '@',
  CELL_FOG = '?'
};

enum Direction {
  DIR_NONE = 0,
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_SCREAM,
  DIR_QUIT
};

typedef struct {
  char **grid;
  int width;
  int height;
  int floor;
  int camp_x;
  int camp_y;
  int scream_x;
  int scream_y;
  int scream_radius;
  int scream_steps;
} Map;

// Создание и удаление карты
Map *create_map(int floor_num);
void free_map(Map *map);

// Отображение и логика приключения
void print_map(const Map *map, const Player *player);
void travel(Player *player);

#endif
#ifndef MAP_H
#define MAP_H

#include "player.h"

#define MAP_MIN_SIZE 5
#define MAP_SIZE_RANGE 26

#define FLOOR_STAMINA_COST 5
#define MAP_SKETCH_STAMINA_COST 5
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
  DIR_SKETCH,
  DIR_QUIT
};

enum Biome {
  BIOME_LIGHT_FOREST = 0, // Редколесье
  BIOME_SWAMP,            // Топь
  BIOME_STONE_RUINS,      // Каменные руины
  BIOME_CRYSTAL_CAVE,     // Кристальная пещера
  BIOME_COUNT
};

typedef struct {
  enum Biome biome;
  int obstacle_chance;
  int gold_multiplier;
  int step_stamina_cost;
  int is_mapped;
} MapInfo;

typedef struct {
  char **grid;
  int width;
  int height;
  int floor;
  MapInfo info;
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
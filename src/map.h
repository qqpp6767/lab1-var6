#ifndef MAP_H
#define MAP_H

typedef struct {
  char **grid;
  int width;
  int height;
  int camp_x;
  int camp_y;
  int hero_x;
  int hero_y;
  int scream_x;
  int scream_y;
  int scream_radius;
  int scream_steps;
} Map;

// Создание и удаление карты
Map *create_map(void);
void free_map(Map *map);

// Отображение и логика путешествия
void print_map(const Map *map);
void travel(void);

#endif
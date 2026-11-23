#include "map.h"
#include "utils.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Поиск случайной свободной клетки '.'
static int random_free_cell(const Map *map, int *cell_x, int *cell_y) {
  int free_cells = 0;
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      if (map->grid[y][x] == '.') {
        free_cells++;
      }
    }
  }
  if (free_cells == 0) {
    return 0;
  }

  int target = rand() % free_cells;
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      if (map->grid[y][x] == '.') {
        if (target == 0) {
          *cell_x = x;
          *cell_y = y;
          return 1;
        }
        target--;
      }
    }
  }
  return 0;
}

// Динамическое создание карты и её сетки
Map *create_map(void) {
  Map *map = malloc(sizeof(Map));
  if (map == NULL) {
    return NULL;
  }

  map->width = rand() % 26 + 5;
  map->height = rand() % 26 + 5;
  int chance = rand() % 16 + 15;

  map->grid = malloc(map->height * sizeof(char *));
  if (map->grid == NULL) {
    free_map(map);
    return NULL;
  }

  int free_cells = 0;
  for (int y = 0; y < map->height; y++) {
    map->grid[y] = malloc(map->width + 1);
    if (map->grid[y] == NULL) {
      map->height = y; // Чтобы free_map освободил только выделенные строки
      free_map(map);
      return NULL;
    }

    for (int x = 0; x < map->width; x++) {
      if (rand() % 100 < chance) {
        map->grid[y][x] = (rand() % 2 == 0) ? 'T' : 'R';
      } else {
        map->grid[y][x] = '.';
        free_cells++;
      }
    }
    map->grid[y][map->width] = '\0';
  }

  // Гарантируем минимум 2 свободные клетки
  for (int y = 0; y < map->height && free_cells < 2; y++) {
    for (int x = 0; x < map->width && free_cells < 2; x++) {
      if (map->grid[y][x] != '.') {
        map->grid[y][x] = '.';
        free_cells++;
      }
    }
  }

  random_free_cell(map, &map->camp_x, &map->camp_y);
  map->grid[map->camp_y][map->camp_x] = '^';

  map->hero_x = map->camp_x;
  map->hero_y = map->camp_y;
  map->scream_x = -1;
  map->scream_y = -1;
  map->scream_radius = 0;
  map->scream_steps = 0;

  return map;
}

// Освобождение памяти карты
void free_map(Map *map) {
  if (map == NULL) {
    return;
  }

  if (map->grid != NULL) {
    for (int y = 0; y < map->height; y++) {
      free(map->grid[y]);
    }
    free(map->grid);
  }

  uintptr_t addr = (uintptr_t)map;
  free(map);
  printf("Память структуры по адресу %p освобождена\n", (void *)addr);
}

// Отрисовка карты с туманом войны
void print_map(const Map *map) {
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      int dx = x - map->scream_x;
      int dy = y - map->scream_y;

      if (x == map->hero_x && y == map->hero_y) {
        putchar('@');
      } else if (map->scream_steps > 0 && dx * dx + dy * dy <= map->scream_radius * map->scream_radius) {
        putchar(map->grid[y][x]);
      } else {
        putchar('?');
      }
    }
    putchar('\n');
  }
}

// Активация крика (подсветка области вокруг героя)
static void activate_scream(Map *map) {
  map->scream_x = map->hero_x;
  map->scream_y = map->hero_y;
  map->scream_radius = rand() % 3 + 5;
  map->scream_steps = rand() % 2 + 2;
}

// Переход на новый участок карты при выходе за границы
static int switch_to_new_map(Map **map_ptr, int next_x, int next_y, int *distance) {
  Map *old_map = *map_ptr;
  int old_w = old_map->width;
  int old_h = old_map->height;
  int old_hero_x = old_map->hero_x;
  int old_hero_y = old_map->hero_y;

  free_map(old_map);
  (*distance)++;

  Map *new_map = create_map();
  if (new_map == NULL) {
    *map_ptr = NULL;
    return 0;
  }

  if (next_x < 0) {
    new_map->hero_x = new_map->width - 1;
    new_map->hero_y = (old_hero_y < new_map->height) ? old_hero_y : new_map->height / 2;
  } else if (next_x >= old_w) {
    new_map->hero_x = 0;
    new_map->hero_y = (old_hero_y < new_map->height) ? old_hero_y : new_map->height / 2;
  } else if (next_y < 0) {
    new_map->hero_y = new_map->height - 1;
    new_map->hero_x = (old_hero_x < new_map->width) ? old_hero_x : new_map->width / 2;
  } else if (next_y >= old_h) {
    new_map->hero_y = 0;
    new_map->hero_x = (old_hero_x < new_map->width) ? old_hero_x : new_map->width / 2;
  }

  if (new_map->grid[new_map->hero_y][new_map->hero_x] != '^') {
    new_map->grid[new_map->hero_y][new_map->hero_x] = '.';
  }

  *map_ptr = new_map;
  return 1;
}

// Шаг героя по карте
static int move_hero(Map **map_ptr, int dx, int dy, int *distance) {
  Map *map = *map_ptr;
  int next_x = map->hero_x + dx;
  int next_y = map->hero_y + dy;

  if (next_x < 0 || next_x >= map->width || next_y < 0 || next_y >= map->height) {
    return switch_to_new_map(map_ptr, next_x, next_y, distance);
  }

  if (map->grid[next_y][next_x] == 'T' || map->grid[next_y][next_x] == 'R') {
    return 1;
  }

  map->hero_x = next_x;
  map->hero_y = next_y;

  if (map->scream_steps > 0) {
    map->scream_steps--;
  }
  return 1;
}

// Главный цикл путешествия по ночному лесу
void travel(void) {
  int distance = 0;
  Map *map = create_map();
  if (map == NULL) {
    return;
  }

  while (1) {
    printf("\nДистанция: %d\n", distance);
    print_map(map);
    printf("> ");

    char cmd;
    if (!input_char(&cmd)) {
      continue;
    }

    if (cmd == 'q' || cmd == 'Q') {
      if (map->hero_x == map->camp_x && map->hero_y == map->camp_y) {
        free_map(map);
        return;
      }
      continue;
    }

    if (cmd == 'r' || cmd == 'R') {
      activate_scream(map);
      continue;
    }

    int dx = 0, dy = 0;
    if (cmd == 'w' || cmd == 'W') {
      dy = -1;
    } else if (cmd == 's' || cmd == 'S') {
      dy = 1;
    } else if (cmd == 'a' || cmd == 'A') {
      dx = -1;
    } else if (cmd == 'd' || cmd == 'D') {
      dx = 1;
    } else {
      continue;
    }

    if (!move_hero(&map, dx, dy, &distance)) {
      return;
    }
  }
}
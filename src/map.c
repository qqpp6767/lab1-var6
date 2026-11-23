#include "map.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Название биома для вывода на экран
static const char *get_biome_name(enum Biome biome) {
  switch (biome) {
  case BIOME_LIGHT_FOREST:
    return "Редколесье";
  case BIOME_SWAMP:
    return "Топь";
  case BIOME_STONE_RUINS:
    return "Каменные руины";
  case BIOME_CRYSTAL_CAVE:
    return "Кристальная пещера";
  default:
    return "Неизвестно";
  }
}

// Случайный выбор биома и его параметров
static MapInfo generate_map_info(void) {
  MapInfo info;
  info.biome = (enum Biome)(rand() % BIOME_COUNT);
  info.is_mapped = 0;

  switch (info.biome) {
  case BIOME_LIGHT_FOREST:
    info.obstacle_chance = 15;
    info.gold_multiplier = 1;
    info.step_stamina_cost = 1;
    break;
  case BIOME_SWAMP:
    info.obstacle_chance = 20;
    info.gold_multiplier = 1;
    info.step_stamina_cost = 2;
    break;
  case BIOME_STONE_RUINS:
    info.obstacle_chance = 35;
    info.gold_multiplier = 2;
    info.step_stamina_cost = 1;
    break;
  case BIOME_CRYSTAL_CAVE:
    info.obstacle_chance = 25;
    info.gold_multiplier = 3;
    info.step_stamina_cost = 1;
    break;
  default:
    info.obstacle_chance = 20;
    info.gold_multiplier = 1;
    info.step_stamina_cost = 1;
    break;
  }

  return info;
}

// Поиск случайной свободной клетки '.'
static int random_free_cell(const Map *map, int *cell_x, int *cell_y) {
  int free_cells = 0;
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      if (map->grid[y][x] == CELL_EMPTY) {
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
      if (map->grid[y][x] == CELL_EMPTY) {
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
Map *create_map(int floor_num) {
  Map *map = malloc(sizeof(Map));
  if (map == NULL) {
    return NULL;
  }

  map->width = rand() % MAP_SIZE_RANGE + MAP_MIN_SIZE;
  map->height = rand() % MAP_SIZE_RANGE + MAP_MIN_SIZE;
  map->floor = floor_num;
  map->info = generate_map_info();

  map->grid = malloc(map->height * sizeof(char *));
  if (map->grid == NULL) {
    free_map(map);
    return NULL;
  }

  int free_cells = 0;
  for (int y = 0; y < map->height; y++) {
    map->grid[y] = malloc(map->width + 1);
    if (map->grid[y] == NULL) {
      map->height = y;
      free_map(map);
      return NULL;
    }

    for (int x = 0; x < map->width; x++) {
      if (rand() % 100 < map->info.obstacle_chance) {
        map->grid[y][x] = (rand() % 2 == 0) ? CELL_TREE : CELL_ROCK;
      } else {
        map->grid[y][x] = CELL_EMPTY;
        free_cells++;
      }
    }
    map->grid[y][map->width] = '\0';
  }

  // Гарантируем минимум 2 свободные клетки
  for (int y = 0; y < map->height && free_cells < 2; y++) {
    for (int x = 0; x < map->width && free_cells < 2; x++) {
      if (map->grid[y][x] != CELL_EMPTY) {
        map->grid[y][x] = CELL_EMPTY;
        free_cells++;
      }
    }
  }

  random_free_cell(map, &map->camp_x, &map->camp_y);
  map->grid[map->camp_y][map->camp_x] = CELL_CAMP;

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

// Вывод строки состояния (HUD)
static void print_hud(const Map *map, const Player *player) {
  printf("\nHP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d (%s) | "
         "Время: %02d:00\n",
         player->hp, player->max_hp, player->stamina, player->max_stamina,
         player->gold, map->floor, get_biome_name(map->info.biome),
         player->hour);
}

// Отрисовка карты с туманом войны
void print_map(const Map *map, const Player *player) {
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      int dx = x - map->scream_x;
      int dy = y - map->scream_y;

      if (x == player->x && y == player->y) {
        putchar(CELL_HERO);
      } else if (map->scream_steps > 0 &&
                 dx * dx + dy * dy <= map->scream_radius * map->scream_radius) {
        putchar(map->grid[y][x]);
      } else {
        putchar(CELL_FOG);
      }
    }
    putchar('\n');
  }
}

// Проверка допуска к походу
static int can_start_adventure(const Player *player) {
  if (!check_player_exists(player)) {
    return 0;
  }
  if (player->stamina <= 0) {
    printf("Выносливость равна 0! Сначала отдохните.\n");
    return 0;
  }
  if (player->hour == NIGHT_HOUR) {
    printf("На часах %02d:00 ночи! Слишком опасно идти в лес, пора спать.\n",
           NIGHT_HOUR);
    return 0;
  }
  return 1;
}

// Обработка обморока и эвакуации
static void handle_faint(Map *map, Player *player) {
  int lost_gold = player->gold - (player->gold / 2);
  player->gold /= 2;

  printf("\nВы упали без сил! Спасательная служба доставила вас домой.\n");
  printf(
      "За услуги спасения забрали половину золота (-%d зол.). Осталось: %d\n",
      lost_gold, player->gold);

  free_map(map);
}

// Активация крика (подсветка области вокруг героя)
static void activate_scream(Map *map, const Player *player) {
  map->scream_x = player->x;
  map->scream_y = player->y;
  map->scream_radius = rand() % 3 + 5;
  map->scream_steps = rand() % 2 + 2;
}

// Зарисовка текущего этажа за 5 ед. выносливости
static void sketch_current_floor(Map *map, Player *player) {
  if (map->info.is_mapped) {
    printf("Этот этаж уже зарисован!\n");
    return;
  }

  if (player->stamina < MAP_SKETCH_STAMINA_COST) {
    printf("Недостаточно выносливости для зарисовки карты (нужно %d ед.)!\n",
           MAP_SKETCH_STAMINA_COST);
    return;
  }

  player->stamina -= MAP_SKETCH_STAMINA_COST;
  map->info.is_mapped = 1;
  player->sketched_maps++;

  printf("Вы зарисовали этаж (%s)! Карт с собой: %d\n",
         get_biome_name(map->info.biome), player->sketched_maps);
}

// Переход на новый этаж / участок карты
static int switch_to_new_floor(Map **map_ptr, Player *player, int next_x,
                               int next_y) {
  Map *old_map = *map_ptr;
  int old_w = old_map->width;
  int old_h = old_map->height;
  int next_floor = old_map->floor + 1;

  free_map(old_map);

  Map *new_map = create_map(next_floor);
  if (new_map == NULL) {
    *map_ptr = NULL;
    return 0;
  }

  if (next_x < 0) {
    player->x = new_map->width - 1;
    player->y = (player->y < new_map->height) ? player->y : new_map->height / 2;
  } else if (next_x >= old_w) {
    player->x = 0;
    player->y = (player->y < new_map->height) ? player->y : new_map->height / 2;
  } else if (next_y < 0) {
    player->y = new_map->height - 1;
    player->x = (player->x < new_map->width) ? player->x : new_map->width / 2;
  } else if (next_y >= old_h) {
    player->y = 0;
    player->x = (player->x < new_map->width) ? player->x : new_map->width / 2;
  }

  if (new_map->grid[player->y][player->x] != CELL_CAMP) {
    new_map->grid[player->y][player->x] = CELL_EMPTY;
  }

  // Трата сил, времени и награда с учётом множителя биома
  player->stamina -= FLOOR_STAMINA_COST;
  if (player->stamina < 0) {
    player->stamina = 0;
  }
  advance_time(&player->day, &player->hour, FLOOR_TIME_COST);

  int reward =
      BASE_GOLD_MULTIPLIER * new_map->floor * new_map->info.gold_multiplier;
  player->gold += reward;

  printf("Этаж %d (%s, шаг: %d вын., золото: x%d). Награда: +%d золота.\n",
         new_map->floor, get_biome_name(new_map->info.biome),
         new_map->info.step_stamina_cost, new_map->info.gold_multiplier,
         reward);

  *map_ptr = new_map;
  return 1;
}

// Шаг героя по карте
static int move_hero(Map **map_ptr, Player *player, int dx, int dy) {
  Map *map = *map_ptr;
  int next_x = player->x + dx;
  int next_y = player->y + dy;

  if (next_x < 0 || next_x >= map->width || next_y < 0 ||
      next_y >= map->height) {
    return switch_to_new_floor(map_ptr, player, next_x, next_y);
  }

  if (map->grid[next_y][next_x] == CELL_TREE ||
      map->grid[next_y][next_x] == CELL_ROCK) {
    return 1;
  }

  player->x = next_x;
  player->y = next_y;
  player->stamina -= map->info.step_stamina_cost;
  if (player->stamina < 0) {
    player->stamina = 0;
  }

  if (map->scream_steps > 0) {
    map->scream_steps--;
  }
  return 1;
}

// Преобразование символа ввода в направление
static enum Direction parse_command(char cmd) {
  switch (cmd) {
  case 'w':
  case 'W':
    return DIR_UP;
  case 's':
  case 'S':
    return DIR_DOWN;
  case 'a':
  case 'A':
    return DIR_LEFT;
  case 'd':
  case 'D':
    return DIR_RIGHT;
  case 'r':
  case 'R':
    return DIR_SCREAM;
  case 'm':
  case 'M':
    return DIR_SKETCH;
  case 'q':
  case 'Q':
    return DIR_QUIT;
  default:
    return DIR_NONE;
  }
}

// Главный цикл приключения
void travel(Player *player) {
  if (!can_start_adventure(player)) {
    return;
  }

  Map *map = create_map(1);
  if (map == NULL) {
    return;
  }

  player->x = map->camp_x;
  player->y = map->camp_y;

  printf("Вы вошли в локацию: %s (шаг: %d вын., множитель золота: x%d).\n",
         get_biome_name(map->info.biome), map->info.step_stamina_cost,
         map->info.gold_multiplier);

  while (1) {
    print_hud(map, player);
    print_map(map, player);
    printf("WASD - шаг, R - осмотреться, M - зарисовать (5 вын.), Q - выход "
           "('^') > ");

    char raw_cmd;
    if (!input_char(&raw_cmd)) {
      continue;
    }

    enum Direction dir = parse_command(raw_cmd);
    if (dir == DIR_QUIT) {
      if (player->x == map->camp_x && player->y == map->camp_y) {
        printf("Вы благополучно вернулись домой с %d золота!\n", player->gold);
        free_map(map);
        return;
      }
      printf("Выйти можно только с клетки лагеря ('^')!\n");
      continue;
    }

    if (dir == DIR_SCREAM) {
      activate_scream(map, player);
      continue;
    }

    if (dir == DIR_SKETCH) {
      sketch_current_floor(map, player);
    } else {
      int dx = 0, dy = 0;
      if (dir == DIR_UP) {
        dy = -1;
      } else if (dir == DIR_DOWN) {
        dy = 1;
      } else if (dir == DIR_LEFT) {
        dx = -1;
      } else if (dir == DIR_RIGHT) {
        dx = 1;
      } else {
        continue;
      }

      if (!move_hero(&map, player, dx, dy)) {
        return;
      }
    }

    // Проверка на обморок (кончилась выносливость или наступило 02:00 ночи)
    if (player->stamina <= 0 || player->hour == NIGHT_HOUR) {
      handle_faint(map, player);
      return;
    }
  }
}
#include "player.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Динамическое создание игрока
Player *create_player(void) {
  Player *player = malloc(sizeof(Player));
  if (player == NULL) {
    printf("Ошибка выделения памяти под персонажа\n");
    return NULL;
  }

  player->day = 1;
  player->hour = 8;
  player->inventory_size = MAX_ITEMS;
  for (int i = 0; i < player->inventory_size; i++) {
    player->inventory[i] = 0;
  }

  printf("Введи имя фермера: ");
  read_string(player->name, sizeof(player->name));
  printf("Привет, %s!\n", player->name);

  return player;
}

// Освобождение памяти игрока
void free_player(Player *player) {
  if (player == NULL) {
    return;
  }
  uintptr_t addr = (uintptr_t)player;
  free(player);
  printf("Память структуры по адресу %p освобождена\n", (void *)addr);
}

// Показать время игрока
void show_player_time(const Player *player) {
  print_time(player->day, player->hour);
}

// Работа и продвижение времени
void player_work(Player *player) {
  int work_hours;
  printf("Сколько часов работать?: ");
  if (!input_int(&work_hours)) {
    return;
  }

  if (work_hours < 0) {
    printf("Так не работает (к сожалению)\n");
    return;
  }

  advance_time(&player->day, &player->hour, work_hours);
}

// Вывод содержимого рюкзака
void check_inventory(const Player *player,
                     char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  for (int i = 0; i < player->inventory_size; i++) {
    int id = player->inventory[i];
    printf("Слот %d: [%d] (%s)\n", i, id, item_names[id]);
  }
}

// Положить предмет в выбранный слот
void put_item_in_slot(Player *player,
                      char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  int slot, item_id;

  printf("Введи номер слота (0-9): ");
  if (!input_int(&slot)) {
    return;
  }
  if (slot < 0 || slot >= player->inventory_size) {
    printf("Такого слота нет (0-9)\n");
    return;
  }

  printf("Введи ID предмета (1-9): ");
  if (!input_int(&item_id)) {
    return;
  }
  if (item_id == 0) {
    printf("Нельзя удалить предмет\n");
    return;
  }
  if (item_id < 1 || item_id >= MAX_ITEMS) {
    printf("Такого ID нет (1-9)\n");
    return;
  }

  player->inventory[slot] = item_id;
  printf("В слот %d помещен предмет: [%d] (%s)\n", slot, item_id,
         item_names[item_id]);
}

// Выбросить предмет из слота
void drop_item(Player *player,
               char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  int slot;
  printf("Введи Слот (0-9): ");
  if (!input_int(&slot)) {
    return;
  }
  if (slot < 0 || slot >= player->inventory_size) {
    printf("Такого слота нет (0-9)\n");
    return;
  }

  int item_id = player->inventory[slot];
  player->inventory[slot] = 0;
  printf("Ты выкинул [%s] из слота %d\n", item_names[item_id], slot);
}

// Поиск предметов с ID больше заданного
void check_weights(const Player *player,
                   char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  int item_id;
  printf("Введи ID предмета: ");
  if (!input_int(&item_id)) {
    return;
  }

  int found = 0;
  printf("Слоты, ID которых больше %d:\n", item_id);
  for (int i = 0; i < player->inventory_size; i++) {
    if (player->inventory[i] > item_id) {
      int id = player->inventory[i];
      printf("- Слот %d: [%d] (%s)\n", i, id, item_names[id]);
      found = 1;
    }
  }

  if (!found) {
    printf("Не найдено\n");
  }
}

// Поиск предмета в рюкзаке по названию
void find_item_by_name(const Player *player,
                       char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  char search_name[MAX_ITEM_NAME_LEN];
  printf("Введи название предмета для поиска: ");
  read_string(search_name, sizeof(search_name));

  int target_id = -1;
  for (int i = 0; i < MAX_ITEMS; i++) {
    if (strcmp(item_names[i], search_name) == 0) {
      target_id = i;
      break;
    }
  }

  if (target_id == -1) {
    printf("Предмета [%s] нет в каталоге!\n", search_name);
    return;
  }

  int found = 0;
  printf("Поиск %s (ID %d) в инвентаре...\n", search_name, target_id);
  for (int i = 0; i < player->inventory_size; i++) {
    if (player->inventory[i] == target_id) {
      printf("- Слот [%d]\n", i);
      found = 1;
    }
  }

  if (!found) {
    printf("В рюкзаке предмета [%s] нет.\n", search_name);
  }
}

// Запись состояния игрока в дневник
void player_save_diary(const Player *player,
                       char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  save_to_diary(player->name, player->day, player->hour, player->inventory,
                player->inventory_size, item_names);
}

// Проверка доступных рецептов крафта
void player_check_recipes(const Player *player,
                          char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  check_crafting_recipes(player->inventory, player->inventory_size, item_names);
}
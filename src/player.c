#include "player.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Динамическое создание персонажа
Player *create_player(void) {
  Player *player = malloc(sizeof(Player));
  if (player == NULL) {
    printf("Ошибка выделения памяти под персонажа\n");
    return NULL;
  }

  player->hp = START_HP;
  player->max_hp = START_HP;
  player->stamina = START_STAMINA;
  player->max_stamina = START_STAMINA;
  player->gold = START_GOLD;
  player->sketched_maps = 0;
  player->x = 0;
  player->y = 0;
  player->day = START_DAY;
  player->hour = START_HOUR;
  player->inventory_size = MAX_ITEMS;

  for (int i = 0; i < player->inventory_size; i++) {
    player->inventory[i] = 0;
  }

  printf("Введи имя фермера: ");
  read_string(player->name, sizeof(player->name));
  if (player->name[0] == '\0') {
    strcpy(player->name, "Фермер");
  }
  printf("Персонаж %s создан!\n", player->name);

  return player;
}

// Создание или пересоздание персонажа с подтверждением
void recreate_player(Player **player) {
  if (*player != NULL) {
    int confirm = 0;
    printf("Персонаж %s уже существует! Точно удалить и создать нового? "
           "(1 - Да, 0 - Нет): ",
           (*player)->name);
    if (!input_int(&confirm) || confirm != 1) {
      printf("Пересоздание отменено.\n");
      return;
    }
    free_player(*player);
    *player = NULL;
  }

  *player = create_player();
}

// Освобождение памяти персонажа
void free_player(Player *player) {
  if (player == NULL) {
    return;
  }
  uintptr_t addr = (uintptr_t)player;
  free(player);
  printf("Память структуры по адресу %p освобождена\n", (void *)addr);
}

// Проверка, создан ли персонаж
int check_player_exists(const Player *player) {
  if (player == NULL) {
    printf("Персонаж ещё не создан! Сначала выберите пункт [1].\n");
    return 0;
  }
  return 1;
}

// Карточка персонажа
void print_player_card(const Player *player,
                       char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  if (!check_player_exists(player)) {
    return;
  }

  printf("\n══════════ КАРТОЧКА ПЕРСОНАЖА ══════════\n");
  printf("Имя:             %s\n", player->name);
  printf("Здоровье:        %d / %d\n", player->hp, player->max_hp);
  printf("Выносливость:    %d / %d\n", player->stamina, player->max_stamina);
  printf("Золото:          %d\n", player->gold);
  printf("Зарисовано карт: %d\n", player->sketched_maps);
  printf("Дата и время:    День %d, %02d:00\n", player->day, player->hour);
  printf("────────────────────────────────────────\n");
  printf("Инвентарь:\n");
  check_inventory(player, item_names);
  printf("════════════════════════════════════════\n\n");
}

// Пропустить время (Ждать)
static void wait_hours_action(Player *player) {
  int max_wait = get_hours_until_night(player->hour);
  if (max_wait == 0) {
    printf("Уже %02d:00 ночи! Фермеру пора спать.\n", NIGHT_HOUR);
    return;
  }

  int hours = 0;
  printf("Сколько часов подождать? (от 1 до %d): ", max_wait);
  if (!input_int(&hours)) {
    return;
  }

  if (hours < 1) {
    printf("Нужно подождать хотя бы 1 час.\n");
    return;
  }

  if (hours > max_wait) {
    printf("Нельзя прокрутить время дальше %02d:00 ночи без сна! "
           "(Доступно максимум %d ч.)\n",
           NIGHT_HOUR, max_wait);
    return;
  }

  advance_time(&player->day, &player->hour, hours);
  print_time(player->day, player->hour);
}

// Отдых (Лечь спать)
static void sleep_action(Player *player) {
  player->day += 1;
  player->hour = START_HOUR;
  player->hp = player->max_hp;
  player->stamina = player->max_stamina;

  printf("Фермер выспался! Наступил День %d, %02d:00.\n", player->day,
         player->hour);
  printf("Здоровье (%d/%d) и выносливость (%d/%d) полностью восстановлены!\n",
         player->hp, player->max_hp, player->stamina, player->max_stamina);
}

// Подменю управления временем и режимом дня
void manage_time_menu(Player *player) {
  if (!check_player_exists(player)) {
    return;
  }

  int choice = -1;
  do {
    printf("\n─── Режим дня и время ───\n"
           "1 - Посмотреть на часы\n"
           "2 - Пропустить время (Ждать)\n"
           "3 - Отдых (Лечь спать)\n"
           "0 - Назад в главное меню\n"
           "Выбор: ");

    if (!input_int(&choice)) {
      continue;
    }

    switch (choice) {
    case TIME_VIEW:
      print_time(player->day, player->hour);
      break;
    case TIME_WAIT:
      wait_hours_action(player);
      break;
    case TIME_SLEEP:
      sleep_action(player);
      break;
    case TIME_BACK:
      break;
    default:
      printf("Неверный пункт (0-3)\n");
      break;
    }
  } while (choice != TIME_BACK);
}

// Продажа зарисованных карт в главном меню
void sell_sketched_maps(Player *player) {
  if (!check_player_exists(player)) {
    return;
  }

  if (player->sketched_maps <= 0) {
    printf("У вас нет зарисованных карт для продажи.\n");
    return;
  }

  int count = player->sketched_maps;
  int bonus_gold = count * MAP_SELL_PRICE;
  player->gold += bonus_gold;
  player->sketched_maps = 0;

  printf("Вы продали зарисованные карты (%d шт.) и получили %d бонусного "
         "золота! Всего золота: %d\n",
         count, bonus_gold, player->gold);
}

// Вывод содержимого рюкзака
void check_inventory(const Player *player,
                     char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  if (!check_player_exists(player)) {
    return;
  }

  for (int i = 0; i < player->inventory_size; i++) {
    int id = player->inventory[i];
    printf("  Слот %d: [%d] (%s)\n", i, id, item_names[id]);
  }
}

// Положить предмет в выбранный слот
void put_item_in_slot(Player *player,
                      char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  if (!check_player_exists(player)) {
    return;
  }

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
    printf("Нельзя положить пустоту\n");
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
void drop_item(Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  if (!check_player_exists(player)) {
    return;
  }

  int slot;
  printf("Введи слот (0-9): ");
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
  if (!check_player_exists(player)) {
    return;
  }

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
  if (!check_player_exists(player)) {
    return;
  }

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
  if (!check_player_exists(player)) {
    return;
  }

  save_to_diary(player->name, player->day, player->hour, player->hp,
                player->max_hp, player->stamina, player->max_stamina,
                player->gold, player->inventory, player->inventory_size,
                item_names);
}

// Проверка доступных рецептов крафта
void player_check_recipes(const Player *player,
                          char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  if (!check_player_exists(player)) {
    return;
  }

  check_crafting_recipes(player->inventory, player->inventory_size, item_names);
}
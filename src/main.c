#include "map.h"
#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void print_menu(void) {
  printf("────────────────\n"
         "0 - Выход\n"
         "1 - Посмотреть на часы\n"
         "2 - Поработать\n"
         "3 - Посмотреть инвентарь\n"
         "4 - Положить предмет в слот\n"
         "5 - Выбросить предмет\n"
         "6 - Найти тяжести\n"
         "7 - Поиск предмета в рюкзаке по названию\n"
         "8 - Записать состояние в дневник фермера\n"
         "9 - Расшифровать старые записи\n"
         "10 - Отправиться в путешествие\n"
         "11 - Пересоздать фермера\n"
         "Введи № действия (0-11): ");
}

static void recreate_player(Player **player) {
  free_player(*player);
  *player = create_player();
}

static void handle_menu_choice(int choice, Player **player,
                               char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  switch (choice) {
  case 1:
    show_player_time(*player);
    break;
  case 2:
    player_work(*player);
    break;
  case 3:
    check_inventory(*player, item_names);
    break;
  case 4:
    put_item_in_slot(*player, item_names);
    break;
  case 5:
    drop_item(*player, item_names);
    break;
  case 6:
    check_weights(*player, item_names);
    break;
  case 7:
    find_item_by_name(*player, item_names);
    break;
  case 8:
    player_save_diary(*player, item_names);
    break;
  case 9:
    player_check_recipes(*player, item_names);
    break;
  case 10:
    travel();
    break;
  case 11:
    recreate_player(player);
    break;
  case 0:
    printf("Ну пока\n");
    break;
  default:
    printf("Ты че? только 0-11\n");
    break;
  }
}

int main(void) {
  srand((unsigned int)time(NULL));

  char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN];
  load_items(item_names);

  Player *player = create_player();
  if (player == NULL) {
    return 1;
  }

  int task_num = -1;
  do {
    print_menu();
    if (!input_int(&task_num)) {
      continue;
    }
    handle_menu_choice(task_num, &player, item_names);
  } while (task_num != 0 && player != NULL);

  free_player(player);
  return 0;
}
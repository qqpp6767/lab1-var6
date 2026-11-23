#include "map.h"
#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum MenuOption {
  MENU_EXIT = 0,
  MENU_CREATE_PLAYER,
  MENU_PLAYER_CARD,
  MENU_TIME,
  MENU_INVENTORY,
  MENU_PUT_ITEM,
  MENU_DROP_ITEM,
  MENU_WEIGHTS,
  MENU_FIND_ITEM,
  MENU_DIARY,
  MENU_ADVENTURE,
  MENU_CRAFT
};

static void print_menu(void) {
  printf("────────────────\n"
         "0 - Выход\n"
         "1 - Создать / пересоздать персонажа\n"
         "2 - Карточка персонажа\n"
         "3 - Время и отдых\n"
         "4 - Посмотреть инвентарь\n"
         "5 - Положить предмет в слот\n"
         "6 - Выбросить предмет\n"
         "7 - Найти тяжести\n"
         "8 - Поиск предмета в рюкзаке по названию\n"
         "9 - Записать состояние в дневник фермера\n"
         "10 - Отправиться в приключение\n"
         "11 - Расшифровать старые записи\n"
         "Введи № действия (0-11): ");
}

static void handle_menu_choice(int choice, Player **player,
                               char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  switch (choice) {
  case MENU_CREATE_PLAYER:
    recreate_player(player);
    break;
  case MENU_PLAYER_CARD:
    print_player_card(*player, item_names);
    break;
  case MENU_TIME:
    manage_time_menu(*player);
    break;
  case MENU_INVENTORY:
    check_inventory(*player, item_names);
    break;
  case MENU_PUT_ITEM:
    put_item_in_slot(*player, item_names);
    break;
  case MENU_DROP_ITEM:
    drop_item(*player, item_names);
    break;
  case MENU_WEIGHTS:
    check_weights(*player, item_names);
    break;
  case MENU_FIND_ITEM:
    find_item_by_name(*player, item_names);
    break;
  case MENU_DIARY:
    player_save_diary(*player, item_names);
    break;
  case MENU_ADVENTURE:
    travel(*player);
    break;
  case MENU_CRAFT:
    player_check_recipes(*player, item_names);
    break;
  case MENU_EXIT:
    printf("Ну пока!\n");
    break;
  default:
    printf("Неверный пункт меню (0-11)\n");
    break;
  }
}

int main(void) {
  srand((unsigned int)time(NULL));

  char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN];
  load_items(item_names);

  Player *player = NULL;
  int task_num = -1;

  do {
    print_menu();
    if (!input_int(&task_num)) {
      continue;
    }
    handle_menu_choice(task_num, &player, item_names);
  } while (task_num != MENU_EXIT);

  if (player != NULL) {
    free_player(player);
  }

  return 0;
}
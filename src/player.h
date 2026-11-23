#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

#define START_HP 100
#define START_STAMINA 50
#define START_GOLD 0
#define MAP_SELL_PRICE 25

typedef struct {
  char name[MAX_NAME_LEN + 1];
  int hp;
  int max_hp;
  int stamina;
  int max_stamina;
  int gold;
  int sketched_maps;
  int x;
  int y;
  int day;
  int hour;
  int inventory[MAX_ITEMS];
  int inventory_size;
} Player;

// Жизненный цикл персонажа
Player *create_player(void);
void recreate_player(Player **player);
void free_player(Player *player);
int check_player_exists(const Player *player);

// Карточка, режим дня и продажа карт
void print_player_card(const Player *player,
                       char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void manage_time_menu(Player *player);
void sell_sketched_maps(Player *player);

// Инвентарь
void check_inventory(const Player *player,
                     char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void put_item_in_slot(Player *player,
                      char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void drop_item(Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void check_weights(const Player *player,
                   char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void find_item_by_name(const Player *player,
                       char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);

// Файловые операции игрока
void player_save_diary(const Player *player,
                       char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void player_check_recipes(const Player *player,
                          char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);

#endif
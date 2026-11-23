#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

typedef struct {
  char name[MAX_NAME_LEN + 1];
  int day;
  int hour;
  int inventory[MAX_ITEMS];
  int inventory_size;
} Player;

// Создание и удаление структуры игрока
Player *create_player(void);
void free_player(Player *player);

// Статы и время
void show_player_time(const Player *player);
void player_work(Player *player);

// Инвентарь
void check_inventory(const Player *player,
                     const char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void put_item_in_slot(Player *player,
                      const char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void drop_item(Player *player,
               const char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void check_weights(const Player *player,
                   const char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void find_item_by_name(const Player *player,
                       const char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);

// Файловые операции игрока
void player_save_diary(const Player *player,
                       const char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void player_check_recipes(const Player *player,
                          const char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);

#endif
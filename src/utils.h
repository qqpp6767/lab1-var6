#ifndef UTILS_H
#define UTILS_H

#ifndef DATA_DIR
#define DATA_DIR "data/"
#endif
#define DATA_FILE(name) (DATA_DIR name)

#define MAX_ITEMS 10
#define MAX_ITEM_NAME_LEN 32
#define MAX_NAME_LEN 32

#define START_DAY 1
#define START_HOUR 8
#define NIGHT_HOUR 2
#define HOURS_IN_DAY 24

#define ITEM_EMPTY "Пусто"
#define ITEM_WOOD "Дерево"
#define ITEM_STONE "Камень"
#define ITEM_SEEDS "Семена"
#define ITEM_GRASS "Трава"
#define ITEM_MUSHROOM "Гриб"
#define ITEM_IRON "Железо"
#define ITEM_SAND "Песок"
#define ITEM_COAL "Уголь"
#define ITEM_GOLD "Золото"

enum TimeAction { TIME_BACK = 0, TIME_VIEW, TIME_WAIT, TIME_SLEEP };

// Ввод и строки
void trim_newline(char *str);
void read_string(char *buffer, int size);
int input_int(int *var);
int input_char(char *var);

// Время
int get_hours_until_night(int current_hour);
void advance_time(int *day, int *hour, int hours_to_add);
void print_time(int day, int hour);

// Файлы
void load_items(char names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void save_to_diary(const char *player_name, int day, int hour, int hp,
                   int max_hp, int stamina, int max_stamina, int gold,
                   const int *inventory, int inventory_size,
                   char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void check_crafting_recipes(const int *inventory, int inventory_size,
                            char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);

#endif
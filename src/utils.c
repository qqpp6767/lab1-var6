#include "utils.h"
#include <stdio.h>
#include <string.h>

// Удаление символов переноса строки в конце
void trim_newline(char *str) {
  if (str == NULL) {
    return;
  }
  size_t len = strlen(str);
  while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
    str[len - 1] = '\0';
    len--;
  }
}

// Безопасное чтение строки из консоли
void read_string(char *buffer, int size) {
  if (fgets(buffer, size, stdin) == NULL) {
    buffer[0] = '\0';
    return;
  }
  if (strchr(buffer, '\n') == NULL) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
  }
  trim_newline(buffer);
}

// Ввод целого числа с очисткой буфера
int input_int(int *var) {
  int ch;
  if (scanf("%d", var) != 1) {
    printf("Нужно ввести число!\n");
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
    *var = -1;
    return 0;
  }
  while ((ch = getchar()) != '\n' && ch != EOF) {
  }
  return 1;
}

// Ввод одного символа с очисткой буфера
int input_char(char *var) {
  int ch;
  if (scanf(" %c", var) != 1) {
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
    return 0;
  }
  while ((ch = getchar()) != '\n' && ch != EOF) {
  }
  return 1;
}

// Сколько часов осталось до ближайших 02:00 ночи
int get_hours_until_night(int current_hour) {
  if (current_hour == NIGHT_HOUR) {
    return 0;
  }
  if (current_hour > NIGHT_HOUR) {
    return (HOURS_IN_DAY - current_hour) + NIGHT_HOUR;
  }
  return NIGHT_HOUR - current_hour;
}

// Перевод времени вперед
void advance_time(int *day, int *hour, int hours_to_add) {
  *hour += hours_to_add;
  while (*hour >= HOURS_IN_DAY) {
    (*day)++;
    *hour -= HOURS_IN_DAY;
  }
}

// Вывод текущего времени
void print_time(int day, int hour) {
  printf("Текущее время: День %d, %02d:00\n", day, hour);
}

// Загрузка каталога предметов из файла
void load_items(char names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  FILE *file = fopen(DATA_FILE("items.txt"), "r");
  if (file == NULL) {
    printf("items.txt не найден, загружены стандартные названия\n");
    strcpy(names[0], ITEM_EMPTY);
    strcpy(names[1], ITEM_WOOD);
    strcpy(names[2], ITEM_STONE);
    strcpy(names[3], ITEM_SEEDS);
    strcpy(names[4], ITEM_GRASS);
    strcpy(names[5], ITEM_MUSHROOM);
    strcpy(names[6], ITEM_IRON);
    strcpy(names[7], ITEM_SAND);
    strcpy(names[8], ITEM_COAL);
    strcpy(names[9], ITEM_GOLD);
    return;
  }

  int id;
  char name[MAX_ITEM_NAME_LEN];
  while (fscanf(file, "%d %31s", &id, name) == 2) {
    if (id >= 0 && id < MAX_ITEMS) {
      strcpy(names[id], name);
    }
  }
  fclose(file);
}

// Сохранение текущего состояния в дневник
void save_to_diary(const char *player_name, int day, int hour, int hp,
                   int max_hp, int stamina, int max_stamina, int gold,
                   const int *inventory, int inventory_size,
                   char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  FILE *file = fopen(DATA_FILE("diary.txt"), "a");
  if (file == NULL) {
    printf("Не удалось открыть файл diary.txt\n");
    return;
  }

  fprintf(file, "────────── Запись в дневнике ──────────\n");
  fprintf(file, "Фермер: %s\n", player_name);
  fprintf(file, "Время: День %d, %02d:00\n", day, hour);
  fprintf(file, "HP: %d/%d | Выносливость: %d/%d | Золото: %d\n", hp, max_hp,
          stamina, max_stamina, gold);
  fprintf(file, "Содержимое рюкзака:\n");
  for (int i = 0; i < inventory_size; i++) {
    fprintf(file, "  Слот %d: [%d] %s\n", i, inventory[i],
            item_names[inventory[i]]);
  }
  fprintf(file, "───────────────────────────────────────\n\n");

  fclose(file);
  printf("Запись успешно добавлена в diary.txt!\n");
}

// Проверка рецептов крафта по файлам input.txt и output.txt
void check_crafting_recipes(const int *inventory, int inventory_size,
                            char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
  FILE *f_in = fopen(DATA_FILE("input.txt"), "r");
  if (f_in == NULL) {
    printf("Файл input.txt не найден\n");
    return;
  }

  FILE *f_out = fopen(DATA_FILE("output.txt"), "w");
  if (f_out == NULL) {
    printf("Не удалось создать output.txt\n");
    fclose(f_in);
    return;
  }

  int available[MAX_ITEMS] = {0};
  for (int i = 0; i < inventory_size; i++) {
    if (inventory[i] > 0 && inventory[i] < MAX_ITEMS) {
      available[inventory[i]]++;
    }
  }

  printf("\n=== РЕЗУЛЬТАТ ПРОВЕРКИ КРАФТА ===\n");
  fprintf(f_out, "=== РЕЗУЛЬТАТ ПРОВЕРКИ КРАФТА ===\n");

  char line[256];
  while (fgets(line, sizeof(line), f_in) != NULL) {
    int colon_index = -1;
    for (int i = 0; line[i] != '\0'; i++) {
      if (line[i] == ':') {
        line[i] = '\0';
        colon_index = i;
        break;
      }
    }
    if (colon_index == -1) {
      continue;
    }

    char *recipe_name = line;
    int required[MAX_ITEMS] = {0};
    for (int i = colon_index + 1; line[i] != '\0'; i++) {
      if (line[i] >= '0' && line[i] <= '9') {
        required[line[i] - '0']++;
      }
    }

    int can_craft = 1;
    for (int id = 1; id < MAX_ITEMS; id++) {
      if (available[id] < required[id]) {
        can_craft = 0;
        break;
      }
    }

    if (can_craft) {
      printf("%s: [ДОСТУПНО]\n", recipe_name);
      fprintf(f_out, "%s: [ДОСТУПНО]\n", recipe_name);
    } else {
      printf("%s: [НЕ ХВАТАЕТ РЕСУРСОВ] (Не хватает: ", recipe_name);
      fprintf(f_out, "%s: [НЕ ХВАТАЕТ РЕСУРСОВ] (Не хватает: ", recipe_name);

      int first = 1;
      for (int id = 1; id < MAX_ITEMS; id++) {
        if (available[id] < required[id]) {
          int missing = required[id] - available[id];
          if (!first) {
            printf(", ");
            fprintf(f_out, ", ");
          }
          printf("%s x%d", item_names[id], missing);
          fprintf(f_out, "%s x%d", item_names[id], missing);
          first = 0;
        }
      }
      printf(")\n");
      fprintf(f_out, ")\n");
    }
  }

  fclose(f_in);
  fclose(f_out);
  printf("Сохранено в output.txt\n\n");
}
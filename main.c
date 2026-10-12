#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 10
#define MAX_ITEM_NAME_LEN 32
#define MAX_NAME_LEN 32


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


// Переменные
int current_day = 1;
int current_hour = 8;
int inventory[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN];
int inventory_size = sizeof(inventory) / sizeof(inventory[0]);

char username[MAX_NAME_LEN + 1];
int work_hours;
int slot,item_id;

// Декларации функций
int input(int *var);
int check_inventory(int *inventory, int inventory_size);
int check_weights(int item_id, int inventory_size);
void load_items(char names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void find_by_name(const char *search_name);
void save_to_diary(void);
void check_crafting_recipes(void);

int main()
{
  load_items(item_names);
  int taskNum;
  printf("Введи имя фермера: ");
  fgets(username, sizeof(username), stdin);
  printf("Привет, %s", username);
  do 
  {
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
           "Введи № действия (1-9): ");

    if(!input(&taskNum)) continue;
    switch (taskNum)
    {
    case 1: // посмотреть время
      
      printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
      break;

    case 2: // поработать
      
      printf("Сколько часов работать?: ");
      if (!input(&work_hours)) break;

      if(work_hours < 0)
      {
        printf("Так не работает (к сожалению)\n");
        continue;
      }
      // перевод времени
      current_hour += work_hours;
      while (current_hour >= 24)
      {
        current_day++;
        current_hour -= 24;
      }
      break;

    case 3: // чекнуть инвентарь
      
      check_inventory(inventory, inventory_size);
      break;

    case 4: // положить предмет в слот

      printf("Введи номер слота (0-9): ");
      if (!input(&slot)) break;

      if (slot < 0 || slot > 9) 
      {
        printf("Такого слота нет (0-9)\n");
        continue;
      }

      printf("Введи ID предмета (1-9): ");
      if (!input(&item_id)) break;

      if (item_id == 0)
      {
        printf("Нельзя удалить предмет\n");
        continue;
      }
      else if (item_id < 1 || item_id > 9)
      {
        printf("Такого ID нет (1-9)\n");
        continue;
      }

      inventory[slot] = item_id;
      printf("В слот %d помещен предмет: [%d] (%s)\n", slot, item_id,item_names[item_id]);
      break;

    case 5: // Выбросить предмет

    printf("Введи Слот (0-9): ");
    if (!input(&slot)) break;

    item_id = inventory[slot];
    inventory[slot] = 0;
    printf("Ты выкинул [%s] из слота %d\n", item_names[item_id], slot);
    break;

    case 6: // Найти тяжести

      printf("Введи ID предмета: ");
      if(!input(&item_id)) break;

      check_weights(item_id, inventory_size);
      break;

    case 7: // Поиск предмета в рюкзаке по названию
    {
      char search_name[MAX_ITEM_NAME_LEN];
      printf("Введи название предмета для поиска: ");
      scanf("%31s", search_name);
      while (getchar() != '\n');

      find_by_name(search_name);
      break;
    }

    case 8: // Записать состояние в дневник фермера (diary.txt)

      save_to_diary();
      break;

    case 9: // Расшифровать старые записи

      check_crafting_recipes();
      break;

    case 0: // выход

      printf("Ну пока");
      break;

    default:

      printf("Ты че? только 1-9\n");
      break;

    }

  } while (taskNum != 0);
  return 0;
}

// Проверка ввода
int input(int *var) {
  if (scanf("%d", var) != 1)
  {
    printf("Нужно число\n");
    while (getchar() != '\n');
    *var = -1;
    return 0;
  }
  while (getchar() != '\n');
  return 1;
}

// Загрузка предметов
void load_items(char names[MAX_ITEMS][MAX_ITEM_NAME_LEN])
{
  FILE *file = fopen("items.txt", "r");
  if (file == NULL)
  {
    printf("items.txt не найден\n");
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

  // Чтение числа и слова построчно
  while (fscanf(file, "%d %31s", &id, name) == 2)
  {
    if (id >= 0 && id < MAX_ITEMS)
    {
      strcpy(names[id], name);
    }
  }
  fclose(file);
}

// Вывод инвентаря
int check_inventory(int *inventory, int inventory_size)
{
  for (int i = 0; i < inventory_size; i++)
  {
    printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]]);
  }
  return 0;
}

// Найти тяжести
int check_weights(int item_id, int inventory_size)
{
  int found = 0;
  printf("Слоты, ID которых больше %d:\n", item_id);

  for (int i = 0; i < inventory_size; i++)
  {
    if (inventory[i] > item_id)
    {
      printf("- Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]]);
      found = 1;
    }
  }

  if (!found) {printf("Не найдено\n");}
  return 0;
}

// Поиск предмета по названию
void find_by_name(const char *search_name)
{
  int target_id = -1;
  // Поиск ID в каталоге
  for (int i = 0; i < MAX_ITEMS; i++)
  {
    if (strcmp(item_names[i], search_name) == 0) // сравнение строк: одинаковые - 0, иначе 1
    {
      target_id = i;
      break;
    }
  }
  if (target_id == -1)
  {
    printf("Предмета [%s] нет в каталоге!\n", search_name);
    return;
  }

  // Поиск ID в слотах рюкзака
  int found = 0;
  printf("Поиск %s (ID %d) в инвентаре...\n", search_name, target_id);
  for (int i = 0; i < inventory_size; i++)
  {
    if (inventory[i] == target_id)
    {
      printf("- Слот [%d]\n", i);
      found = 1;
    }
  }
  if (!found) {printf("В рюкзаке предмета [%s] нет.\n", search_name);}
}

// Запись состояния в дневник
void save_to_diary(void)
{
  FILE *file = fopen("diary.txt", "a");
  if (file == NULL)
  {
    printf("Не удалось открыть файл diary.txt\n");
    return;
  }

  fprintf(file, "────────── Запись в дневнике ──────────\n");
  fprintf(file, "Фермер: %s", username);
  fprintf(file, "Время: День %d, %02d:00\n", current_day, current_hour);
  fprintf(file, "Содержимое рюкзака:\n");
  for (int i = 0; i < inventory_size; i++)
  {
    fprintf(file, "  Слот %d: [%d] %s\n", i, inventory[i], item_names[inventory[i]]);
  }
  fprintf(file, "───────────────────────────────────────\n\n");

  fclose(file);
  printf("Запись успешно добавлена в diary.txt!\n");
}

// Проверка рецептов крафта
void check_crafting_recipes(void)
{
  FILE *f_in = fopen("input.txt", "r");
  if (f_in == NULL)
  {
    printf("Файл input.txt не найден\n");
    return;
  }

  FILE *f_out = fopen("output.txt", "w");
  if (f_out == NULL)
  {
    printf("Не удалось создать output.txt\n");
    fclose(f_in);
    return;
  }

  // Сколько ресурсов прямо сейчас лежит в инвентаре
  int available[MAX_ITEMS] = {0};
  for (int i = 0; i < inventory_size; i++)
  {
    if (inventory[i] > 0 && inventory[i] < MAX_ITEMS) {available[inventory[i]]++;}
  }

  printf("\n=== РЕЗУЛЬТАТ ПРОВЕРКИ КРАФТА ===\n");
  fprintf(f_out, "=== РЕЗУЛЬТАТ ПРОВЕРКИ КРАФТА ===\n");

  char line[256];
  // Чтение рецептов
  while (fgets(line, sizeof(line), f_in) != NULL)
  {
    int colon_index = -1;

    // Поиск двоеточия и разделение названий с рецептом
    for (int i = 0; line[i] != '\0'; i++)
    {
      if (line[i] == ':')
      {
        line[i] = '\0';
        colon_index = i;
        break;
      }
    }

    if (colon_index == -1) continue;

    char *recipe_name = line; // Название рецепта
    int required[MAX_ITEMS] = {0};

    // Сбор всех цифр после двоеточия
    for (int i = colon_index + 1; line[i] != '\0'; i++)
    {
      if (line[i] >= '0' && line[i] <= '9')
      {
        int id = line[i] - '0'; // число - 48 (по ASCII)
        required[id]++;
      }
    }

    // Проверка ресурсов
    int can_craft = 1;
    for (int id = 1; id < MAX_ITEMS; id++)
    {
      if (available[id] < required[id])
      {
        can_craft = 0;
        break;
      }
    }

    // Вывод на экран и в output.txt
    if (can_craft)
    {
      printf("%s: [ДОСТУПНО]\n", recipe_name);
      fprintf(f_out, "%s: [ДОСТУПНО]\n", recipe_name);
    }
    else
    {
      printf("%s: [НЕ ХВАТАЕТ РЕСУРСОВ] (Не хватает: ", recipe_name);
      fprintf(f_out, "%s: [НЕ ХВАТАЕТ РЕСУРСОВ] (Не хватает: ", recipe_name);

      int first = 1;
      for (int id = 1; id < MAX_ITEMS; id++)
      {
        if (available[id] < required[id])
        {
          int missing = required[id] - available[id];
          if (!first)
          {
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
  printf("сохранено в output.txt\n\n");
}
#include <stdio.h>

// Переменные
int current_day = 1;
int current_hour = 8;
int inventory[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int inventory_size = sizeof(inventory) / sizeof(inventory[0]);
const char *items[10] = {"Пусто", "Дерево", "Камень", "Семена", "Трава",
                         "Гриб",  "Железо", "Песок",  "Уголь",  "Золото"};
int work_hours;
int slot,item_id;

// Декларации функций
int input(int *var);
int check_inventory(int *inventory, int inventory_size);
int check_weights(int item_id, int inventory_size);

    int main() {
  int taskNum;
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
           "Введи № действия (1-6): ");

    input(&taskNum);

    switch (taskNum)
    {
    case 1: // посмотреть время
      
      printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
      break;

    case 2: // поработать
      
      printf("Сколько часов работать?: ");
      input(&work_hours);

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
      input(&slot);

      if (slot < 0 || slot > 9) 
      {
        printf("Такого слота нет (0-9)\n");
        continue;
      }

      printf("Введи ID предмета (1-9): ");
      input(&item_id);

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
      printf("В слот %d помещен предмет: [%d] (%s)\n", slot, item_id,items[item_id]);
      break;

    case 5: // Выбросить предмет

    printf("Введи Слот (0-9): ");
    input(&slot);

    item_id = inventory[slot];
    inventory[slot] = 0;
    printf("Ты выкинул [%s] из слота %d\n", items[item_id], slot);
    break;

    case 6: // Найти тяжести

      printf("Введи ID предмета: ");
      input(&item_id);

      check_weights(item_id, inventory_size);
      break;

    case 0: // выход

      printf("Ну пока");
      break;

    default:

      printf("Ты че? только 1-6\n");
      break;

    }

  } while (taskNum != 0);
  return 0;
}

// Проверка ввода
int input(int *var)
{
  if (scanf("%d", var) != 1)
  {
    printf("Нужно число\n");
    while (getchar() != '\n');
    return 0;
  }
  return 1;
}

// Вывод инвентаря
int check_inventory(int *inventory, int inventory_size)
{
  for (int i = 0; i < inventory_size; i++)
  {
    printf("Слот %d: [%d] (%s)\n", i, inventory[i], items[inventory[i]]);
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
      printf("- Слот %d: [%d] (%s)\n", i, inventory[i], items[inventory[i]]);
      found = 1;
    }
  }

  if (!found) {printf("Такых нэд\n");}
  return 0;
}
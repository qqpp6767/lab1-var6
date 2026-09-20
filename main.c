#include <stdio.h>

int current_day = 1;
int current_hour = 8;
int inventory[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const char *items[10] = {"Пусто", "Дерево", "Камень", "Семена", "Трава",
                         "Гриб",  "Железо", "Песок",  "Уголь",  "Золото"};

int work_hours;
int slot,item_id;

int main() 
{
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

    if (scanf("%d", &taskNum) != 1) // проверка ввода
    {
      printf("Э, я число просил\n");
      while (getchar() != '\n'); // очистка буфера ввода
      taskNum = -1; // чтобы случайно не завершилась вся программа
      continue;
    }

    switch (taskNum)
    {
    case 1: // посмотреть время
      
      printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
      break;

    case 2: // поработать
      
      printf("Сколько часов работать?: ");
      if (scanf("%d", &work_hours) != 1)
      {
        printf("Э, я число просил\n");
        while (getchar() != '\n');
      }

      current_hour += work_hours;

      // перевод времени
      while (current_hour >= 24)
      {
        current_day++;
        current_hour -= 24;
      }
      break;

    case 3: // чекнуть инвентарь
      
      for (int i = 0; i < sizeof(inventory) / sizeof(inventory[0]); i++) 
      {
        printf("Слот %d: [%d] (%s)\n", i, inventory[i], items[inventory[i]]);
      }
      break;

    case 4: // положить предмет в слот

      printf("Введи номер слота (0-9): ");
      if (scanf("%d", &slot) != 1)
      {
        printf("Не то\n");
        while (getchar() != '\n');
        continue;
      }

      if (slot < 0 || slot > 9) 
      {
        printf("Такого слота нет родной (0-9)\n");
        continue;
      }

      printf("Введи ID предмета (1-9): ");
      if (scanf("%d", &item_id) != 1)
      {
        printf("Не то\n");
        while (getchar() != '\n');
        continue;
      }

      if (item_id == 0) {
        printf("Неа, удалить незя\n");
        continue;
      }
      else if (item_id < 1 || item_id > 9)
      {
        printf("Такого ID нет родной (1-9)\n");
        continue;
      }

      inventory[slot] = item_id;
      printf("В слот %d помещен предмет: [%d] (%s)\n", slot, item_id,items[item_id]);
      break;

    case 5: // Выбросить предмет

    printf("Введи Слот (0-9): ");
    if (scanf("%d", &slot) != 1)
    {
      printf("Э, я число просил\n");
      while (getchar() != '\n');
      continue;
    }

    item_id = inventory[slot];
    inventory[slot] = 0;
    printf("Ты выкинул [%s] из слота %d\n",items[item_id], slot);
    break;

    case 6: // Найти тяжести

      printf("6\n");
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

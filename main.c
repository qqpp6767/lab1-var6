#include <stdio.h>

int current_day = 1;
int current_hour = 8;
int inventory[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/* 
ID предметов
0 - пусто
1 - дерево
2 - камень
3 - семена
4 - трава
5 - грибы
6 - железо
7 - песок
8 - уголь
9 - золото
*/
int work_hours;

int main() 
{
  int taskNum;
  do 
  {
    printf("──────────────────────────────\n"
           "╭0 - Выход                   ╮\n"
           "├1 - Посмотреть на часы      │\n"
           "├2 - Поработать              │\n"
           "├3 - Посмотреть инвентарь    │\n"
           "├4 - Положить предмет в слот │\n"
           "├5 - Выбросить предмет       │\n"
           "╰6 - Найти тяжести           ╯\n"
           "Введи № действия (1-6): ");

    if (scanf("%d", &taskNum) != 1) // проверка ввода
    {
      printf("Э, я число просил\n");
      while (getchar() != '\n'); // очистка буфера ввода
      taskNum = -1; // чтобы случайно не завершился код
      continue;
    }

    switch (taskNum)
    {
    case 1: // посмотреть время
      
      printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
      break;

    case 2: // поработать
      
      printf("Сколько часов работать?: ");
      if (scanf("%d", &work_hours) != 1) // проверка ввода
      {
        printf("Э, я число просил\n");
        while (getchar() != '\n'); // очистка буфера ввода
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

      printf("3\n");
      break;

    case 4: // положить предмет в слот

      printf("4\n");
      break;

    case 5: // Выбросить предмет

      printf("5\n");
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

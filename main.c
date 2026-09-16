#include <stdio.h>

int current_day = 1;
int current_hour = 8;
int inventory[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
/*
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

int main() 
{
  int taskNum;
  do 
  {
    printf("Введи № действия (1-6): ");
    if (scanf("%d", &taskNum) == 0) // проверка ввода
    {
      printf("Ты конч? Я просил число\n");
      while (getchar() != '\n'); // очистка буфера ввода
      continue;
    }

    switch (taskNum) 
    {
    case 1:
      printf("1\n");
      break;
    case 2:
      printf("2\n");
      break;
    case 3:
      printf("3\n");
      break;
    case 4:
      printf("4\n");
      break;
    case 5:
      printf("5\n");
      break;
    case 6:
      printf("6\n");
      break;
    case 0:
      printf("Ну пока");
      break;
    default:
      printf("Ты че? только 1-6\n");
      break;
    }

  } while (taskNum != 0);
  return 0;
}

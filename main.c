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

int main(){
    int taskNum;
    do{
        printf("Введите № действия: ");
        scanf("%d",&taskNum);
    } while (taskNum != 0);
    return 0;
}

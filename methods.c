#include <stdio.h>
#include <stdlib.h> //Содержит функции randomize и random
#include <conio.h> //Содержит функции clrscr и getch
#include <cstdlib>

int TRUE = 1;
int FALSE = 0;

enum Direction { LEFT, UP, RIGHT, DOWN };

int Field[4][4];
int EmptyX, EmptyY;

void CreateField()
{
    int NumIsFree[15]; //NumIsFree[i] показывает, определили ли мы уже позицию i-й костяшки
    int Nums[15]; //Nums[i] содержит номер костяшки, находящейся в i-й позиции
    for (int i = 0; i < 15; i++) //Объявляем, что изначально все костяшки свободны
        NumIsFree[i] = TRUE;
    srand((unsigned)time(0)); //randomize позволяет при каждом прогоне программы получать разные последовательности псевдослучайных чисел
    int Ok; //Флаг, определяющий корректность выбора костяшки для данной позиции
    int RandNum; //Номер костяшки, генерируемый в дальнейшем случайным образом
    for (int i = 0; i < 15; i++) //Нам надо определить номер костяшки, находящейся в каждой из 15 позиций поля
    {
        Ok = FALSE; //Каждый раз сбрасываем значение флага
        while (!Ok) //Продолжаем случайным образом определять номер костяшки, пока он не окажется корректным
        {
            RandNum = (rand() % 15) + 1; //random(n) генерирует псевдослучайное число от 0 до n - 1, а нам нужно от 1 до 15
            if (NumIsFree[RandNum - 1]) //Если костяшка с таким номером еще свободна (помним, что массивы нумеруются начиная с нуля)
                Ok = FALSE; //то мы определили ее номер корректно
        }
        Nums[i] = RandNum; //Записываем этот корректный номер в i-ю позицию
        NumIsFree[RandNum - 1] = FALSE; //Костяшка с этим номером теперь занята
    }

    int Chaos = 0; //Количество беспорядков на поле
    int CurrNum; //Костяшка, для которой мы рассматриваем беспорядки
    for (int i = 0; i < 14; i++) //Считаем для костяшек на первых 14 позициях (для 15-й это бессмысленно)
    {
        CurrNum = Nums[i];
        for (int j = i + 1; j < 15; j++)
            if (CurrNum > Nums[j])
                Chaos++;
    }
    if (Chaos % 2 == 1) //Если общее число беспорядков нечетное,
    { //меняем местами костяшки на 14-й и 15-й позициях
        int temp = Nums[13];
        Nums[13] = Nums[14];
        Nums[14] = temp;
    }

    for (int i = 0; i < 15; i++)
        Field[i % 4][i / 4] = Nums[i]; //a % b - остаток от деления a на b
    Field[3][3] = 0;
    EmptyX = 3; EmptyY = 3;
}

void DrawField() //Вариант, подходящий для всех реализаций
{
    //Очищаем экран
    system("cls");
    Sleep(1000);

    for (int j = 0; j < 4; j++) //Рисуем построчно четыре горизонтальных ряда костяшек
    {
        for (int i = 0; i < 4; i++) //Рисуем верхнюю часть ряда
        {
            if (Field[i][j])
                printf("++++");
            else
                printf("    ");
        }
        printf("\n");
        for (int i = 0; i < 4; i++) //Рисуем среднюю часть ряда (с номерами)
        {
            if (Field[i][j])
            {
                printf("+");
                //std::cout.width(2);
                printf("%d", &Field[i][j]);
                printf("+");
            }
            else
                printf("    ");
        }
        printf("\n");
        for (int i = 0; i < 4; i++) //Рисуем нижнюю часть ряда
        {
            if (Field[i][j])
                printf("++++");
            else
                printf("    ");
        }
        printf("\n");
    }
}

void Move(enum Direction dir)
{
    switch (dir)
    {
    case LEFT:
    {
        if (EmptyX < 3)
        {
            Field[EmptyX][EmptyY] = Field[EmptyX + 1][EmptyY];
            Field[EmptyX + 1][EmptyY] = 0;
            EmptyX++;
        }
    } break;
    case UP:
    {
        if (EmptyY < 3)
        {
            Field[EmptyX][EmptyY] = Field[EmptyX][EmptyY + 1];
            Field[EmptyX][EmptyY + 1] = 0;
            EmptyY++;
        }
    } break;
    case RIGHT:
    {
        if (EmptyX > 0)
        {
            Field[EmptyX][EmptyY] = Field[EmptyX - 1][EmptyY];
            Field[EmptyX - 1][EmptyY] = 0;
            EmptyX--;
        }
    } break;
    case DOWN:
    {
        if (EmptyY > 0)
        {
            Field[EmptyX][EmptyY] = Field[EmptyX][EmptyY - 1];
            Field[EmptyX][EmptyY - 1] = 0;
            EmptyY--;
        }
    } break;
    }
}

int FieldIsCorrect()
{
    for (int i = 0; i < 15; i++)
        if (Field[i % 4][i / 4] != i + 1)
            return FALSE; //При первом же нахождении несоответствия выходим и возвращаем false
    return TRUE;//Если не найдено ни одного несоответствия - поле собрано верно
}

int Exmain()
{
    CreateField(); //Генерация поля
    DrawField(); //Первоначальное рисование поля
    char c; //В эту переменную мы считываем символ нажатой клавиши
    while (!FieldIsCorrect()) //Игровой цикл (выполняется пока поле не будет собрано)
    {
        c = _getch(); //Считываем нажатие клавиши
        switch (c) //В зависимости от нажатой клавиши (от ее целочисленного кода) двигаем костяшки
        {
        case 75: Move(LEFT); break; //Нажата клавиша "Влево"
        case 72: Move(UP); break; //Нажата клавиша "Вверх"
        case 77: Move(RIGHT); break; //Нажата клавиша "Вправо"
        case 80: Move(DOWN); break; //Нажата клавиша "Вниз"
        case 27: return 0; //Нажата клавиша "Escape"
        }

        DrawField(); //Рисование поля
    }
    printf("\n\nCongratulations! Press Enter to exit!"); //Поздравление игрока
    //getchar(); //Ждем ввода строки и нажатия Enter перед выходом
}
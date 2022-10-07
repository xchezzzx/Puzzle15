#include <stdio.h>
#include <stdlib.h> //�������� ������� randomize � random
#include <conio.h> //�������� ������� clrscr � getch
#include <cstdlib>

int TRUE = 1;
int FALSE = 0;

enum Direction { LEFT, UP, RIGHT, DOWN };

int Field[4][4];
int EmptyX, EmptyY;

void CreateField()
{
    int NumIsFree[15]; //NumIsFree[i] ����������, ���������� �� �� ��� ������� i-� ��������
    int Nums[15]; //Nums[i] �������� ����� ��������, ����������� � i-� �������
    for (int i = 0; i < 15; i++) //���������, ��� ���������� ��� �������� ��������
        NumIsFree[i] = TRUE;
    srand((unsigned)time(0)); //randomize ��������� ��� ������ ������� ��������� �������� ������ ������������������ ��������������� �����
    int Ok; //����, ������������ ������������ ������ �������� ��� ������ �������
    int RandNum; //����� ��������, ������������ � ���������� ��������� �������
    for (int i = 0; i < 15; i++) //��� ���� ���������� ����� ��������, ����������� � ������ �� 15 ������� ����
    {
        Ok = FALSE; //������ ��� ���������� �������� �����
        while (!Ok) //���������� ��������� ������� ���������� ����� ��������, ���� �� �� �������� ����������
        {
            RandNum = (rand() % 15) + 1; //random(n) ���������� ��������������� ����� �� 0 �� n - 1, � ��� ����� �� 1 �� 15
            if (NumIsFree[RandNum - 1]) //���� �������� � ����� ������� ��� �������� (������, ��� ������� ���������� ������� � ����)
                Ok = FALSE; //�� �� ���������� �� ����� ���������
        }
        Nums[i] = RandNum; //���������� ���� ���������� ����� � i-� �������
        NumIsFree[RandNum - 1] = FALSE; //�������� � ���� ������� ������ ������
    }

    int Chaos = 0; //���������� ����������� �� ����
    int CurrNum; //��������, ��� ������� �� ������������� ����������
    for (int i = 0; i < 14; i++) //������� ��� �������� �� ������ 14 �������� (��� 15-� ��� ������������)
    {
        CurrNum = Nums[i];
        for (int j = i + 1; j < 15; j++)
            if (CurrNum > Nums[j])
                Chaos++;
    }
    if (Chaos % 2 == 1) //���� ����� ����� ����������� ��������,
    { //������ ������� �������� �� 14-� � 15-� ��������
        int temp = Nums[13];
        Nums[13] = Nums[14];
        Nums[14] = temp;
    }

    for (int i = 0; i < 15; i++)
        Field[i % 4][i / 4] = Nums[i]; //a % b - ������� �� ������� a �� b
    Field[3][3] = 0;
    EmptyX = 3; EmptyY = 3;
}

void DrawField() //�������, ���������� ��� ���� ����������
{
    //������� �����
    system("cls");
    Sleep(1000);

    for (int j = 0; j < 4; j++) //������ ��������� ������ �������������� ���� ��������
    {
        for (int i = 0; i < 4; i++) //������ ������� ����� ����
        {
            if (Field[i][j])
                printf("++++");
            else
                printf("    ");
        }
        printf("\n");
        for (int i = 0; i < 4; i++) //������ ������� ����� ���� (� ��������)
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
        for (int i = 0; i < 4; i++) //������ ������ ����� ����
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
            return FALSE; //��� ������ �� ���������� �������������� ������� � ���������� false
    return TRUE;//���� �� ������� �� ������ �������������� - ���� ������� �����
}

int Exmain()
{
    CreateField(); //��������� ����
    DrawField(); //�������������� ��������� ����
    char c; //� ��� ���������� �� ��������� ������ ������� �������
    while (!FieldIsCorrect()) //������� ���� (����������� ���� ���� �� ����� �������)
    {
        c = _getch(); //��������� ������� �������
        switch (c) //� ����������� �� ������� ������� (�� �� �������������� ����) ������� ��������
        {
        case 75: Move(LEFT); break; //������ ������� "�����"
        case 72: Move(UP); break; //������ ������� "�����"
        case 77: Move(RIGHT); break; //������ ������� "������"
        case 80: Move(DOWN); break; //������ ������� "����"
        case 27: return 0; //������ ������� "Escape"
        }

        DrawField(); //��������� ����
    }
    printf("\n\nCongratulations! Press Enter to exit!"); //������������ ������
    //getchar(); //���� ����� ������ � ������� Enter ����� �������
}
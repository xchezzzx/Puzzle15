#include <stdio.h>
#include <stdlib.h> //�������� ������� randomize � random

#include "windows.h"
#include "commctrl.h"
#include "Puzzle15.h"
//#include "methods.c"

#define MAX_LOADSTRING 100
#define SIZE 16
#define WIDTH 200
#define SPACE 10

typedef struct
{
    RECT TileRect;
    int TileNum;
    //wchar_t* TileName;
} Tile;

// Global Variables:


HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"The Puzzle of 15";                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("GameWnd");            // the main window class name

RECT ClientArea;                    //size of client area in window
Tile TilesArray[4][4];               //array of tiles for field

//int FieldInt[4][4];                 //array of puzzle numbers
Tile EmptyTile;
int EmptyX, EmptyY;                     //coordinates of empty place
int movesCount = 0;

enum Direction { LEFT, UP, RIGHT, DOWN };

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int GenerateField()
{
    int TrueVar = 1;
    int FalseVar = 0;

    int NumIsFree[15]; //NumIsFree[i] ����������, ���������� �� �� ��� ������� i-� ��������
    int Nums[15]; //Nums[i] �������� ����� ��������, ����������� � i-� �������
    for (int i = 0; i < 15; i++) //���������, ��� ���������� ��� �������� ��������
        NumIsFree[i] = TrueVar;
    srand((unsigned)time(0)); //randomize ��������� ��� ������ ������� ��������� �������� ������ ������������������ ��������������� �����
    int Flag; //����, ������������ ������������ ������ �������� ��� ������ �������
    int RandNum; //����� ��������, ������������ � ���������� ��������� �������
    for (int i = 0; i < 15; i++) //��� ���� ���������� ����� ��������, ����������� � ������ �� 15 ������� ����
    {
        Flag = FalseVar; //������ ��� ���������� �������� �����
        while (!Flag) //���������� ��������� ������� ���������� ����� ��������, ���� �� �� �������� ����������
        {
            RandNum = (rand() % 15) + 1; //random(n) ���������� ��������������� ����� �� 0 �� n - 1, � ��� ����� �� 1 �� 15
            if (NumIsFree[RandNum - 1]) //���� �������� � ����� ������� ��� �������� (������, ��� ������� ���������� ������� � ����)
                Flag = TrueVar; //�� �� ���������� �� ����� ���������
        }
        Nums[i] = RandNum; //���������� ���� ���������� ����� � i-� �������
        NumIsFree[RandNum - 1] = FalseVar; //�������� � ���� ������� ������ ������
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
    {   
        TilesArray[i % 4][i / 4].TileNum = Nums[i]; //a % b - ������� �� ������� a �� b
       // _itoa_s(Nums[i], TilesArray[i % 4][i / 4].TileName, sizeof(TilesArray[i % 4][i / 4].TileName), i);
    }

    EmptyX = 3;
    EmptyY = 3;

    TilesArray[EmptyX][EmptyY].TileNum = 0;

    return 0;
}

void CreateField()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            TilesArray[i][j].TileRect.left = SPACE * (i + 1) + WIDTH * i;
            TilesArray[i][j].TileRect.top = SPACE * (j + 1) + WIDTH * j;
            TilesArray[i][j].TileRect.right = SPACE * (i + 1) + WIDTH * (i + 1);
            TilesArray[i][j].TileRect.bottom = SPACE * (j + 1) + WIDTH * (j + 1);
        }
    }
}

void Move(enum Direction dir)
{
    Tile TempTile;

    switch (dir)
    {
        case LEFT:
        {
            if (EmptyX < 3)
            {
                TilesArray[EmptyX][EmptyY].TileNum = TilesArray[EmptyX + 1][EmptyY].TileNum;
                TilesArray[EmptyX][EmptyY].TileRect = TilesArray[EmptyX + 1][EmptyY].TileRect;
                TilesArray[EmptyX + 1][EmptyY].TileNum = 0;
                EmptyX++;
            }
        } break;
        case UP:
        {
            if (EmptyY < 3)
            {
                TilesArray[EmptyX][EmptyY].TileNum = TilesArray[EmptyX][EmptyY + 1].TileNum;
                TilesArray[EmptyX][EmptyY + 1].TileNum = 0;
                EmptyY++;
            }
        } break;

        case RIGHT:
        {
            if (EmptyX > 0)
            {
                //TilesArray[EmptyX][EmptyY] = TilesArray[EmptyX - 1][EmptyY];
                //TilesArray[EmptyX - 1][EmptyY].TileNum = 0;

                TempTile = TilesArray[EmptyX - 1][EmptyY];
                TilesArray[EmptyX-1][EmptyY] = EmptyTile;
                TilesArray[EmptyX][EmptyY] = TempTile;

                //TilesArray[EmptyX - 1][EmptyY].TileNum;
                //TilesArray[EmptyX-1][EmptyY].TileRect.left += WIDTH;
                //TilesArray[EmptyX-1][EmptyY].TileRect.right += WIDTH;

                //TilesArray[EmptyX - 1][EmptyY].TileNum = 0;
                EmptyX--;
            }
        }
        break;

        case DOWN:
        {
            if (EmptyY > 0)
            {
                TilesArray[EmptyX][EmptyY].TileNum = TilesArray[EmptyX][EmptyY - 1].TileNum;
                TilesArray[EmptyX][EmptyY - 1].TileNum = 0;
                EmptyY--;
            }
        } break;
    }
}

BOOL FieldIsCorrect()
{
    for (int i = 0; i < 15; i++)
        if (TilesArray[i % 4][i / 4].TileNum != i + 1)
            return FALSE; //��� ������ �� ���������� �������������� ������� � ���������� false
    return TRUE;//���� �� ������� �� ������ �������������� - ���� ������� �����
    //SendMessage
}



void DrawField(HWND hwnd, HDC hdc)
{
    HBRUSH hBrushColorBG = CreateSolidBrush(RGB(0, 0, 75));
    HBRUSH hBrushColorRECT = CreateSolidBrush(RGB(60, 60, 200));
    HBRUSH hBrushColorEL = CreateSolidBrush(RGB(220, 220, 220));
    HFONT hFont, holdFont;
    wchar_t bufer[10];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (TilesArray[i][j].TileNum != 0)
            {
                SelectObject(hdc, hBrushColorBG);
                RoundRect(hdc, TilesArray[i][j].TileRect.left, TilesArray[i][j].TileRect.top, TilesArray[i][j].TileRect.right, TilesArray[i][j].TileRect.bottom, 15, 20);
                SelectObject(hdc, hBrushColorRECT);
                RoundRect(hdc, TilesArray[i][j].TileRect.left + SPACE, TilesArray[i][j].TileRect.top + SPACE, TilesArray[i][j].TileRect.right - SPACE, TilesArray[i][j].TileRect.bottom - SPACE, 15, 20);
                SelectObject(hdc, hBrushColorEL);
                Ellipse(hdc,
                    TilesArray[i][j].TileRect.left + 150,
                    TilesArray[i][j].TileRect.top + 150,
                    TilesArray[i][j].TileRect.right - 150,
                    TilesArray[i][j].TileRect.bottom - 150
                );
                
                SetBkColor(hdc, RGB(220, 220, 220));

                hFont = CreateFontW(60, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"CourierNew");
                holdFont = SelectObject(hdc, hFont);

                wsprintf(bufer, TEXT("%i"), TilesArray[i][j].TileNum);

                TextOutW(hdc,
                    TilesArray[i][j].TileRect.left + 75,
                    TilesArray[i][j].TileRect.top + 70,
                    bufer,
                    lstrlen(bufer));
             
                SelectObject(hdc, holdFont);
                DeleteObject(hFont);
                InvalidateRect(hwnd, 0, 0);
            }
        }
    }

}

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MSG msg;

    GenerateField();
    CreateField();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PUZZLE15, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PUZZLE15));


    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, TEXT("D:\\sources\\C\\Puzzle15\\15.ico"));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PUZZLE15);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, TEXT("D:\\sources\\C\\Puzzle15\\15.ico"));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindow(
        szWindowClass,                  // window class name
        szTitle,                    // window caption
        WS_OVERLAPPEDWINDOW,        // window style
        200,              // initial x position
        80,              // initial y position
        1400,                          // initial x size
        910,                          // initial y size
        NULL,                        // parent window handle
        NULL,                       // window menu handle
        hInstance,                  // program instance handle
        NULL);                     // creation parameters

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND restart, moveCount, textMoves;
    PAINTSTRUCT ps;
    HDC hdc;

    Tile TempTile;


    HFONT hFont1, hFont2, holdFont;
    wchar_t bufer1[30];
    wchar_t bufer2[30];
    wchar_t bufer3[30];
    wchar_t bufer4[30];
    wchar_t bufer5[30];
    wchar_t bufer6[30];
    wchar_t bufer7[30];


    switch (message)
    {
    case WM_CREATE:

        restart = CreateWindowW(
            L"button", L"Restart game",
            WS_VISIBLE | WS_CHILD,
            900, 640, 200, 100,
            hWnd, (HMENU) 10001, NULL, NULL);
        
        CreateWindowW(L"Button", L"Moves made",
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            860, 10, 200, 200, hWnd, (HMENU)10002, NULL, NULL);
        
        //textMoves = CreateWindowW(WC_STATIC, L"0", WS_CHILD | WS_VISIBLE
          //  | SS_CENTER, 15, 60, 300, 230, hWnd, (HMENU)10003, NULL, NULL);

        break;

    case WM_SIZE:
        GetClientRect(hWnd, &ClientArea);
        break;

    case WM_NOTIFY:
       // const int asize = 4;
        //wchar_t buf[4];
        //size_t cbDest = asize * sizeof(wchar_t);
        //StringCbPrintfW(buf, cbDest, L"%d", movesCount);

        //SetWindowTextW(textMoves, buf);

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        // TODO: Add any drawing code that uses hdc here...
        DrawField(hWnd, hdc);
        //SetBkColor(hdc, RGB(220, 220, 220));

        hFont1 = CreateFontW(28, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"CourierNew");
        hFont2 = CreateFontW(80, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"CourierNew");

        holdFont = SelectObject(hdc, hFont1);

        wsprintf(bufer1, TEXT("LEFT: %i"), TilesArray[EmptyX][EmptyY].TileRect.left);
        wsprintf(bufer2, TEXT("TOP: %i"), TilesArray[EmptyX][EmptyY].TileRect.top);
        wsprintf(bufer3, TEXT("RIGHT: %i"), TilesArray[EmptyX][EmptyY].TileRect.right);
        wsprintf(bufer4, TEXT("BOTTOM: %i"), TilesArray[EmptyX][EmptyY].TileRect.bottom);
        wsprintf(bufer5, TEXT("EmptyX: %i"), EmptyX);
        wsprintf(bufer6, TEXT("EmptyY: %i"), EmptyY);
        DeleteObject(holdFont);
        
        holdFont = SelectObject(hdc, hFont2);

        wsprintf(bufer7, TEXT("%i"), movesCount);


        TextOutW(hdc, 900, 200, bufer1, lstrlen(bufer1));
        TextOutW(hdc, 900, 250, bufer2, lstrlen(bufer2));
        TextOutW(hdc, 1100, 200, bufer3, lstrlen(bufer3));
        TextOutW(hdc, 1100, 250, bufer4, lstrlen(bufer4));
        TextOutW(hdc, 900, 300, bufer5, lstrlen(bufer5));
        TextOutW(hdc, 900, 350, bufer6, lstrlen(bufer6));
        TextOutW(hdc, 900, 40, bufer7, lstrlen(bufer7));


        EndPaint(hWnd, &ps);
    }
    break;

    case WM_KEYDOWN:
         
        //c = getch();                          //��������� ������� �������
        switch (wParam)                              //� ����������� �� ������� ������� (�� �� �������������� ����) ������� ��������
        {
            case VK_LEFT:                   //������ ������� "�����" 
                if (EmptyX < 3)
                {
                    TempTile = TilesArray[EmptyX][EmptyY];
                    TilesArray[EmptyX][EmptyY] = TilesArray[EmptyX+1][EmptyY];
                    TilesArray[EmptyX+1][EmptyY] = TempTile;
                    CreateField();
                    EmptyX++;
                    movesCount++;
                }

                //Move(LEFT);
                SendMessage(hWnd, WM_PAINT, wParam, lParam);
                InvalidateRect(hWnd, 0, 0);

                break;

            case VK_UP:                     //������ ������� "�����"
                if (EmptyY < 3)
                {
                    TempTile = TilesArray[EmptyX][EmptyY];
                    TilesArray[EmptyX][EmptyY] = TilesArray[EmptyX][EmptyY+1];
                    TilesArray[EmptyX][EmptyY+1] = TempTile;
                    CreateField();
                    EmptyY++;
                    movesCount++;
                }
                //Move(UP);
                SendMessage(hWnd, WM_PAINT, wParam, lParam);
                InvalidateRect(hWnd, 0, 0);

                break;
                 
            case VK_RIGHT:                  //������ ������� "������"
            {
                if (EmptyX > 0)
                {
                    //TilesArray[EmptyX][EmptyY] = TilesArray[EmptyX - 1][EmptyY];
                    //TilesArray[EmptyX - 1][EmptyY].TileNum = 0;

                    TempTile = TilesArray[EmptyX - 1][EmptyY];
                    TilesArray[EmptyX - 1][EmptyY] = TilesArray[EmptyX][EmptyY];
                    TilesArray[EmptyX][EmptyY] = TempTile;
                    CreateField();
                    movesCount++;

                    //TilesArray[EmptyX - 1][EmptyY].TileNum;
                    //TilesArray[EmptyX-1][EmptyY].TileRect.left += WIDTH;
                    //TilesArray[EmptyX-1][EmptyY].TileRect.right += WIDTH;

                    //TilesArray[EmptyX - 1][EmptyY].TileNum = 0;
                    EmptyX--;

                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                }

            }
                //Move(RIGHT, hWnd);
                SendMessage(hWnd, WM_PAINT, wParam, lParam);
                InvalidateRect(hWnd, 0, 0);

                break;

            case VK_DOWN:                   //������ ������� "����"
                if (EmptyY > 0)
                {

                    TempTile = TilesArray[EmptyX][EmptyY-1];
                    TilesArray[EmptyX][EmptyY-1] = TilesArray[EmptyX][EmptyY];
                    TilesArray[EmptyX][EmptyY] = TempTile;
                    CreateField();
                    EmptyY--;
                    movesCount++;
                }
                //Move(DOWN);
                SendMessage(hWnd, WM_PAINT, wParam, lParam);
                InvalidateRect(hWnd, 0, 0);

                break;

            case VK_ESCAPE:                 //������ ������� "Escape" 
                SendMessage(hWnd, WM_DESTROY, 0, 0);
                return 0; 
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    //default:
    //    return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
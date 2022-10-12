#include <stdio.h>
#include <stdlib.h> //Содержит функции randomize и random

#include "windows.h"
#include "commctrl.h"
#include "Puzzle15.h"
//#include "methods.c"

#define MAX_LOADSTRING 100
#define SIZE 16
#define WIDTH 200
#define SPACE 0
#define ID_RESTART 10001
#define ID_QUIT 10002
#define ID_SOLVE 10003
#define DARKTHEME RGB(100, 100, 100)
#define SLEEPTIMER 200

typedef struct
{
    RECT TileRect;
    HBITMAP Pic;
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
int MovesCount = 0;                     //MovesCount - counter of moves done by player

enum Direction { LEFT, UP, RIGHT, DOWN };
int Victory = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int GenerateField()
{
    int TrueVar = 1;
    int FalseVar = 0;
    int counter = 0;
    MovesCount = 0;
    Victory = 0;

    int NumIsFree[15]; //NumIsFree[i] показывает, определили ли мы уже позицию i-й костяшки
    int Nums[15]; //Nums[i] содержит номер костяшки, находящейся в i-й позиции
    for (int i = 0; i < 15; i++) //Объявляем, что изначально все костяшки свободны
        NumIsFree[i] = TrueVar;
    srand((unsigned)time(0)); //randomize позволяет при каждом прогоне программы получать разные последовательности псевдослучайных чисел
    int Flag; //Флаг, определяющий корректность выбора костяшки для данной позиции
    int RandNum; //Номер костяшки, генерируемый в дальнейшем случайным образом
    for (int i = 0; i < 15; i++) //Нам надо определить номер костяшки, находящейся в каждой из 15 позиций поля
    {
        Flag = FalseVar; //Каждый раз сбрасываем значение флага
        while (!Flag) //Продолжаем случайным образом определять номер костяшки, пока он не окажется корректным
        {
            RandNum = (rand() % 15) + 1; //random(n) генерирует псевдослучайное число от 0 до n - 1, а нам нужно от 1 до 15
            if (NumIsFree[RandNum - 1]) //Если костяшка с таким номером еще свободна (помним, что массивы нумеруются начиная с нуля)
                Flag = TrueVar; //то мы определили ее номер корректно
        }
        Nums[i] = RandNum; //Записываем этот корректный номер в i-ю позицию
        NumIsFree[RandNum - 1] = FalseVar; //Костяшка с этим номером теперь занята
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

    for (int i = 0; i < 4; i++)
    {   
        for (int j = 0; j < 4; j++)
        {
            TilesArray[i][j].TileNum = Nums[counter]; //a % b - остаток от деления a на b
            counter++;
        }
    }

    EmptyX = 3;
    EmptyY = 3;

    TilesArray[EmptyX][EmptyY].TileNum = 16;

    return 0;
}

void SolvePuzzle()
{
    Tile buf;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            buf = TilesArray[i][j];
            int l = j + 1;
            for (int k = i; k < 4; k++) {
                while (l < 4) {
                    /* swapping the data */
                    if (buf.TileNum > TilesArray[k][l].TileNum)
                    {
                        buf = TilesArray[k][l];
                        TilesArray[k][l] = TilesArray[i][j];
                        TilesArray[i][j] = buf;
                        MovesCount++;
                    }
                    l++;
                }
                l = 0;
            }
        }
    }

    EmptyX = 3;
    EmptyY = 3;
}

void CreateField()
{
    wchar_t bufer[50];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (TilesArray[i][j].TileNum != 16)
            {
                TilesArray[i][j].TileRect.left = SPACE * (j + 1) + WIDTH * j;
                TilesArray[i][j].TileRect.top = SPACE * (i + 1) + WIDTH * i;
                TilesArray[i][j].TileRect.right = SPACE * (j + 1) + WIDTH * (j + 1);
                TilesArray[i][j].TileRect.bottom = SPACE * (i + 1) + WIDTH * (i + 1);

                wsprintf(bufer, TEXT("d:\\sources\\C\\Puzzle15\\sources\\tile%i.bmp"), TilesArray[i][j].TileNum);
                TilesArray[i][j].Pic = LoadImageW(hInst, bufer, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            }
        }
    }
}

void Move(enum Direction dir)
{
    Tile TempTile;

    switch (dir)
    {
    case LEFT:                  //Left Arrow pressed 
        if (EmptyY < 3)
        {
            TempTile = TilesArray[EmptyX][EmptyY];
            TilesArray[EmptyX][EmptyY] = TilesArray[EmptyX][EmptyY + 1];
            TilesArray[EmptyX][EmptyY + 1] = TempTile;
            PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
            EmptyY++;
            MovesCount++;
        }
        break;

    case UP:                     //Up Arrow pressed
        if (EmptyX < 3)
        {
            TempTile = TilesArray[EmptyX][EmptyY];
            TilesArray[EmptyX][EmptyY] = TilesArray[EmptyX + 1][EmptyY];
            TilesArray[EmptyX + 1][EmptyY] = TempTile;
            PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
            EmptyX++;
            MovesCount++;
        }
        break;

    case RIGHT:                  //Right Arrow pressed
        if (EmptyY > 0)
        {
            TempTile = TilesArray[EmptyX][EmptyY - 1];
            TilesArray[EmptyX][EmptyY - 1] = TilesArray[EmptyX][EmptyY];
            TilesArray[EmptyX][EmptyY] = TempTile;
            PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);

            EmptyY--;
            MovesCount++;
        }
        break;

    case DOWN:                   //Down Arrow pressed
        if (EmptyX > 0)
        {
            TempTile = TilesArray[EmptyX - 1][EmptyY];
            TilesArray[EmptyX - 1][EmptyY] = TilesArray[EmptyX][EmptyY];
            TilesArray[EmptyX][EmptyY] = TempTile;
            PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
            EmptyX--;
            MovesCount++;
        }
    break;
    }
}

BOOL FieldIsCorrect()
{
    int counter = 1;
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (TilesArray[i][j].TileNum != counter)
                return FALSE;//a % b - остаток от деления a на b
            counter++;
        }
    }
    Victory = 42;

    return TRUE;
}

void DrawField(HDC hdc)
{
    BITMAP bitmap;
    HGDIOBJ oldBitmap;

    HFONT hFont1, hFont2, holdFont;
    wchar_t bufer1[30];
    wchar_t bufer2[30];
    wchar_t bufer3[30];
    wchar_t bufer4[30];
    wchar_t bufer5[30];
    wchar_t bufer6[30];
    wchar_t bufer7[30];
    wchar_t bufer8[30];

    wchar_t buferX[30];
    wchar_t buferY[30];

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP memBM = CreateCompatibleBitmap(hdc, ClientArea.right - ClientArea.left, ClientArea.bottom - ClientArea.top);

    SelectObject(hdcMem, memBM);

    wchar_t bufer[10];

    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, DARKTHEME);
    Rectangle(hdc, 0, 0, 800, 800);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (TilesArray[i][j].TileNum != 16)
            {
                oldBitmap = SelectObject(hdcMem, TilesArray[i][j].Pic);

                GetObject(TilesArray[i][j].Pic, sizeof(bitmap), &bitmap);
                BitBlt(hdc, TilesArray[i][j].TileRect.left, TilesArray[i][j].TileRect.top, bitmap.bmWidth, bitmap.bmHeight,
                    hdcMem, 0, 0, SRCCOPY);

                //wsprintf(bufer, TEXT("%i"), TilesArray[i][j].TileNum);

                //TextOutW(hdc,
                //    TilesArray[i][j].TileRect.left + 75,
                //    TilesArray[i][j].TileRect.top + 70,
                //    bufer,
                //    lstrlen(bufer));
            }
        }
    }

    //SetBkColor(hdc, DARKTHEME);

    hFont1 = CreateFontW(28, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"CourierNew");
    hFont2 = CreateFontW(42, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"CourierNew");

    holdFont = SelectObject(hdc, hFont1);

    wsprintf(bufer1, TEXT("LEFT: %i"), TilesArray[EmptyX][EmptyY].TileRect.left);
    wsprintf(bufer2, TEXT("TOP: %i"), TilesArray[EmptyX][EmptyY].TileRect.top);
    wsprintf(bufer3, TEXT("RIGHT: %i"), TilesArray[EmptyX][EmptyY].TileRect.right);
    wsprintf(bufer4, TEXT("BOTTOM: %i"), TilesArray[EmptyX][EmptyY].TileRect.bottom);
    wsprintf(bufer5, TEXT("EmptyX: %i"), EmptyX);
    wsprintf(bufer6, TEXT("EmptyY: %i"), EmptyY);
    wsprintf(bufer8, TEXT("Status: %i"), Victory);

    DeleteObject(holdFont);

    holdFont = SelectObject(hdc, hFont2);

    wsprintf(bufer7, TEXT("%i"), MovesCount);


    TextOutW(hdc, 900, 200, bufer1, lstrlen(bufer1));
    TextOutW(hdc, 900, 250, bufer2, lstrlen(bufer2));
    TextOutW(hdc, 1100, 200, bufer3, lstrlen(bufer3));
    TextOutW(hdc, 1100, 250, bufer4, lstrlen(bufer4));
    TextOutW(hdc, 900, 300, bufer5, lstrlen(bufer5));
    TextOutW(hdc, 900, 350, bufer6, lstrlen(bufer6));
    //TextOutW(hdc, 0, 800, bufer7, lstrlen(bufer7));
    TextOutW(hdc, 900, 400, bufer8, lstrlen(bufer8));

    TextOutW(hdc, 1100, 300, buferX, lstrlen(buferX));
    TextOutW(hdc, 1100, 350, buferY, lstrlen(buferY));


    //BitBlt(hdc, 0, 0, ClientArea.right - ClientArea.left, ClientArea.bottom - ClientArea.top, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
    DeleteObject(memBM);
}

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MSG msg;

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
    //while (1)
    //{
    //    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    //    {
    //        if (msg.message == WM_QUIT) break;
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //    else
    //    {

    //    }
    //}

    //while (GetMessage(&msg, NULL, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(NULL, IDC_HAND);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PUZZLE15);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

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
        L"The Puzzle of 15",                    // window caption
        WS_OVERLAPPEDWINDOW,        // window style
        200,              // initial x position
        20,              // initial y position
        815,                          // initial x size
        958,                          // initial y size
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

    int xMouse;
    int yMouse;

    wchar_t bufer7[30];

    switch (message)
    {
        case WM_CREATE:

            GenerateField();
            CreateField();

            CreateWindowW(
                L"button", L"Solve",
                WS_VISIBLE | WS_CHILD,
                200, 800, WIDTH, WIDTH / 2,
                hWnd, (HMENU)ID_SOLVE, NULL, NULL);

            CreateWindowW(
                L"button", L"Restart game",
                WS_VISIBLE | WS_CHILD,
                400, 800, WIDTH, WIDTH / 2,
                hWnd, (HMENU) ID_RESTART, NULL, NULL);

            CreateWindowW(
                L"button", L"Quit",
                WS_VISIBLE | WS_CHILD,
                600, 800, WIDTH, WIDTH / 2,
                hWnd, (HMENU) ID_QUIT, NULL, NULL);

            return 0;

        case WM_SIZE:
            GetClientRect(hWnd, &ClientArea);
            break;

        //case WM_MOUSEMOVE:
            //xMouse = LOWORD(lParam);
            //yMouse = HIWORD(lParam);


        case WM_LBUTTONDOWN:
        {
            xMouse = LOWORD(lParam);
            yMouse = HIWORD(lParam);

            //if ((400 <= xMouse) && (xMouse <= 600) && (600 <= yMouse) && (yMouse <= 800))


            if ((TilesArray[EmptyX][EmptyY + 1].TileRect.left <= xMouse) && (xMouse <= TilesArray[EmptyX][EmptyY + 1].TileRect.right) &&
                (TilesArray[EmptyX][EmptyY + 1].TileRect.top <= yMouse) && (yMouse <= TilesArray[EmptyX][EmptyY + 1].TileRect.bottom) && EmptyY < 3)
            {
                SendMessage(hWnd, WM_KEYDOWN, VK_LEFT, 0);
            }

            if ((TilesArray[EmptyX + 1][EmptyY].TileRect.left <= xMouse) && (xMouse <= TilesArray[EmptyX + 1][EmptyY].TileRect.right) &&
                (TilesArray[EmptyX + 1][EmptyY].TileRect.top <= yMouse) && (yMouse <= TilesArray[EmptyX + 1][EmptyY].TileRect.bottom) && EmptyX < 3)
            {
                SendMessage(hWnd, WM_KEYDOWN, VK_UP, 0);
            }

            if ((TilesArray[EmptyX][EmptyY - 1].TileRect.left <= xMouse) && (xMouse <= TilesArray[EmptyX][EmptyY - 1].TileRect.right) &&
                (TilesArray[EmptyX][EmptyY - 1].TileRect.top <= yMouse) && (yMouse <= TilesArray[EmptyX][EmptyY - 1].TileRect.bottom) && EmptyY > 0)
            {
                SendMessage(hWnd, WM_KEYDOWN, VK_RIGHT, 0);
            }

            if ((TilesArray[EmptyX - 1][EmptyY].TileRect.left <= xMouse) && (xMouse <= TilesArray[EmptyX - 1][EmptyY].TileRect.right) &&
                (TilesArray[EmptyX - 1][EmptyY].TileRect.top <= yMouse) && (yMouse <= TilesArray[EmptyX - 1][EmptyY].TileRect.bottom) && EmptyX > 0)
            {
                SendMessage(hWnd, WM_KEYDOWN, VK_DOWN, 0);
            }

        }
        break;

        //case WM_NOTIFY:
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
            case ID_RESTART:
                MovesCount = 0;
                //GenerateField();
                //CreateField();


                SendMessage(hWnd, WM_CREATE, 0, 0);
                SendMessage(hWnd, WM_PAINT, 0, 0);

                InvalidateRect(hWnd, 0, 0);

                SetFocus(hWnd);

                break;

            case ID_SOLVE:
                SolvePuzzle();
                CreateField();
                //SendMessage(hWnd, WM_CREATE, 0, 0);
                SendMessage(hWnd, WM_PAINT, 0, 0);

                InvalidateRect(hWnd, 0, 0);
                SetFocus(hWnd);

                break;


            case ID_QUIT:
                SendMessage(hWnd, WM_DESTROY, 0, 0);
                return 0;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            //default:
            //    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

        case WM_PAINT:
        {
            InvalidateRect(hWnd, &ClientArea, 0);

            hdc = BeginPaint(hWnd, &ps);

            // TODO: Add any drawing code that uses hdc here...

            DrawField(hdc);

            wsprintf(bufer7, TEXT("Moves made:\n %i"), MovesCount);

            CreateWindowW(WC_STATIC, bufer7, WS_CHILD | WS_VISIBLE
                | SS_CENTER | SS_CENTERIMAGE, 0, 800, WIDTH, WIDTH / 2, hWnd, (HMENU)10003, NULL, NULL);

            InvalidateRect(hWnd, 0, 0);

            EndPaint(hWnd, &ps);
        }
        break;

        case WM_KEYDOWN:

            switch (wParam)                              //В зависимости от нажатой клавиши (от ее целочисленного кода) двигаем костяшки
            {
                case VK_LEFT:
                    Move(LEFT);
                    CreateField();
                    //FieldIsCorrect();

                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                    Sleep(SLEEPTIMER);
                break;

                case VK_UP:                     //Нажата клавиша "Вверх"
                    Move(UP);
                    CreateField();
                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                    Sleep(SLEEPTIMER);

                break;
                 
                case VK_RIGHT:                  //Нажата клавиша "Вправо"
                    Move(RIGHT);
                    CreateField();
                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                    Sleep(SLEEPTIMER);

                break;

                case VK_DOWN:                   //Нажата клавиша "Вниз"
                    Move(DOWN);
                    CreateField();
                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                    Sleep(SLEEPTIMER);

                break;

                case VK_SPACE:
                    SolvePuzzle();
                    CreateField();
                    SendMessage(hWnd, WM_PAINT, 0, 0);
                    InvalidateRect(hWnd, 0, 0);
                    break;

                case VK_ESCAPE:                 //Нажата клавиша "Escape" 
                    SendMessage(hWnd, WM_CREATE, 0, 0);
                    SendMessage(hWnd, WM_PAINT, 0, 0);
                    InvalidateRect(hWnd, 0, 0);
                break; 
            }

            if (FieldIsCorrect())
            {
                int msgboxID = MessageBox(
                    hWnd,
                    (LPCWSTR)L"Wow! Congratulations!\nWanna once more?",
                    (LPCWSTR)L"Puzzle 15",
                    MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON1);

                switch (msgboxID)
                {
                    case IDYES:
                    {
                        SendMessage(hWnd, WM_CREATE, 0, 0);
                        SendMessage(hWnd, WM_PAINT, 0, 0);
                        InvalidateRect(hWnd, 0, 0);
                    }
                        break;

                    case IDNO:
                        SendMessage(hWnd, WM_DESTROY, 0, 0);
                        break;
                    }

                return msgboxID;

            }

            break;

        case WM_DESTROY:
            PostQuitMessage(0);
        break;

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
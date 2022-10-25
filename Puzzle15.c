#include <stdio.h>
#include <stdlib.h> //—одержит функции randomize и random

#include "windows.h"
#include "commctrl.h"
#include "Puzzle15.h"
//#include "methods.c"
#pragma comment(lib,"comctl32")
////
////#pragma comment(linker,"/manifestdependency:\"type='win64' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x64' publicKeyToken='6595b64144ccf1df' language='*'\"") 

#define MAX_LOADSTRING 100
#define SIZE 16
#define WIDTH 200
#define SPACE 0
#define ID_RESTART 10001
#define ID_QUIT 10002
#define ID_SOLVE 10003
#define ID_PRGBAR 10004
#define DARKTHEME RGB(100, 100, 100)
#define SLEEPTIMER 200

typedef struct
{
    RECT TileRect;
    HBITMAP Pic;
    int TileNum;
} Tile;

// Global Variables:

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"The Puzzle of 15";                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("GameWnd");            // the main window class name

HWND hwndPrgBar, hwndMovesCount;

RECT ClientArea;                    //size of client area in window
Tile TilesArray[4][4];               //array of tiles for field
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

    int NumIsFree[15]; //NumIsFree[i] показывает, определили ли мы уже позицию i-й кост€шки
    int Nums[15]; //Nums[i] содержит номер кост€шки, наход€щейс€ в i-й позиции
    for (int i = 0; i < 15; i++) //ќбъ€вл€ем, что изначально все кост€шки свободны
        NumIsFree[i] = TrueVar;
    srand((unsigned)time(0)); //randomize позвол€ет при каждом прогоне программы получать разные последовательности псевдослучайных чисел
    int Flag; //‘лаг, определ€ющий корректность выбора кост€шки дл€ данной позиции
    int RandNum; //Ќомер кост€шки, генерируемый в дальнейшем случайным образом
    for (int i = 0; i < 15; i++) //Ќам надо определить номер кост€шки, наход€щейс€ в каждой из 15 позиций пол€
    {
        Flag = FalseVar; // аждый раз сбрасываем значение флага
        while (!Flag) //ѕродолжаем случайным образом определ€ть номер кост€шки, пока он не окажетс€ корректным
        {
            RandNum = (rand() % 15) + 1; //random(n) генерирует псевдослучайное число от 0 до n - 1, а нам нужно от 1 до 15
            if (NumIsFree[RandNum - 1]) //≈сли кост€шка с таким номером еще свободна (помним, что массивы нумеруютс€ начина€ с нул€)
                Flag = TrueVar; //то мы определили ее номер корректно
        }
        Nums[i] = RandNum; //«аписываем этот корректный номер в i-ю позицию
        NumIsFree[RandNum - 1] = FalseVar; // ост€шка с этим номером теперь зан€та
    }

    int Chaos = 0; // оличество беспор€дков на поле
    int CurrNum; // ост€шка, дл€ которой мы рассматриваем беспор€дки
    for (int i = 0; i < 14; i++) //—читаем дл€ кост€шек на первых 14 позици€х (дл€ 15-й это бессмысленно)
    {
        CurrNum = Nums[i];
        for (int j = i + 1; j < 15; j++)
            if (CurrNum > Nums[j])
                Chaos++;
    }
    if (Chaos % 2 == 1) //≈сли общее число беспор€дков нечетное,
    { //мен€ем местами кост€шки на 14-й и 15-й позици€х
        int temp = Nums[13];
        Nums[13] = Nums[14];
        Nums[14] = temp;
    }

    for (int i = 0; i < 4; i++)
    {   
        for (int j = 0; j < 4; j++)
        {
            TilesArray[i][j].TileNum = Nums[counter]; //a % b - остаток от делени€ a на b
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

                //wsprintf(bufer, TEXT("d:\\sources\\C\\Puzzle15\\sources\\tile%i.bmp"), TilesArray[i][j].TileNum);

                TilesArray[i][j].Pic = LoadImage(hInst, IDB_BITMAP1 + TilesArray[i][j].TileNum - 1, IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
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
                return FALSE;//a % b - остаток от делени€ a на b
            counter++;
        }
    }
    return TRUE;
}

void CheckForProgress()
{
    int counter = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((TilesArray[i][j].TileNum == 1 + j + 4 * i) && TilesArray[i][j].TileNum != 16)
            {
                counter++;
            }
        }
    }

    SendMessage(hwndPrgBar, PBM_SETPOS, counter, 0);

}

void DrawField(HDC hdc)
{
    BITMAP bitmap;
    HGDIOBJ oldBitmap;

    wchar_t bufer[3];

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP memBM = CreateCompatibleBitmap(hdc, ClientArea.right - ClientArea.left, ClientArea.bottom - ClientArea.top);

    SelectObject(hdcMem, memBM);

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

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //while (GetMessage(&msg, NULL, 0, 0))
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}

    //return (int)msg.wParam;
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON5));
    wcex.hCursor = LoadCursor(NULL, IDC_HAND);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PUZZLE15);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON5));



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

    //SendMessage(hWnd, WM_SETICON, ICON_BIG, (LONG)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3)));
    //SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LONG)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3)));

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
    PAINTSTRUCT ps;
    HDC hdc;
    HFONT hFont, holdFont;

    wchar_t buf[30];

    int xMouse, yMouse;

    switch (message)
    {
        case WM_CREATE:

            GenerateField();
            CreateField();
            CheckForProgress();
            InitCommonControls();

            CreateWindowW(                                      //"Solve" button
                L"button", L"Solve",
                WS_VISIBLE | WS_CHILD,
                200, 800, WIDTH, WIDTH / 2,
                hWnd, (HMENU)ID_SOLVE, NULL, NULL);

            CreateWindowW(                                      //"Restart" button
                L"button", L"Restart",
                WS_VISIBLE | WS_CHILD,
                400, 800, WIDTH, WIDTH / 2,
                hWnd, (HMENU) ID_RESTART, NULL, NULL);

            CreateWindowW(                                      //"Quit" button
                L"button", L"Quit",
                WS_VISIBLE | WS_CHILD,
                600, 800, WIDTH, WIDTH / 2,
                hWnd, (HMENU) ID_QUIT, NULL, NULL);

            hwndMovesCount = CreateWindow(                      //Static field fot moves counter
                WC_STATIC, L"Press any arrow",
                WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
                0, 800, WIDTH, 3 * WIDTH / 8, hWnd, (HMENU)10003, hInst, NULL);

            hwndPrgBar = CreateWindowEx(0,                      //Progress bar to show a progress of the solving puzzle
                PROGRESS_CLASS, NULL,
                WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                0, 875, WIDTH, WIDTH / 8, hWnd, (HMENU)ID_PRGBAR, hInst, NULL);

            SendMessage(hwndPrgBar, PBM_SETRANGE, 0, MAKELPARAM(0, 15));
            SendMessage(hwndPrgBar, PBM_SETSTEP, (WPARAM)1, 0);
            SendMessage(hwndPrgBar, PBM_SETPOS, 0, 0);

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

            //if ((400 <= xMouse) && (xMouse <= 600) && (600 <= yMouse) && (yMouse <= 800)) 100 < x < 200


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

        case WM_COMMAND:                                // Parse the menu selections:
        {
            int wmId = LOWORD(wParam);
            
            switch (wmId)
            {
            case ID_RESTART:
                MovesCount = 0;
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

            case IDM_FILE_SOLVE:
                SendMessage(hWnd, WM_COMMAND, ID_SOLVE, 0);
                break;

            case IDM_FILE_RESTART:
                SendMessage(hWnd, WM_COMMAND, ID_RESTART, 0);
                break;

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
            InvalidateRect(hWnd, 0, 0);

            hdc = BeginPaint(hWnd, &ps);

            hFont = CreateFontW(28, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Calibri");

            holdFont = SelectObject(hdc, hFont);


            DrawField(hdc);
            InvalidateRect(hWnd, &ClientArea, 0);
            EndPaint(hWnd, &ps);
        }
        break;

        case WM_KEYDOWN:

            switch (wParam)                              //Parse the arrow pressed and making the appropriate move
            {
                case VK_LEFT:                           //Arrow Left pressed
                    Move(LEFT);
                    CreateField();
                    wsprintf(buf, TEXT("Moves made:\n %i"), MovesCount);
                    SendMessageW(hwndMovesCount, WM_SETTEXT, (WPARAM)NULL, (LPARAM)buf);
                    CheckForProgress();
                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                    Sleep(SLEEPTIMER);
                break;

                case VK_UP:                             //Arrow Up pressed
                    Move(UP);
                    CreateField();
                    wsprintf(buf, TEXT("Moves made:\n %i"), MovesCount);
                    SendMessageW(hwndMovesCount, WM_SETTEXT, (WPARAM)NULL, (LPARAM)buf);
                    CheckForProgress();
                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                    Sleep(SLEEPTIMER);

                break;
                 
                case VK_RIGHT:                          //Arrow Right pressed
                    Move(RIGHT);
                    CreateField();

                    wsprintf(buf, TEXT("Moves made:\n %i"), MovesCount);
                    SendMessageW(hwndMovesCount, WM_SETTEXT, (WPARAM)NULL, (LPARAM)buf);
                    CheckForProgress();
                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                    Sleep(SLEEPTIMER);

                break;

                case VK_DOWN:                          //Arrow Down pressed
                    Move(DOWN);
                    CreateField();
                    wsprintf(buf, TEXT("Moves made:\n %i"), MovesCount);
                    SendMessageW(hwndMovesCount, WM_SETTEXT, (WPARAM)NULL, (LPARAM)buf);
                    CheckForProgress();
                    SendMessage(hWnd, WM_PAINT, wParam, lParam);
                    InvalidateRect(hWnd, 0, 0);
                    Sleep(SLEEPTIMER);

                break;

                case VK_SPACE:                          //In case of Space is pressed - restart the game
                    SolvePuzzle();
                    CreateField();
                    SendMessage(hWnd, WM_PAINT, 0, 0);
                    InvalidateRect(hWnd, 0, 0);
                    break;

                case VK_ESCAPE:                         //In case of ESC pressed - close the window and exit 
                    SendMessage(hWnd, WM_CREATE, 0, 0);
                    SendMessage(hWnd, WM_PAINT, 0, 0);
                    InvalidateRect(hWnd, 0, 0);
                break; 
            }

            if (FieldIsCorrect())                       //Checking if the field is correct and if so - the message box with grats and 2 buttons is appeared
            {
                int msgboxID = MessageBox(
                    hWnd,
                    (LPCWSTR)L"Wow! Congratulations!\nWanna once more?",
                    (LPCWSTR)L"Puzzle 15",
                    MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON1);

                switch (msgboxID)
                {
                    case IDYES:                         //Yes button - restarting a game
                    {
                        SendMessage(hWnd, WM_CREATE, 0, 0);
                        SendMessage(hWnd, WM_PAINT, 0, 0);
                        InvalidateRect(hWnd, 0, 0);
                    }
                        break;

                    case IDNO:                          //No button - close & exit
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
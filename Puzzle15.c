#include "framework.h"
#include "Puzzle15.h"
#include "methods.c"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PUZZLE15, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    int cxScreen, cyScreen;

    cxScreen = GetSystemMetrics(SM_CXSCREEN);
    cyScreen = GetSystemMetrics(SM_CYSCREEN);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PUZZLE15));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PUZZLE15));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PUZZLE15);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 870, 910, NULL, NULL, hInstance, NULL);

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
    static HWND      tile1, tile2, tile3, tile4, tile5, tile6, tile7, tile8, tile9, tile10, tile11, tile12, tile13, tile14, tile15, tile16;

    switch (message)
    {
    case WM_CREATE:

        tile1 = CreateWindow(
            L"button", L"",
            WS_VISIBLE | WS_CHILD,
            10, 10, 200, 200,
            hWnd, (HMENU) 1001, NULL, NULL);

        tile2 = CreateWindow(
            L"button", L"Tile 2",
            WS_VISIBLE | WS_CHILD,
            220, 10, 200, 200,
            hWnd, (HMENU) 1002, NULL, NULL);

        tile3 = CreateWindow(
            L"button", L"Tile 3",
            WS_VISIBLE | WS_CHILD,
            430, 10, 200, 200,
            hWnd, (HMENU) 1003, NULL, NULL);

        tile4 = CreateWindow(
            L"button", L"Tile 4",
            WS_VISIBLE | WS_CHILD,
            640, 10, 200, 200,
            hWnd, (HMENU) 1004, NULL, NULL);

        tile5 = CreateWindow(
            L"button", L"Tile 5",
            WS_VISIBLE | WS_CHILD,
            10, 220, 200, 200,
            hWnd, (HMENU)1005, NULL, NULL);

        tile6 = CreateWindow(
            L"button", L"Tile 6",
            WS_VISIBLE | WS_CHILD,
            220, 220, 200, 200,
            hWnd, (HMENU)1006, NULL, NULL);

        tile7 = CreateWindow(
            L"button", L"Tile 7",
            WS_VISIBLE | WS_CHILD,
            430, 220, 200, 200,
            hWnd, (HMENU)1007, NULL, NULL);

        tile8 = CreateWindow(
            L"button", L"Tile 8",
            WS_VISIBLE | WS_CHILD,
            640, 220, 200, 200,
            hWnd, (HMENU)1008, NULL, NULL);

        tile9 = CreateWindow(
            L"button", L"Tile 9",
            WS_VISIBLE | WS_CHILD,
            10, 430, 200, 200,
            hWnd, (HMENU)1009, NULL, NULL);

        tile10 = CreateWindow(
            L"button", L"Tile 10",
            WS_VISIBLE | WS_CHILD,
            220, 430, 200, 200,
            hWnd, (HMENU)1010, NULL, NULL);

        tile11 = CreateWindow(
            L"button", L"Tile 11",
            WS_VISIBLE | WS_CHILD,
            430, 430, 200, 200,
            hWnd, (HMENU)1011, NULL, NULL);

        tile12 = CreateWindow(
            L"button", L"Tile 12",
            WS_VISIBLE | WS_CHILD,
            640, 430, 200, 200,
            hWnd, (HMENU)1012, NULL, NULL);

        tile13 = CreateWindow(
            L"button", L"Tile 13",
            WS_VISIBLE | WS_CHILD,
            10, 640, 200, 200,
            hWnd, (HMENU)1013, NULL, NULL);

        tile14 = CreateWindow(
            L"button", L"Tile 14",
            WS_VISIBLE | WS_CHILD,
            220, 640, 200, 200,
            hWnd, (HMENU)1014, NULL, NULL);

        tile15 = CreateWindow(
            L"button", L"Tile 15",
            WS_VISIBLE | WS_CHILD,
            430, 640, 200, 200,
            hWnd, (HMENU)1015, NULL, NULL);

        tile16 = CreateWindow(
            L"button", L"Empty",
            WS_CHILD,
            640, 640, 200, 200,
            hWnd, (HMENU)1000, NULL, NULL);

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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;

/*
    case WM_DRAWITEM:
        pdis = (LPDRAWITEMSTRUCT)lParam;

        // Fill area with white and frame it black

        FillRect(pdis->hDC, &pdis->rcItem,
            (HBRUSH)GetStockObject(WHITE_BRUSH));

        FrameRect(pdis->hDC, &pdis->rcItem,
            (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Draw inward and outward black triangles
        cx = pdis->rcItem.right - pdis->rcItem.left;
        cy = pdis->rcItem.bottom - pdis->rcItem.top;

        switch (pdis->CtlID)
        {
        case 1001:
            pt[0].x = 3 * cx / 8;  pt[0].y = 1 * cy / 8;
            pt[1].x = 5 * cx / 8;  pt[1].y = 1 * cy / 8;
            pt[2].x = 4 * cx / 8;  pt[2].y = 3 * cy / 8;

            Triangle(pdis->hDC, pt);

            pt[0].x = 7 * cx / 8;  pt[0].y = 3 * cy / 8;
            pt[1].x = 7 * cx / 8;  pt[1].y = 5 * cy / 8;
            pt[2].x = 5 * cx / 8;  pt[2].y = 4 * cy / 8;

            Triangle(pdis->hDC, pt);

            pt[0].x = 5 * cx / 8;  pt[0].y = 7 * cy / 8;
            pt[1].x = 3 * cx / 8;  pt[1].y = 7 * cy / 8;
            pt[2].x = 4 * cx / 8;  pt[2].y = 5 * cy / 8;

            Triangle(pdis->hDC, pt);

            pt[0].x = 1 * cx / 8;  pt[0].y = 5 * cy / 8;
            pt[1].x = 1 * cx / 8;  pt[1].y = 3 * cy / 8;
            pt[2].x = 3 * cx / 8;  pt[2].y = 4 * cy / 8;

            Triangle(pdis->hDC, pt);
            break;
        }

        // Invert the rectangle if the button is selected

        if (pdis->itemState & ODS_SELECTED)
            InvertRect(pdis->hDC, &pdis->rcItem);

        // Draw a focus rectangle if the button has the focus

        if (pdis->itemState & ODS_FOCUS)
        {
            pdis->rcItem.left += cx / 16;
            pdis->rcItem.top += cy / 16;
            pdis->rcItem.right -= cx / 16;
            pdis->rcItem.bottom -= cy / 16;

            DrawFocusRect(pdis->hDC, &pdis->rcItem);
        }
        return 0;
*/
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
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
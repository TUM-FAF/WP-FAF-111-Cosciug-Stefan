#include <windows.h>
#include <iostream>
#include <cstdio>
#include <string.h>





/*///////////////////////////////////////////
1.Function prototypes & global declaratons://
///////////////////////////////////////////*/


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define WndWidth  640
#define WndHeight 460
HWND hwndMain;
PAINTSTRUCT ps;
HDC hdc;
HPEN hPenOld;
RECT GeneralPurposeRect;
int DrawStartupObjects = 1;





/*///////////////////////
//2. The Main Function://
///////////////////////*/


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{

    // Declarations:
    MSG msg;
    WNDCLASSEX wcx;


    // Defining Window Style:
    wcx.cbSize = sizeof(WNDCLASSEX);                                // Must always be sizeof(WNDCLASSEX)
    wcx.style = CS_DBLCLKS;                                         // Class styles
    wcx.lpfnWndProc = MainWndProc;                                  // Pointer to callback procedure
    wcx.cbClsExtra = 0;                                             // Extra bytes to allocate following the wndclassex structure
    wcx.cbWndExtra = 0;                                             // Extra bytes to allocate following an instance of the structure
    wcx.hInstance = hInst;                                          // Instance of the application
    wcx.hIcon = LoadIcon(NULL, IDI_WARNING);                        // Class Icon
    wcx.hCursor = LoadCursor(NULL, IDI_APPLICATION);                // Class cursor
    wcx.hbrBackground = (HBRUSH) COLOR_BACKGROUND;                  // Background brush
    wcx.lpszMenuName = NULL;                                        // Menu Resource
    wcx.lpszClassName = "MainWindow";                               // Name of this class
    wcx.hIconSm = LoadIcon(NULL, IDI_WARNING);                      // Small icon for this class


    // Register this window class with MS-Windows
    if (!RegisterClassEx(&wcx)) return 0;


    // Creating the main window:
    hwndMain = CreateWindow(  "MainWindow",                         // Window class name
                              "Laboratory Work #2",                 // Window title
                              WS_OVERLAPPEDWINDOW,                  // Window style
                              CW_USEDEFAULT, CW_USEDEFAULT,         // (x,y) pos of the window
                              WndWidth, WndHeight,                  // Width and height of the window
                              HWND_DESKTOP,                         // HWND of the parent window (can be null also)
                              NULL,                                 // Handle to menu
                              hInst,                                // Handle to application instance
                              NULL);
    // Checking if window creation was successful
    if (!hwndMain) return 0;


    // Make the window visible
    ShowWindow(hwndMain,SW_SHOW);


    // Process messages coming to this window
    while (GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}





/*///////////////
//3. Functions://
///////////////*/


//Main window procedure
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
        break;


        // Main objects initialization
        case WM_CREATE:
        break;


        //Window min/max limits and dependancy on resize
        case WM_GETMINMAXINFO:
        {
            MINMAXINFO *lpMinMaxInfo;
            lpMinMaxInfo = (MINMAXINFO*) lParam;

            // Making the main window non resizable
            lpMinMaxInfo->ptMinTrackSize.x = WndWidth;
            lpMinMaxInfo->ptMinTrackSize.y = WndHeight;
            lpMinMaxInfo->ptMaxTrackSize.x = WndWidth;
            lpMinMaxInfo->ptMaxTrackSize.y = WndHeight;
        }
        break;


        // Buttons
        case WM_COMMAND:
        {

        }
        break;


        // Text positioning and fonts application
        case WM_PAINT:
        {
            hdc = BeginPaint(hwnd, &ps);

            //General declarations
            HPEN hPen;
            COLORREF qColor;
            HBRUSH HatchBrush;

            // Creating the working area
            qColor = RGB(0, 0, 0);
            hPen = CreatePen(PS_SOLID, 2, qColor);
            hPenOld = (HPEN)SelectObject(hdc, hPen);
            Rectangle(hdc, 0, 20, 420, 400);
            SelectObject (hdc, hPenOld);
            DeleteObject(hPen);
            GeneralPurposeRect.left = 0;
            GeneralPurposeRect.top = 0;
            GeneralPurposeRect.right = 420;
            GeneralPurposeRect.bottom = 400;
            DrawText(hdc, "Working area:" , lstrlen("Working area:"), &GeneralPurposeRect, DT_CENTER);

            if(DrawStartupObjects == 1)
            {
                // Drawing a red line
                qColor = RGB(255, 0, 0);
                hPen = CreatePen(PS_SOLID, 7, qColor);
                hPenOld = (HPEN)SelectObject(hdc, hPen);
                MoveToEx(hdc, 50, 50, NULL);
                LineTo(hdc, 100, 50);
                DeleteObject(hPen);

                // Drawing a blue line
                qColor = RGB(0, 0, 255);
                hPen = CreatePen(PS_SOLID, 5, qColor);
                SelectObject(hdc, hPen);
                MoveToEx(hdc, 50, 70, NULL);
                LineTo(hdc, 100, 70);
                SelectObject(hdc, hPenOld);
                DeleteObject(hPen);

                // Drawing a green circle
                qColor = RGB(0, 255, 0);
                hPen = CreatePen(PS_SOLID, 3, qColor);
                hPenOld = (HPEN)SelectObject(hdc, hPen);
                Arc(hdc, 100, 100, 150, 150, 0, 0, 0, 0);
                SelectObject (hdc, hPenOld);
                DeleteObject(hPen);

                // Drawing a filled, violet rectangle
                qColor = RGB(170, 110, 200);
                hPen = CreatePen(PS_SOLID, 6, qColor);
                hPenOld = (HPEN)SelectObject(hdc, hPen);
                Rectangle(hdc, 130, 50, 200, 80);
                GeneralPurposeRect.left = 130;
                GeneralPurposeRect.top = 50;
                GeneralPurposeRect.right = 200;
                GeneralPurposeRect.bottom = 80;
                HatchBrush = CreateHatchBrush(HS_HORIZONTAL, qColor);
                FillRect(hdc, &GeneralPurposeRect, HatchBrush);
                SelectObject (hdc, hPenOld);
                DeleteObject(hPen);
                DeleteObject(HatchBrush);
            }

            EndPaint (hwnd, &ps);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);




        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }


return 0;
}

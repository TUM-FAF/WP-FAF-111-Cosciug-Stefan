#include <windows.h>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Ring.cpp"





/*///////////////////////////////////////////
1.Function prototypes & global declaratons://
///////////////////////////////////////////*/

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Initialization(void);

#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

#define WndWidth  1024
#define WndHeight 768
#define IDT_TIMER1 10
HWND hwndMain;
HDC hdc;
PAINTSTRUCT ps;
HPEN WndFramePen;                                                      // A pen dedicated to draw frames
int ClicksCounter = 0;                                                 // Based on the number of clicks, the program determines what figure needs to be drawn
int NrRings = 0;
Ring* B = new Ring[20];                                                // Keeps the information required to draw a Ring
RECT WorkingAreaRect;
int SpeedRed = 950;                                                    // The value that will be subtracted from 1000. Result will be the actual speed
// Double buffering
static HBITMAP hbmMem;
static HANDLE hOld;
static HDC hdcMem;






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
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);        // Background brush
    wcx.lpszMenuName = NULL;                                        // Menu Resource
    wcx.lpszClassName = "MainWindow";                               // Name of this class
    wcx.hIconSm = LoadIcon(NULL, IDI_WARNING);                      // Small icon for this class

    // Register this window class with MS-Windows
    if (!RegisterClassEx(&wcx)) return 0;

    // Creating the main window:
    hwndMain = CreateWindow(  "MainWindow",                         // Window class name
                              "Laboratory Work #4",                 // Window title
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

    // Performing program entry point operations
    Initialization();

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

// Main window procedure
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
        break;

        // Main objects initialization
        case WM_CREATE:
        {
            // Creating a compatible bitmap for double buffering
            hdc=GetDC(hwnd);
            hdcMem=CreateCompatibleDC(hdc);
            hbmMem=CreateCompatibleBitmap(hdc,1024,768);
            hOld = SelectObject(hdcMem,hbmMem);
            ReleaseDC(hwnd, hdc);

            // Setting up a timer
            SetTimer(hwnd, IDT_TIMER1, 50, (TIMERPROC) NULL);
        }
        break;

        // Window min/max limits and dependancy on resize
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

        case WM_TIMER:
        {
            switch (wParam)
            {
                case IDT_TIMER1:
                    {
                        for (int i=0; i<NrRings; i++)
                        {
                            // Calculating new coordinates using the angle
                            double y = sin(B[i].Direction*3.14/180.0);
                            double x = cos(B[i].Direction*3.14/180.0);

                            // Computing the angle on contact with the window frame
                            if (B[i].Coord.right+x >= WorkingAreaRect.right)
                            {
                                if (B[i].Direction < 90.0)
                                {
                                    B[i].Direction = 180.0-B[i].Direction;
                                }
                                else if (B[i].Direction > 270.0)
                                {
                                    B[i].Direction = 180.0+(360.0-B[i].Direction);
                                }
                            }
                            else if (B[i].Coord.top-y <= WorkingAreaRect.top)
                            {
                                B[i].Direction = 360.0-B[i].Direction;
                            }
                            else if (B[i].Coord.left+x <= WorkingAreaRect.left)
                            {
                                if (B[i].Direction<270.0 && B[i].Direction>180.0)
                                {
                                    B[i].Direction = 360.0-(B[i].Direction-180.0);
                                }
                                else if (B[i].Direction>90.0 && B[i].Direction<=180.0)
                                {
                                    B[i].Direction = 180.0-B[i].Direction;
                                }
                            }
                            else if (B[i].Coord.bottom-y >= WorkingAreaRect.bottom)
                            {
                                B[i].Direction = 360.0-B[i].Direction;
                            }

                            // Computing coordinates for the center of the local Ring
                            double CenterLocX = B[i].Coord.left+(B[i].Diameter)/2;
                            double CenterLocY = B[i].Coord.top+(B[i].Diameter)/2;

                            // Comparing Ring coordinates for determining the moment of interaction
                            for (int j=0; j<NrRings; j++)
                            {
                                // Computing coordinates for the center of the remote Ring
                                double CenterRemX = B[j].Coord.left+(B[j].Diameter)/2.0;
                                double CenterRemY = B[j].Coord.top+(B[j].Diameter)/2.0;

                                if (j!=i)
                                {
                                    if (sqrt((abs(CenterLocX-CenterRemX)*abs(CenterLocX-CenterRemX))+
                                             (abs(CenterLocY-CenterRemY)*abs(CenterLocY-CenterRemY)))<((B[i].Diameter+B[j].Diameter)/2.0))
                                    {
                                        B[i].Color.Red = rand() % 255;
                                        B[i].Color.Green = rand() % 255;
                                        B[i].Color.Blue = rand() % 255;
                                    }
                                }
                            }
                            // Updating the coordinates
                            B[i].Coord.left = B[i].Coord.left+x;
                            B[i].Coord.right = B[i].Coord.right+x;
                            B[i].Coord.top = B[i].Coord.top-y;
                            B[i].Coord.bottom = B[i].Coord.bottom-y;
                        }

                        InvalidateRect(hwnd, &WorkingAreaRect, FALSE);
                    }
            }
        }
        break;

        case WM_MOUSEWHEEL:
        {
            if((short)HIWORD(wParam)>0)
            {
                SpeedRed = SpeedRed+10;
                if (SpeedRed>1000) SpeedRed=1000;
            }
            else
            {
                SpeedRed = SpeedRed-10;
                if (SpeedRed<0) SpeedRed=1;
            }
            KillTimer(hwnd, IDT_TIMER1);
            SetTimer(hwnd, IDT_TIMER1, 1000-SpeedRed, (TIMERPROC)NULL);
        }
        break;

        // System buttons
        case WM_SYSCOMMAND:
            switch(wParam)
            {
                // Disabling the "window maximize" functionality
                case SC_MAXIMIZE:
                break;

                default:
                    return DefWindowProc(hwnd, msg, wParam, lParam);
            }

        // Title bar double click event. Disabling.
        case WM_NCLBUTTONDBLCLK:
        break;

        // Left mouse button press event
        case WM_LBUTTONDOWN:
        {
            // General declarations
            float xPos = LOWORD(lParam);        // Horizontal position of cursor
            float yPos = HIWORD(lParam);        // Vertical position of cursor

            // Checking if cursor position isn't beyond the working area bounds
            if (xPos < WorkingAreaRect.left || xPos > WorkingAreaRect.right || yPos < WorkingAreaRect.top ||
                yPos > WorkingAreaRect.bottom) {break;}

            // Reloading objects array to prevent overflow
            if (NrRings == 20)
            {
                delete [] B;
                Ring* B = new Ring[20];
                NrRings = 0;
            }

            // Stores the coordinates of the first and second point
            if (ClicksCounter == 0)
            {
                B[NrRings].Diameter = 30.0;
                B[NrRings].Coord.left = xPos;
                B[NrRings].Coord.top = yPos;
                ClicksCounter++;
            }
            else if (ClicksCounter == 1)
            {
                B[NrRings].Coord.right = B[NrRings].Coord.left+B[NrRings].Diameter;
                B[NrRings].Coord.bottom = B[NrRings].Coord.top+B[NrRings].Diameter;

                if (B[NrRings].Coord.right >= WorkingAreaRect.right)
                {
                    B[NrRings].Coord.right = WorkingAreaRect.right-1;
                    B[NrRings].Coord.bottom = B[NrRings].Coord.top+(B[NrRings].Coord.right-B[NrRings].Coord.left);
                    B[NrRings].Diameter = B[NrRings].Coord.right-B[NrRings].Coord.left;
                }
                else if (B[NrRings].Coord.bottom >= WorkingAreaRect.bottom)
                {
                    B[NrRings].Coord.bottom = WorkingAreaRect.bottom-1;
                    B[NrRings].Coord.right = B[NrRings].Coord.left+(B[NrRings].Coord.bottom-B[NrRings].Coord.top);
                    B[NrRings].Diameter = B[NrRings].Coord.bottom-B[NrRings].Coord.top;
                }


                // Setting a random direction (angle)
                B[NrRings].Direction = (rand() % 358)+2;
                ClicksCounter++;
                InvalidateRect(hwnd, &WorkingAreaRect, FALSE);
            }
        }
        break;

        // Mouse move event
        case WM_MOUSEMOVE:
        {
            // General declarations
            float xPos = LOWORD(lParam);        // Horizontal position of cursor
            float yPos = HIWORD(lParam);        // Vertical position of cursor

            // Current position of the cursor. Will be used in 'WM_PAINT' for real-time view of the figure during it's drawing
            if (ClicksCounter == 1)
            {
                // Calculating and storing diameter of the Ring and the endpoint
                if (max(xPos-B[NrRings].Coord.left, yPos-B[NrRings].Coord.top)>30.0
                    && max(xPos-B[NrRings].Coord.left, yPos-B[NrRings].Coord.top)<200.0)
                {
                    // Storing the diameter of the Ring
                    B[NrRings].Diameter = max(xPos-B[NrRings].Coord.left, yPos-B[NrRings].Coord.top);

                    InvalidateRect(hwnd, &WorkingAreaRect, FALSE);
                }
            }
        }
        break;

        // Text positioning and fonts application
        case WM_PAINT:
        {
            //General declarations
            hdc = BeginPaint(hwnd, &ps);

            // Creating the working area
            SelectObject(hdcMem, WndFramePen);
            Rectangle(hdcMem, WorkingAreaRect.left, WorkingAreaRect.top, WorkingAreaRect.right, WorkingAreaRect.bottom);
            FillRect(hdcMem, &WorkingAreaRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

            // Block of code responsible for real-time view of the figure during it's drawing
            if (ClicksCounter == 1)
            {
                Arc(hdcMem, B[NrRings].Coord.left, B[NrRings].Coord.top, B[NrRings].Coord.left+B[NrRings].Diameter,
                    B[NrRings].Coord.top+B[NrRings].Diameter, 0, 0, 0, 0);
            }
            else if (ClicksCounter == 2)
            {

                NrRings++;
                ClicksCounter = 0;
            }

            // With this block of code, the recorded figures will be redrawn each time window refresh occurs
            for (int i=0; i<NrRings; i++)
            {
                COLORREF qColor = RGB(B[i].Color.Red, B[i].Color.Green, B[i].Color.Blue);
                HPEN hPen = CreatePen(PS_SOLID, 5, qColor);
                SelectObject(hdcMem, hPen);

                Arc(hdcMem, B[i].Coord.left, B[i].Coord.top, B[i].Coord.right, B[i].Coord.bottom, 0, 0, 0, 0);

                // Removing unused varaibles, restoring to initial state
                SelectObject(hdcMem, WndFramePen);
                DeleteObject(hPen);
            }

            // Showing the bitmap on the screen, ending paint
            BitBlt(hdc,0,0,1024,768,hdcMem,0,0,SRCCOPY);
            EndPaint(hwnd, &ps);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

return 0;
}


void Initialization(void)
{
    // Setting the working area rectangle coordinates
    WorkingAreaRect.left = 20;
    WorkingAreaRect.top = 40;
    WorkingAreaRect.right = 996;
    WorkingAreaRect.bottom = 720;

    // Creating a pen for the working area rectangle frame
    COLORREF qColor = RGB(255, 255, 255);   // White
    WndFramePen = CreatePen(PS_SOLID, 5, qColor);

    // Using current time for first number generation
    time_t t;
    srand((unsigned) time(&t));
}




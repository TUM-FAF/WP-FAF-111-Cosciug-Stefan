#include <windows.h>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Ball.cpp"
#include "Dock.cpp"





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
#define MENU_NEWGAME 101
#define MENU_RANDOM 102
#define MENU_EXIT 103

HWND hwndMain;
HDC hdc;
PAINTSTRUCT ps;
HPEN WndFramePen;                                                       // A pen dedicated to draw frames
int ClicksCounter = 0;                                                  // Based on the number of clicks, the program determines what figure needs to be drawn
int NrBalls = 0;
Ball* B = new Ball[20];                                                 // Keeps the information required to draw a Ball
Dock D0(0);
Dock D1(1);
RECT WorkingAreaRect;
RECT ScoreboardRect;
int SpeedRed = 950;                                                     // The value that will be subtracted from 1000. Result will be the actual speed
int RightMost = 0;                                                      // Index of the right most ball. Will be used to determine the ball, that the "computer" dock should focus on
int PlayerScore[2];
RECT ScoreRectText[4];                                                  // Keeps the position coordinates where score data will be printed
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
                              "Laboratory Work #5",                 // Window title
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

    // Creating the menu
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, MENU_NEWGAME, "New Game");
    AppendMenu(hFileMenu, MF_STRING, MENU_RANDOM, "Random");
    AppendMenu(hFileMenu, MF_STRING, MENU_EXIT, "Exit");
    AppendMenu(hMenu, MF_STRING|MF_POPUP, (UINT)hFileMenu, "Game");
    SetMenu(hwndMain, hMenu);

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

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case MENU_EXIT:
                    PostQuitMessage(0);
                break;

                case MENU_NEWGAME:
                {
                    // Reloading objects array
                    delete [] B;
                    Ball* B = new Ball[20];
                    NrBalls = 0;
                    // Reseting recorded data
                    RightMost = 0;
                    PlayerScore[0] = 0; PlayerScore[1] = 0;
                    // Refreshing the screen
                    InvalidateRect(hwnd, &ScoreboardRect, FALSE);
                }
                break;

                case MENU_RANDOM:
                {
                    // Reloading objects array
                    delete [] B;
                    Ball* B = new Ball[20];
                    NrBalls = 0;
                    // Reseting recorded data
                    RightMost = 0;
                    PlayerScore[0] = 0; PlayerScore[1] = 0;
                    // Generating a random number
                    time_t t;
                    srand((unsigned) time(&t));
                    // Setting random data
                    NrBalls = 1+rand()%12;
                    for (int i=0; i<NrBalls; i++)
                    {
                        B[i].Diameter = 30.0+rand()%160;
                        double xPos = (B[i].Diameter+36.0)+rand()%530;
                        double yPos = (B[i].Diameter+41.0)+rand()%280;
                        B[i].Coord.left = xPos;
                        B[i].Coord.right = xPos+B[i].Diameter;
                        B[i].Coord.top = yPos;
                        B[i].Coord.bottom = yPos+B[i].Diameter;
                        // Setting a random direction (angle)
                        B[i].SetDirection((rand() % 358)+2);
                        printf("Impulse for ball %d is: (%f,%f) \n", NrBalls, B[NrBalls].Impulse.x, B[NrBalls].Impulse.y);
                    }
                    // Refreshing the screen
                    InvalidateRect(hwnd, &ScoreboardRect, FALSE);
                }
                break;
            }
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
                        // Block of code responsible for balls movement and interaction
                        for (int i=0; i<NrBalls; i++)
                        {
                            // Calculating new coordinates using the direction angle
                            double y = sin(B[i].Direction*3.14/180.0);
                            double x = cos(B[i].Direction*3.14/180.0);

                            // Determining the index of the right most ball. Will be used to determine the ball, that the "computer" dock should focus on
                            if (B[i].Coord.right>B[RightMost].Coord.right && (D1.Coord.left-B[i].Coord.right)>10.0)
                            {
                                RightMost = i;
                            }
                            else if((D1.Coord.left-B[RightMost].Coord.right)<0 && (D1.Coord.left-B[i].Coord.right)>10.0)
                            {
                                RightMost = i;
                            }

                            // Case when ball goes beyond the window right side bound
                            if (B[i].Coord.left >= WorkingAreaRect.right)
                            {
                                // Repositioning the objects in the center of the screen with a new direction angle
                                B[i].SetDirection((rand() % 358)+2);
                                printf("Impulse for the renewed ball %d is: (%f,%f) \n", i, B[i].Impulse.x, B[i].Impulse.y);
                                B[i].Coord.left = ((WorkingAreaRect.right-WorkingAreaRect.left)/2.0)-B[i].Diameter/2.0;
                                B[i].Coord.right = ((WorkingAreaRect.right-WorkingAreaRect.left)/2.0)+B[i].Diameter/2.0;
                                B[i].Coord.top = ((WorkingAreaRect.bottom-WorkingAreaRect.top)/2.0)-B[i].Diameter/2.0;
                                B[i].Coord.bottom = ((WorkingAreaRect.bottom-WorkingAreaRect.top)/2.0)+B[i].Diameter/2.0;

                                PlayerScore[0]++;
                                InvalidateRect(hwnd, &ScoreboardRect, FALSE);
                            }
                            // Case when ball contacts with the window upper boundary line
                            else if ((B[i].Coord.top-fabs(y)) <= WorkingAreaRect.top)
                            {
                                B[i].Direction = (360.0-B[i].Direction);
                                B[i].Impulse.y = B[i].Impulse.y * (-1.0);

                                // Correcting coordinates of the ball on contact with the window frame
                                B[i].Coord.top = WorkingAreaRect.top+(B[i].SpeedM*3.0)+1.0;
                                B[i].Coord.bottom = (B[i].Coord.top+B[i].Diameter);
                            }
                            // Case when ball goes beyond the window left side bound
                            else if (B[i].Coord.right <= WorkingAreaRect.left)
                            {
                                // Repositioning the objects in the center of the screen with a new direction angle
                                B[i].SetDirection((rand() % 358)+2);
                                printf("Impulse for ball %d is: (%f,%f) \n", i, B[i].Impulse.x, B[i].Impulse.y);
                                B[i].Coord.left = ((WorkingAreaRect.right-WorkingAreaRect.left)/2.0)-B[i].Diameter/2.0;
                                B[i].Coord.right = ((WorkingAreaRect.right-WorkingAreaRect.left)/2.0)+B[i].Diameter/2.0;
                                B[i].Coord.top = ((WorkingAreaRect.bottom-WorkingAreaRect.top)/2.0)-B[i].Diameter/2.0;
                                B[i].Coord.bottom = ((WorkingAreaRect.bottom-WorkingAreaRect.top)/2.0)+B[i].Diameter/2.0;

                                PlayerScore[1]++;
                                InvalidateRect(hwnd, &ScoreboardRect, FALSE);
                            }
                            // Case when ball contacts with the window lower boundary line
                            else if ((B[i].Coord.bottom+fabs(y)) >= WorkingAreaRect.bottom)
                            {
                                B[i].Direction = (360.0-B[i].Direction);
                                B[i].Impulse.y = B[i].Impulse.y * (-1.0);

                                // Correcting coordinates of the ball on contact with the window frame
                                B[i].Coord.bottom = WorkingAreaRect.bottom-(B[i].SpeedM*3.0)-1.0;
                                B[i].Coord.top = (B[i].Coord.bottom-B[i].Diameter);
                            }

                            // Computing coordinates for the center of the local ball. Local ball is with the index 'i'
                            double CenterLocX = B[i].Coord.left+(B[i].Diameter)/2;
                            double CenterLocY = B[i].Coord.top+(B[i].Diameter)/2;

                            // Computing dynamics between the local ball and dock surface
                            double CenterDockLX = (D0.Coord.right+D0.Coord.left)/2.0;
                            double CenterDockLY = (D0.Coord.bottom+D0.Coord.top)/2.0;
                            double CenterDockRX = (D1.Coord.right+D1.Coord.left)/2.0;
                            double CenterDockRY = (D1.Coord.bottom+D1.Coord.top)/2.0;
                            // If the ball's closest to the dock margin is inside the dock's coordinates rectangle then identify the dock involved
                            int DockID = 0;
                            if (B[i].Coord.left<=D0.Coord.right && B[i].Coord.top<D0.Coord.bottom && B[i].Coord.top>(D0.Coord.top-B[i].Diameter))
                            {
                                DockID = 1;
                            }
                            else if (B[i].Coord.right>=D1.Coord.left && B[i].Coord.top<D1.Coord.bottom && B[i].Coord.top>(D1.Coord.top-B[i].Diameter))
                            {
                                DockID = 2;
                            }
                            // Proceed with dynamics
                            if (DockID)
                            {
                                // Calculating the intersection coordinates
                                double IntX=0, IntY=0;  // Intersection coordinates along the X-axys and Y-axys
                                // Cycling along the dock's ellipse Y-axys to determine X as a function of Y
                                for (double j=0; j<81.0; j=j+1.0)
                                {
                                    if (IntX>0 && IntY>0) {break;}  // Interupting the loop if the intersection point was determined

                                    double dx, result;
                                    dx = sqrtf((1.0-j*j/6400.0)*225.0);     // 'dx' is a function of Y for the dock's ellipse
                                    for (int u=1; u<5; u++)
                                    {
                                        // 'u' is the quadrant supposed to check
                                        switch (u)
                                        {
                                            case 1:
                                            {
                                                if (DockID == 1)
                                                {
                                                    // result==1 means that the selected point that is a part of the dock's ellipse is also a part of the ball's ellipse
                                                    result = ((CenterDockLX+dx-CenterLocX)*(CenterDockLX+dx-CenterLocX)/(B[i].Diameter*B[i].Diameter/4.0)+
                                                              (CenterDockLY-j-CenterLocY)*(CenterDockLY-j-CenterLocY)/(B[i].Diameter*B[i].Diameter/4.0));

                                                    if (result < 1.01)
                                                    {
                                                        IntX = CenterDockLX+dx;
                                                        IntY = CenterDockLY-j;
                                                        printf("!x=%f,y=%f \n",CenterDockLX+dx,CenterDockLY-j);
                                                    }
                                                }
                                                else if (DockID == 2)
                                                {
                                                    // result==1 means that the selected point that is a part of the dock's ellipse is also a part of the ball's ellipse
                                                    result = ((CenterDockRX+dx-CenterLocX)*(CenterDockRX+dx-CenterLocX)/(B[i].Diameter*B[i].Diameter/4.0)+
                                                              (CenterDockRY-j-CenterLocY)*(CenterDockRY-j-CenterLocY)/(B[i].Diameter*B[i].Diameter/4.0));

                                                    if (result < 1.01)
                                                    {
                                                        IntX = CenterDockRX+dx;
                                                        IntY = CenterDockRY-j;
                                                        printf("!x=%f,y=%f \n",CenterDockRX+dx,CenterDockRY-j);
                                                    }
                                                }
                                            }
                                            break;
                                            case 2:
                                            {
                                                if (DockID == 1)
                                                {
                                                    result = ((CenterDockLX-dx-CenterLocX)*(CenterDockLX-dx-CenterLocX)/(B[i].Diameter*B[i].Diameter/4.0)+
                                                              (CenterDockLY-j-CenterLocY)*(CenterDockLY-j-CenterLocY)/(B[i].Diameter*B[i].Diameter/4.0));

                                                    if (result < 1.01)
                                                    {
                                                        IntX = CenterDockLX-dx;
                                                        IntY = CenterDockLY-j;
                                                        printf("!x=%f,y=%f \n",CenterDockLX-dx,CenterDockLY-j);
                                                    }
                                                }
                                                else if (DockID == 2)
                                                {
                                                    result = ((CenterDockRX-dx-CenterLocX)*(CenterDockRX-dx-CenterLocX)/(B[i].Diameter*B[i].Diameter/4.0)+
                                                              (CenterDockRY-j-CenterLocY)*(CenterDockRY-j-CenterLocY)/(B[i].Diameter*B[i].Diameter/4.0));

                                                    if (result < 1.01)
                                                    {
                                                        IntX = CenterDockRX-dx;
                                                        IntY = CenterDockRY-j;
                                                        printf("!x=%f,y=%f \n",CenterDockRX-dx,CenterDockRY-j);
                                                    }
                                                }
                                            }
                                            break;
                                            case 3:
                                            {
                                                if (DockID == 1)
                                                {
                                                    result = ((CenterDockLX-dx-CenterLocX)*(CenterDockLX-dx-CenterLocX)/(B[i].Diameter*B[i].Diameter/4.0)+
                                                              (CenterDockLY+j-CenterLocY)*(CenterDockLY+j-CenterLocY)/(B[i].Diameter*B[i].Diameter/4.0));

                                                    if (result < 1.01)
                                                    {
                                                        IntX = CenterDockLX-dx;
                                                        IntY = CenterDockLY+j;
                                                        printf("!x=%f,y=%f \n",CenterDockLX-dx,CenterDockLY+j);
                                                    }
                                                }
                                                else if (DockID == 2)
                                                {
                                                    result = ((CenterDockRX-dx-CenterLocX)*(CenterDockRX-dx-CenterLocX)/(B[i].Diameter*B[i].Diameter/4.0)+
                                                              (CenterDockRY+j-CenterLocY)*(CenterDockRY+j-CenterLocY)/(B[i].Diameter*B[i].Diameter/4.0));

                                                    if (result < 1.01)
                                                    {
                                                        IntX = CenterDockRX-dx;
                                                        IntY = CenterDockRY+j;
                                                        printf("!x=%f,y=%f \n",CenterDockRX-dx,CenterDockRY+j);
                                                    }
                                                }
                                            }
                                            break;
                                            case 4:
                                            {
                                                // Left dock
                                                if (DockID == 1)
                                                {
                                                     result = ((CenterDockLX+dx-CenterLocX)*(CenterDockLX+dx-CenterLocX)/(B[i].Diameter*B[i].Diameter/4.0)+
                                                              (CenterDockLY+j-CenterLocY)*(CenterDockLY+j-CenterLocY)/(B[i].Diameter*B[i].Diameter/4.0));

                                                    if (result < 1.01)
                                                    {
                                                        IntX = CenterDockLX+dx;
                                                        IntY = CenterDockLY+j;
                                                        printf("!x=%f,y=%f \n",CenterDockLX+dx,CenterDockLY+j);
                                                    }
                                                }
                                                //Right dock
                                                else if (DockID == 2)
                                                {
                                                    result = ((CenterDockRX+dx-CenterLocX)*(CenterDockRX+dx-CenterLocX)/(B[i].Diameter*B[i].Diameter/4.0)+
                                                              (CenterDockRY+j-CenterLocY)*(CenterDockRY+j-CenterLocY)/(B[i].Diameter*B[i].Diameter/4.0));

                                                    if (result < 1.01)
                                                    {
                                                        IntX = CenterDockRX+dx;
                                                        IntY = CenterDockRY+j;
                                                        printf("!x=%f,y=%f \n",CenterDockRX+dx,CenterDockRY+j);
                                                    }
                                                }
                                            }
                                            break;
                                        }
                                    }
                                }

                                // Computing angle of the tangent line and normal vector
                                if (IntX>0 && IntY>0)
                                {
                                    float TnLineAngle;  // Angle of the tangent line in degrees
                                    float NormalVAngle; // Angle of the normal vector

                                    if (DockID == 1)
                                    {
                                        // Prevents the computing of the tangent to a line
                                        if (IntX==35.0 || IntX==5.0)
                                        {
                                            TnLineAngle = 90.0;
                                            if (IntX == 35.0)
                                            {
                                                NormalVAngle = 0;
                                            }
                                            else
                                            {
                                                NormalVAngle = 180.0;
                                            }
                                        }
                                        else if (IntX == 20.0)
                                        {
                                            TnLineAngle = 360.0;
                                            if (IntY < CenterDockLY)
                                            {
                                                NormalVAngle = 90.0;
                                            }
                                            else
                                            {
                                                NormalVAngle = 270.0;
                                            }
                                        }
                                        // Computing equation of the tangent line to a curve
                                        else
                                        {
                                           // Computing equation of the tangent line. Finding the angle of the tangent line that will range between 270-0-90 degrees
                                            double slope = (-6400.0/225.0)*(IntX-CenterDockLX)/(IntY-CenterDockLY);
                                            double coord1 = slope*(IntX-(IntX))+(IntY);            // f(IntX) coordinate on the tangent line
                                            double coord2 = slope*(CenterDockLX-(IntX))+(IntY);    // f(CenterDockLX) coordinate on the tangent line
                                            // Third and first quadrant
                                            if (((CenterDockLX-IntX)>0 && (IntY-CenterDockLY)>0) || ((IntX-CenterDockLX)>0 && (CenterDockLY-IntY)>0))
                                            {
                                                TnLineAngle = 360.0 - atanf(fabs(coord1-coord2)/fabs(IntX-CenterDockLX))*180.0/3.14;

                                               if ((CenterDockLX-IntX)>0)
                                                {
                                                    NormalVAngle = TnLineAngle-90.0;
                                                }
                                                else if ((IntX-CenterDockLX)>0)
                                                {
                                                    NormalVAngle = 90.0 - (360.0-TnLineAngle);
                                                }
                                            }
                                            // Second and forth quadrant
                                            else if (((CenterDockLX-IntX)>0 && (CenterDockLY-IntY)>0) || ((IntX-CenterDockLX)>0 && (IntY-CenterDockLY)>0))
                                            {
                                                TnLineAngle = atanf(fabs(coord1-coord2)/fabs(IntX-CenterDockLX))*180.0/3.14;
                                                if ((CenterDockLX-IntX)>0)
                                                {
                                                    NormalVAngle = 90.0 + TnLineAngle;
                                                }
                                                else if ((IntX-CenterDockLX)>0)
                                                {
                                                    NormalVAngle = 270.0 + TnLineAngle;
                                                }
                                            }
                                        }
                                    }
                                    else if (DockID == 2)
                                    {
                                        // Prevents the computing of the tangent to a line
                                        if (IntX==973.0 || IntX==1003.0)
                                        {
                                            TnLineAngle = 90.0;
                                            if (IntX == 973.0)
                                            {
                                                NormalVAngle = 180.0;
                                            }
                                            else
                                            {
                                                NormalVAngle = 0;
                                            }
                                        }
                                        else if (IntX == 988.0)
                                        {
                                            TnLineAngle = 360.0;
                                            if (IntY < CenterDockRY)
                                            {
                                                NormalVAngle = 90.0;
                                            }
                                            else
                                            {
                                                NormalVAngle = 270.0;
                                            }
                                        }
                                        // Computing equation of the tangent line to a curve
                                        else
                                        {
                                           // Computing equation of the tangent line. Finding the angle of the tangent line that will range between 270-0-90 degrees
                                            double slope = (-6400.0/225.0)*(IntX-CenterDockRX)/(IntY-CenterDockRY);
                                            double coord1 = slope*(IntX-(IntX))+(IntY);            // f(IntX) coordinate on the tangent line
                                            double coord2 = slope*(CenterDockRX-(IntX))+(IntY);    // f(CenterDockLX) coordinate on the tangent line
                                            // First and third quadrant
                                            if (((CenterDockRX-IntX)>0 && (IntY-CenterDockRY)>0) || ((IntX-CenterDockRX)>0 && (CenterDockRY-IntY)>0))
                                            {
                                                TnLineAngle = 360.0 - atanf(fabs(coord1-coord2)/fabs(IntX-CenterDockRX))*180.0/3.14;
                                                if ((CenterDockRX-IntX)>0)
                                                {
                                                    NormalVAngle = TnLineAngle-90.0;
                                                }
                                                else if ((IntX-CenterDockRX)>0)
                                                {
                                                    NormalVAngle = 90.0 - (360.0-TnLineAngle);
                                                }
                                            }
                                            // Second and forth quadrant
                                            else if (((CenterDockRX-IntX)>0 && (CenterDockRY-IntY)>0) || ((IntX-CenterDockRX)>0 && (IntY-CenterDockRY)>0))
                                            {
                                                TnLineAngle = atanf(fabs(coord1-coord2)/fabs(IntX-CenterDockRX))*180.0/3.14;
                                                if ((CenterDockRX-IntX)>0)
                                                {
                                                    if (TnLineAngle == 360.0) {TnLineAngle = 0;}
                                                    NormalVAngle = 90.0+TnLineAngle;
                                                }
                                                else if ((IntX-CenterDockRX)>0)
                                                {
                                                    if (TnLineAngle == 360.0) {TnLineAngle = 0;}
                                                    NormalVAngle = 270.0+TnLineAngle;
                                                }
                                            }
                                        }
                                    }
                                    printf("TnLineAngle=%f \n",TnLineAngle);
                                    printf("NormalVAngle=%f \n",NormalVAngle);

                                    // Calculating the ball's new direction angle
                                    printf ("Old direction = %f \n", B[i].Direction);
                                    float InvNA = NormalVAngle-180.0;   // Normal vector in the opposite direction
                                    float tmp;  // Temporary buffer for difference between angles
                                    if (InvNA <= 0)
                                    {
                                        InvNA = 360.0+InvNA;
                                    }
                                    tmp = (max(B[i].Direction, InvNA)-min(B[i].Direction, InvNA));
                                    // When we subtract a first quadrant angle from the forth quadrant angle
                                    if (tmp > 180.0)
                                    {
                                        tmp = 360.0-tmp;

                                        if (B[i].Direction > InvNA)
                                        {
                                            B[i].Direction = NormalVAngle+tmp;
                                        }
                                        else if (B[i].Direction < InvNA)
                                        {
                                            B[i].Direction = NormalVAngle-tmp;
                                        }
                                    }
                                    // Other cases
                                    else
                                    {
                                        if (B[i].Direction > InvNA)
                                        {
                                            B[i].Direction = NormalVAngle - tmp;
                                        }
                                        else if (B[i].Direction <= InvNA)
                                        {
                                            B[i].Direction = NormalVAngle + tmp;
                                        }
                                    }
                                    // Correcting the angle to an appropriate format
                                    if (B[i].Direction < 0)
                                    {
                                        B[i].Direction = 360.0+B[i].Direction;
                                    }
                                    if (B[i].Direction > 360.0)
                                    {
                                        B[i].Direction = B[i].Direction - 360.0;
                                    }

                                    // Correcting direction of the impulse vector and coordinates of the intersection point
                                    if (B[i].Direction>=0 && B[i].Direction<90.0)
                                    {
                                        B[i].Impulse.y = fabs(B[i].Impulse.y)*(-1.0);
                                        B[i].Impulse.x = fabs(B[i].Impulse.x);

                                        // Correcting coordinates of the intersection point
                                        B[i].Coord.top = B[i].Coord.top-(B[i].SpeedM*1.5)-1.0;
                                        B[i].Coord.bottom = (B[i].Coord.top+B[i].Diameter);
                                        B[i].Coord.left = B[i].Coord.left+(B[i].SpeedM*1.5)+1.0;
                                        B[i].Coord.right = (B[i].Coord.left+B[i].Diameter);
                                    }
                                    else if (B[i].Direction>=90.0 && B[i].Direction<180.0)
                                    {
                                        B[i].Impulse.y = fabs(B[i].Impulse.y)*(-1.0);
                                        B[i].Impulse.x = fabs(B[i].Impulse.x)*(-1.0);

                                        // Correcting coordinates of the intersection point
                                        B[i].Coord.top = B[i].Coord.top-(B[i].SpeedM*1.5)-1.0;
                                        B[i].Coord.bottom = (B[i].Coord.top+B[i].Diameter);
                                        B[i].Coord.left = B[i].Coord.left-(B[i].SpeedM*1.5)-1.0;
                                        B[i].Coord.right = (B[i].Coord.left+B[i].Diameter);
                                    }
                                    else if (B[i].Direction>=180.0 && B[i].Direction<270.0)
                                    {
                                        B[i].Impulse.y = fabs(B[i].Impulse.y);
                                        B[i].Impulse.x = fabs(B[i].Impulse.x)*(-1.0);

                                        // Correcting coordinates of the intersection point
                                        B[i].Coord.top = B[i].Coord.top+(B[i].SpeedM*1.5)+1.0;
                                        B[i].Coord.bottom = (B[i].Coord.top+B[i].Diameter);
                                        B[i].Coord.left = B[i].Coord.left-(B[i].SpeedM*1.5)-1.0;
                                        B[i].Coord.right = (B[i].Coord.left+B[i].Diameter);
                                    }
                                    else if (B[i].Direction>=270.0 && B[i].Direction<360.0)
                                    {
                                        B[i].Impulse.y = fabs(B[i].Impulse.y);
                                        B[i].Impulse.x = fabs(B[i].Impulse.x);

                                        // Correcting coordinates of the intersection point
                                        B[i].Coord.top = B[i].Coord.top+(B[i].SpeedM*1.5)+1.0;
                                        B[i].Coord.bottom = (B[i].Coord.top+B[i].Diameter);
                                        B[i].Coord.left = B[i].Coord.left+(B[i].SpeedM*1.5)+1.0;
                                        B[i].Coord.right = (B[i].Coord.left+B[i].Diameter);
                                    }

                                    printf ("New direction = %f \n", B[i].Direction);
                                }
                            }

                            // Computing dynamics between the local ball and the remote ball. Remote ball will be with the index 'j'
                            for (int j=0; j<NrBalls; j++)
                            {
                                // Computing coordinates for the center of the remote ball
                                double CenterRemX = B[j].Coord.left+(B[j].Diameter)/2.0;
                                double CenterRemY = B[j].Coord.top+(B[j].Diameter)/2.0;

                                if (j!=i)
                                {
                                    // Case when balls are in the interaction range. Determining by comparing radius with distance
                                    if (sqrt((abs(CenterLocX-CenterRemX)*abs(CenterLocX-CenterRemX))+
                                             (abs(CenterLocY-CenterRemY)*abs(CenterLocY-CenterRemY))) <= ((B[i].Diameter+B[j].Diameter)/2.0))
                                    {
                                        // Coloring balls when they contact
                                        B[i].Color.Red = rand() % 255;
                                        B[i].Color.Green = rand() % 255;
                                        B[i].Color.Blue = rand() % 255;

                                        // Computing the coordinates of the intersection point, and the angle of the tangent line at the intersection point
                                        float angle, TanLnAngle, NormalAngle;
                                        double dx;
                                        double dy;
                                        angle = asinf(fabs(CenterRemY-CenterLocY)/sqrtf((CenterRemX-CenterLocX)*(CenterRemX-CenterLocX)+(CenterRemY-CenterLocY)*(CenterRemY-CenterLocY)));
                                        // Calculating error in computing the intersection point
                                        double offsetXY, offsetX, offsetY;
                                        offsetXY = (B[i].Diameter+B[j].Diameter)/2.0 - sqrtf((CenterRemX-CenterLocX)*(CenterRemX-CenterLocX)+(CenterRemY-CenterLocY)*(CenterRemY-CenterLocY));
                                        printf("offsetXY = %f \n", offsetXY);

                                        offsetX = offsetXY*cos(angle)+1.4;
                                        offsetY = offsetXY*sin(angle)+1.4;
                                        printf("offsetX = %f \n", offsetX);
                                        printf("offsetY = %f \n", offsetY);

                                        dx = (B[i].Diameter/2.0)*cos(angle);    // Offset from the intersection point along the x-axys
                                        dy = (B[i].Diameter/2.0)*sin(angle);    // Offset from the intersection point along the y-axys
                                        if (CenterLocX<CenterRemX && CenterLocY<CenterRemY)
                                        {
                                            // Correcting coordinates for the local ball. Averaging offset according to ball radius
                                            CenterLocX = CenterLocX - offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            CenterLocY = CenterLocY - offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.left = B[i].Coord.left - offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.right = B[i].Coord.right - offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.top = B[i].Coord.top - offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.bottom = B[i].Coord.bottom - offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            // Correcting coordinates for the remote ball. Averaging offset according to ball radius
                                            CenterRemX = CenterRemX + offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            CenterRemY = CenterRemY + offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.left = B[j].Coord.left + offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.right = B[j].Coord.right + offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.top = B[j].Coord.top + offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.bottom = B[j].Coord.bottom + offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);

                                            //InterX = CenterLocX+(dx);   // Intersection coordinates along the x-axys
                                            //InterY = CenterLocY+(dy);   // Intersection coordinates along the y-axys
                                            TanLnAngle = 90.0 - angle*180.0/3.14;  // Angle of the tangent line at the intersection point
                                            NormalAngle = TanLnAngle + 90.0;    // Angle of the normal vector
                                            if (NormalAngle > 360.0) {NormalAngle -= 360.0;}
                                        }
                                        else if (CenterLocX<CenterRemX && CenterLocY>CenterRemY)
                                        {
                                            // Correcting coordinates for the local ball. Averaging offset according to ball radius
                                            CenterLocX = CenterLocX - offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            CenterLocY = CenterLocY + offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.left = B[i].Coord.left - offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.right = B[i].Coord.right - offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.top = B[i].Coord.top + offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.bottom = B[i].Coord.bottom + offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            // Correcting coordinates for the remote ball. Averaging offset according to ball radius
                                            CenterRemX = CenterRemX + offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            CenterRemY = CenterRemY - offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.left = B[j].Coord.left + offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.right = B[j].Coord.right + offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.top = B[j].Coord.top - offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.bottom = B[j].Coord.bottom - offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);

                                            //InterX = CenterLocX+(dx);
                                            //InterY = CenterLocY-(dy);
                                            TanLnAngle = 270.0 + angle*180.0/3.14;
                                            NormalAngle = TanLnAngle + 90.0;
                                            if (NormalAngle > 360.0) {NormalAngle -= 360.0;}
                                        }
                                        else if (CenterLocX>CenterRemX && CenterLocY<CenterRemY)
                                        {
                                            // Correcting coordinates for the local ball. Averaging offset according to ball radius
                                            CenterLocX = CenterLocX + offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            CenterLocY = CenterLocY - offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.left = B[i].Coord.left + offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.right = B[i].Coord.right + offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.top = B[i].Coord.top - offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.bottom = B[i].Coord.bottom - offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            // Correcting coordinates for the remote ball. Averaging offset according to ball radius
                                            CenterRemX = CenterRemX - offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            CenterRemY = CenterRemY + offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.left = B[j].Coord.left - offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.right = B[j].Coord.right - offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.top = B[j].Coord.top + offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.bottom = B[j].Coord.bottom + offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);

                                            //InterX = CenterLocX-(dx);
                                            //InterY = CenterLocY+(dy);
                                            TanLnAngle = 270.0 + angle*180.0/3.14;
                                            NormalAngle = TanLnAngle + 90.0;
                                            if (NormalAngle > 360.0) {NormalAngle -= 360.0;}
                                        }
                                        else if (CenterLocX>CenterRemX && CenterLocY>CenterRemY)
                                        {
                                            // Correcting coordinates for the local ball. Averaging offset according to ball radius
                                            CenterLocX = CenterLocX + offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            CenterLocY = CenterLocY + offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.left = B[i].Coord.left + offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.right = B[i].Coord.right + offsetX*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.top = B[i].Coord.top + offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[i].Coord.bottom = B[i].Coord.bottom + offsetY*(B[i].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            // Correcting coordinates for the remote ball. Averaging offset according to ball radius
                                            CenterRemX = CenterRemX - offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            CenterRemY = CenterRemY - offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.left = B[j].Coord.left - offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.right = B[j].Coord.right - offsetX*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.top = B[j].Coord.top - offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);
                                            B[j].Coord.bottom = B[j].Coord.bottom - offsetY*(B[j].Diameter/2.0)/((B[i].Diameter+B[j].Diameter)/2.0);

                                            //InterX = CenterLocX-(dx);
                                            //InterY = CenterLocY-(dy);
                                            TanLnAngle = 90.0 - angle*180.0/3.14;
                                            NormalAngle = TanLnAngle + 90.0;
                                            if (NormalAngle > 360.0) {NormalAngle -= 360.0;}
                                        }
                                        printf("Tangent line/Normal angle is: %f/%f \n", TanLnAngle,NormalAngle);

                                        // Calculating the impulse percentage that will be transmitted on contact
                                        float* ImpulsePerc = new float[2]; //{index 0 - Local ball, index 1 - Remote ball}
                                        // Local ball
                                        if (B[i].Direction>=0 && B[i].Direction<=180.0)
                                        {
                                            ImpulsePerc[0] = fabs(100.0 - fabs(B[i].Direction-NormalAngle)*1.11);
                                        }
                                        else if (B[i].Direction>=180.0 && B[i].Direction<=360.0)
                                        {
                                            ImpulsePerc[0] = fabs(100.0 - fabs((B[i].Direction-180.0)-NormalAngle)*1.11);
                                        }
                                        // Remote ball
                                        if (B[j].Direction>=0 && B[j].Direction<=180.0)
                                        {
                                            ImpulsePerc[1] = fabs(100.0 - fabs(B[j].Direction-NormalAngle)*1.11);
                                        }
                                        else if (B[j].Direction>=180.0 && B[j].Direction<=360.0)
                                        {
                                            ImpulsePerc[1] = fabs(100.0 - fabs((B[j].Direction-180.0)-NormalAngle)*1.11);
                                        }
                                        printf("Percentage: local(%f)/remote(%f)\n", ImpulsePerc[0], ImpulsePerc[1]);
                                        printf ("local ball is: %d, remote ball is: %d \n", i,j);

                                        // Distributing impulse. Case for intersecting directions along the x-axys
                                        float* tmp = new float;
                                        *tmp = B[j].Impulse.x;
                                        B[j].Impulse.x = B[j].Impulse.x + B[i].Impulse.x*ImpulsePerc[0]/100.0;
                                        B[i].Impulse.x = B[i].Impulse.x*(100.0-ImpulsePerc[0])/100.0+(*tmp)*ImpulsePerc[1]/100.0;
                                        B[j].Impulse.x = B[j].Impulse.x-(*tmp)*(ImpulsePerc[1])/100.0;
                                        *tmp = B[j].Impulse.y;
                                        B[j].Impulse.y = B[j].Impulse.y + B[i].Impulse.y*ImpulsePerc[0]/100.0;
                                        B[i].Impulse.y = B[i].Impulse.y*(100.0-ImpulsePerc[0])/100.0+(*tmp)*ImpulsePerc[1]/100.0;
                                        B[j].Impulse.y = B[j].Impulse.y-(*tmp)*(ImpulsePerc[1])/100.0;
                                        printf("Ball %d impulse is (%f,%f)\n",i,B[i].Impulse.x,B[i].Impulse.y);
                                        printf("Ball %d impulse is (%f,%f)\n",j,B[j].Impulse.x,B[j].Impulse.y);

                                        // Computing new angle for the first ball
                                        *tmp = asinf(fabs(B[j].Impulse.y)/sqrtf((B[j].Impulse.x*B[j].Impulse.x)+(B[j].Impulse.y*B[j].Impulse.y)))*180.0/3.14;
                                        if (B[j].Impulse.x>=0 && B[j].Impulse.y>=0)
                                        {
                                            B[j].Direction = 360.0 - (*tmp);
                                        }
                                        else if (B[j].Impulse.x>=0 && B[j].Impulse.y<=0)
                                        {
                                            B[j].Direction = (*tmp);
                                        }
                                        else if (B[j].Impulse.x<=0 && B[j].Impulse.y<=0)
                                        {
                                            B[j].Direction = 180.0 - (*tmp);
                                        }
                                        else if (B[j].Impulse.x<=0 && B[j].Impulse.y>=0)
                                        {
                                            B[j].Direction = 180.0 + (*tmp);
                                        }
                                        // Computing new angle for the second ball.
                                        *tmp = asinf(fabs(B[i].Impulse.y)/sqrtf((B[i].Impulse.x*B[i].Impulse.x)+(B[i].Impulse.y*B[i].Impulse.y)))*180.0/3.14;
                                        if (B[i].Impulse.x>=0 && B[i].Impulse.y>=0)
                                        {
                                            B[i].Direction = 360.0 - (*tmp);
                                        }
                                        else if (B[i].Impulse.x>=0 && B[i].Impulse.y<=0)
                                        {
                                            B[i].Direction = (*tmp);
                                        }
                                        else if (B[i].Impulse.x<=0 && B[i].Impulse.y<=0)
                                        {
                                            B[i].Direction = 180.0 - (*tmp);
                                        }
                                        else if (B[i].Impulse.x<=0 && B[i].Impulse.y>=0)
                                        {
                                            B[i].Direction = 180.0 + (*tmp);
                                        }
                                        printf ("angle is: %f\n", B[j].Direction);
                                        printf ("angle is: %f\n", B[i].Direction);

                                        delete tmp; delete [] ImpulsePerc;
                                        B[i].UpdateSpeed(); B[j].UpdateSpeed();
                                    }
                                }
                            }

                            // Updating the coordinates
                            B[i].Coord.left = B[i].Coord.left+x*B[i].SpeedM;
                            B[i].Coord.right = B[i].Coord.right+x*B[i].SpeedM;
                            B[i].Coord.top = B[i].Coord.top-y*B[i].SpeedM;
                            B[i].Coord.bottom = B[i].Coord.bottom-y*B[i].SpeedM;
                        }

                        // "Computer" dock control. Self-guided
                        if ((D1.Coord.left-B[RightMost].Coord.right)>0 && ((B[RightMost].Direction<90.0)||(B[RightMost].Direction>270.0)))
                        {
                            double* DockStep = new double;
                            double* BallStep = new double;

                            if (B[RightMost].Direction > 270.0)
                            {
                                *BallStep = cos((360.0-B[RightMost].Direction)*3.14/180.0)*B[RightMost].SpeedM;

                            }
                            else
                            {
                                *BallStep = cos(B[RightMost].Direction*3.14/180.0)*B[RightMost].SpeedM;
                            }

                            *DockStep = (((B[RightMost].Coord.top+B[RightMost].Coord.bottom)/2.0)-((D1.Coord.top+D1.Coord.bottom)/2.0))/
                                        (((D1.Coord.left+D1.Coord.right)/2.0)-((B[RightMost].Coord.left+B[RightMost].Coord.right)/2.0))*(*BallStep+1.0);

                            // Updating dock coordinates
                            if ((D1.Coord.bottom+(*DockStep))<720.0 && (D1.Coord.top+(*DockStep))>40.0)
                            {
                                D1.Coord.top = D1.Coord.top+(*DockStep);
                                D1.Coord.bottom = D1.Coord.bottom+(*DockStep);
                            }

                            delete BallStep;
                            delete DockStep;
                        }

                        InvalidateRect(hwnd, &WorkingAreaRect, FALSE);
                    }
            }
        }
        break;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_DOWN:
                {
                    if ((D0.Coord.bottom+10.0) < 720.0)
                    {
                        D0.Coord.bottom = D0.Coord.bottom+1.0;
                        D0.Coord.top = D0.Coord.top+1.0;
                    }
                }
                break;
                case VK_UP:
                {
                    if ((D0.Coord.top-10.0) > 40.0)
                    {
                        D0.Coord.bottom = D0.Coord.bottom-1.0;
                        D0.Coord.top = D0.Coord.top-1.0;
                    }
                }
                break;
                case VK_SPACE:
                {
                    for (int i=0; i<NrBalls; i++)
                    {
                        // If center of the ball is between Dock0 "top" endpoint and Dock0 "bottom" endpoint
                        if ((B[i].Coord.top+B[i].Diameter/2.0)>D0.Coord.top && (B[i].Coord.top+B[i].Diameter/2.0)<D0.Coord.bottom)
                        {
                            // Limiting the range to 100px. Updating impulse. Computing the new direction
                            if (B[i].Coord.left-D0.Coord.right <= 100.0)
                            {
                                B[i].Impulse.x = B[i].Impulse.x+(100.0-(B[i].Coord.left-D0.Coord.right));
                                // Computing the new direction
                                float tmp = asinf(fabs(B[i].Impulse.y)/sqrtf((B[i].Impulse.x*B[i].Impulse.x)+(B[i].Impulse.y*B[i].Impulse.y)))*180.0/3.14;
                                if (B[i].Impulse.x>=0 && B[i].Impulse.y>=0)
                                {
                                    B[i].Direction = 360.0 - (tmp);
                                }
                                else if (B[i].Impulse.x>=0 && B[i].Impulse.y<=0)
                                {
                                    B[i].Direction = (tmp);
                                }
                                else if (B[i].Impulse.x<=0 && B[i].Impulse.y<=0)
                                {
                                    B[i].Direction = 180.0 - (tmp);
                                }
                                else if (B[i].Impulse.x<=0 && B[i].Impulse.y>=0)
                                {
                                    B[i].Direction = 180.0 + (tmp);
                                }
                                B[i].UpdateSpeed();
                                printf ("Space \n");
                            }

                        }
                    }
                }
                break;
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
            float xPos = LOWORD(lParam);    // Horizontal position of cursor
            float yPos = HIWORD(lParam);    // Vertical position of cursor

            // Checking if cursor position isn't beyond the working area bounds
            if (xPos < WorkingAreaRect.left || xPos > WorkingAreaRect.right || yPos < WorkingAreaRect.top ||
                yPos > WorkingAreaRect.bottom) {break;}

            // Reloading objects array to prevent overflow
            if (NrBalls == 20)
            {
                delete [] B;
                Ball* B = new Ball[20];
                NrBalls = 0;
            }

            // Stores the coordinates of the first and second point
            if (ClicksCounter == 0)
            {
                B[NrBalls].Diameter = 30.0;
                B[NrBalls].Coord.left = xPos;
                B[NrBalls].Coord.top = yPos;
                ClicksCounter++;
            }
            else if (ClicksCounter == 1)
            {
                // Forcing the figure to be a circle
                B[NrBalls].Coord.right = B[NrBalls].Coord.left+B[NrBalls].Diameter;
                B[NrBalls].Coord.bottom = B[NrBalls].Coord.top+B[NrBalls].Diameter;

                // Prevent drawing beyond the 'WorkingAreaRect' rectangle
                if (B[NrBalls].Coord.right >= WorkingAreaRect.right)
                {
                    B[NrBalls].Coord.right = WorkingAreaRect.right-1;
                    B[NrBalls].Coord.bottom = B[NrBalls].Coord.top+(B[NrBalls].Coord.right-B[NrBalls].Coord.left);
                    B[NrBalls].Diameter = B[NrBalls].Coord.right-B[NrBalls].Coord.left;
                }
                else if (B[NrBalls].Coord.bottom >= WorkingAreaRect.bottom)
                {
                    B[NrBalls].Coord.bottom = WorkingAreaRect.bottom-1;
                    B[NrBalls].Coord.right = B[NrBalls].Coord.left+(B[NrBalls].Coord.bottom-B[NrBalls].Coord.top);
                    B[NrBalls].Diameter = B[NrBalls].Coord.bottom-B[NrBalls].Coord.top;
                }

                // Setting a random direction (angle)
                B[NrBalls].SetDirection((rand() % 358)+2);
                printf("Impulse for ball %d is: (%f,%f) \n", NrBalls, B[NrBalls].Impulse.x, B[NrBalls].Impulse.y);
                ClicksCounter++;
                InvalidateRect(hwnd, &WorkingAreaRect, FALSE);
            }
        }
        break;

        // Mouse move event
        case WM_MOUSEMOVE:
        {
            // General declarations
            float xPos = LOWORD(lParam);    // Horizontal position of cursor
            float yPos = HIWORD(lParam);    // Vertical position of cursor

            // Current position of the cursor. Will be used in 'WM_PAINT' for real-time view of the figure during it's drawing
            if (ClicksCounter == 1)
            {
                // Calculating and storing diameter of the Ball and the endpoint
                if (max(xPos-B[NrBalls].Coord.left, yPos-B[NrBalls].Coord.top)>30.0
                    && max(xPos-B[NrBalls].Coord.left, yPos-B[NrBalls].Coord.top)<200.0)
                {
                    // Storing the diameter of the Ball
                    B[NrBalls].Diameter = max(xPos-B[NrBalls].Coord.left, yPos-B[NrBalls].Coord.top);
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
            COLORREF qColor = RGB(255, 255, 255);   // White
            HPEN WhitePen = CreatePen(PS_SOLID, 2, qColor);
            HBRUSH WhiteBrush = CreateSolidBrush(qColor);

            // Creating the working area
            SelectObject(hdcMem, WndFramePen);
            MoveToEx(hdcMem, WorkingAreaRect.left, WorkingAreaRect.top, NULL);
            LineTo(hdcMem, WorkingAreaRect.right, WorkingAreaRect.top);
            MoveToEx(hdcMem, WorkingAreaRect.left, WorkingAreaRect.bottom, NULL);
            LineTo(hdcMem, WorkingAreaRect.right, WorkingAreaRect.bottom);
            FillRect(hdcMem, &WorkingAreaRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

            // Creating docks
            HBRUSH hBrush;
            SelectObject(hdcMem, WhitePen);
            // Creating the user's dock
            qColor = RGB(D0.Color.Red, D0.Color.Green, D0.Color.Blue);
            hBrush = CreateSolidBrush(qColor);
            SelectObject(hdcMem, hBrush);
            Ellipse(hdcMem, D0.Coord.left, D0.Coord.top, D0.Coord.right, D0.Coord.bottom);  // User's dock
            DeleteObject(hBrush);
            // Creating the computer's dock
            qColor = RGB(D1.Color.Red, D1.Color.Green, D1.Color.Blue);
            hBrush = CreateSolidBrush(qColor);
            SelectObject(hdcMem, hBrush);
            Ellipse(hdcMem, D1.Coord.left, D1.Coord.top, D1.Coord.right, D1.Coord.bottom);  // Computer's dock
            DeleteObject(hBrush);

            // Printing player score data
            {
                // Declarations
                char** strdata = new char*[2];
                for (int i=0; i<2; i++) {strdata[i] = new char[3];}
                int* NrDigits = new int[2];
                // Cleaning the score board rectangle
                FillRect(hdcMem, &ScoreboardRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
                // Converting 'int' to 'string'
                NrDigits[0] = wsprintf(strdata[0], "%d", PlayerScore[0]);
                NrDigits[1] = wsprintf(strdata[1], "%d", PlayerScore[1]);
                // Selecting font
                HFONT font = CreateFont(-18, 0, 0, 0, 0, ANSI_CHARSET, 0U, 0U, 0U, 0U, 0U, 0U, 0U, TEXT("Times New Roman"));
                SelectObject(hdcMem, font);
                SetBkMode(hdcMem, TRANSPARENT);
                // Printing
                SetTextColor(hdcMem, 0x00FF0000);  // Font color (blue)
                DrawText(hdcMem, "Player:", lstrlen("Player:"), &ScoreRectText[0], DT_LEFT);
                SetTextColor(hdcMem, 0x000000FF);  // Font color (Red)
                DrawText(hdcMem, "Computer:", lstrlen("Computer:"), &ScoreRectText[3], DT_LEFT);
                SetTextColor(hdcMem, 0x00FFFFFF);  // Font color (White)
                DrawText(hdcMem, strdata[0], NrDigits[0], &ScoreRectText[1], DT_LEFT);
                DrawText(hdcMem, strdata[1], NrDigits[1], &ScoreRectText[2], DT_LEFT);
                // Cleaning
                DeleteObject(font);
                delete [] strdata[0]; delete [] strdata[1]; delete [] strdata;
                delete [] NrDigits;
            }

            // Creating the startup ball
            if (!NrBalls)
            {
                // Setting the ball diameter
                B[NrBalls].Diameter = 60.0;
                // Defining the initial direction
                B[NrBalls].SetDirection((rand() % 358)+2);
                printf("Impulse for ball %d is: (%f,%f) \n", NrBalls, B[NrBalls].Impulse.x, B[NrBalls].Impulse.y);
                // Defining the initial position
                B[NrBalls].Coord.left = ((WorkingAreaRect.right-WorkingAreaRect.left)/2.0)-30.0;
                B[NrBalls].Coord.right = ((WorkingAreaRect.right-WorkingAreaRect.left)/2.0)+30.0;
                B[NrBalls].Coord.top = ((WorkingAreaRect.bottom-WorkingAreaRect.top)/2.0)-30.0;
                B[NrBalls].Coord.bottom = ((WorkingAreaRect.bottom-WorkingAreaRect.top)/2.0)+30.0;
                // Registering the new ball
                NrBalls++;
            }

            // Block of code responsible for real-time view of the figure during its drawing
            if (ClicksCounter == 1)
            {
                SelectObject(hdcMem, WhiteBrush);
                Ellipse(hdcMem, B[NrBalls].Coord.left, B[NrBalls].Coord.top, B[NrBalls].Coord.left+B[NrBalls].Diameter, B[NrBalls].Coord.top+B[NrBalls].Diameter);
            }
            else if (ClicksCounter == 2)
            {
                NrBalls++;
                ClicksCounter = 0;
            }

            // With this block of code, the recorded figures will be redrawn each time window refresh occurs
            for (int i=0; i<NrBalls; i++)
            {
                // Selecting the appropriate pen & brush to paint the indexed ball
                SelectObject(hdcMem, WhitePen);
                qColor = RGB(B[i].Color.Red, B[i].Color.Green, B[i].Color.Blue);
                hBrush = CreateSolidBrush(qColor);
                SelectObject(hdcMem, hBrush);
                // Drawing a circle filled with white with 2pt width margins
                Ellipse(hdcMem, B[i].Coord.left, B[i].Coord.top, B[i].Coord.right, B[i].Coord.bottom);
                DeleteObject(hBrush);

                // Cleaning
                SelectObject(hdcMem, WhiteBrush);
                DeleteObject(WhiteBrush);
                SelectObject(hdcMem, WndFramePen);
                DeleteObject(WhitePen);
            }

            // Showing the bitmap on the screen, ending paint
            BitBlt(hdc, 0, 0, 1024, 768, hdcMem, 0, 0, SRCCOPY);
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
    WorkingAreaRect.left = 0;
    WorkingAreaRect.top = 40;
    WorkingAreaRect.right = 1008;
    WorkingAreaRect.bottom = 720;
    // Setting the score board rectangle coordinates
    ScoreboardRect.left = 0;
    ScoreboardRect.top = 1;
    ScoreboardRect.right = 1008;
    ScoreboardRect.bottom = 30;

    // Setting the position coordinates where score data will be printed
    {
        // "Player" label
        ScoreRectText[0].left = 200;
        ScoreRectText[0].top = 5;
        ScoreRectText[0].right = 261;
        ScoreRectText[0].bottom = 25;
        // "Player" data
        ScoreRectText[1].left = 256;
        ScoreRectText[1].top = 5;
        ScoreRectText[1].right = 286;
        ScoreRectText[1].bottom = 25;
        // "Computer" label
        ScoreRectText[3].left = 692;
        ScoreRectText[3].top = 5;
        ScoreRectText[3].right = 779;
        ScoreRectText[3].bottom = 25;
        // "Computer" data
        ScoreRectText[2].left = 774;
        ScoreRectText[2].top = 5;
        ScoreRectText[2].right = 804;
        ScoreRectText[2].bottom = 25;
    }

    // Setting the initial score values
    PlayerScore[0] = 0;
    PlayerScore[1] = 0;

    // Creating a pen for the working area rectangle frame
    COLORREF qColor = RGB(255, 255, 255);   // White
    WndFramePen = CreatePen(PS_SOLID, 5, qColor);

    // Using current time for first number generation
    time_t t;
    srand((unsigned) time(&t));
}





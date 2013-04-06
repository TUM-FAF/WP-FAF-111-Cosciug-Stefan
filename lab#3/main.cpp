#include <windows.h>
#include <iostream>
#include <cstdio>
#include <string.h>





/*///////////////////////////////////////////
1.Function prototypes & global declaratons://
///////////////////////////////////////////*/


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Initialization(void);

#define WndWidth  640
#define WndHeight 460
#define IDC_CLEAR 20
#define IDC_MouseAbility 21
#define IDC_Rectangle 22
#define IDC_Ellipse 23
#define IDC_Bezier 24
HWND hwndMain;                                                      // Handle of the main window
HWND hwndButton[5];                                                 // Handles of the buttons from the main window
PAINTSTRUCT ps;
HDC hdc;
HPEN hPenOld;
RECT GeneralPurposeRect;
RECT WorkingAreaRect;
RECT WorkingAreaTitleRect;
RECT WorkingAreaInfoRect;
POINT CurrentPos;                                                   // Used to pass the current cursor coordinates from 'WM_MOUSEMOVE' to 'WM_PAINT'
POINT BezCurveBI[4];                                                // Keeps the coordinates required to draw a Bezier curve. Reserved for the built-in Bezier curve
POINT BezCurveCoord[20][4];                                         // Keeps the coordinates required to draw a Bezier curve
POINT RectCoord[20][2];                                             // Keeps the coordinates required to draw a rectangle
POINT EllipseCoord[20][2];                                          // Keeps the coordinates required to draw an ellipse
int NrRectangles;
int NrBezCurves;
int NrEllipses;
int DrawStartupObjects = 1;                                         // {1 - yes; 0 - no}
int MouseAbility = 0;                                               // {0 - Use mouse clicks to draw figures; 1 - use drag and drop method to draw figures}
int DrawFigure = 0;                                                 // {1 - Draw a rectangle; 2 - Draw an ellipse; 3 - Draw a bezier curve}
int ClicksCounter = 0;                                              // Based on the number of clicks, the program determines what figure needs to be drawn





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
        {
            hwndButton[0] = CreateWindow(
                                        "BUTTON",                                               // predefined class
                                        "Clear",                                                // button text
                                        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,               // styles
                                        450,                                                    // starting x position
                                        WndHeight - 80,                                         // starting y position
                                        160,                                                    // button width
                                        40,                                                     // button height
                                        hwnd,                                                   // parent window
                                        (HMENU)IDC_CLEAR,                                       // ID
                                        GetModuleHandle(NULL),
                                        NULL);

            hwndButton[1] = CreateWindow(
                                        "BUTTON",                                               // predefined class
                                        "Mouse: Drag and Drop",                                 // button text
                                        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,               // styles
                                        450,                                                    // starting x position
                                        20,                                                     // starting y position
                                        160,                                                    // button width
                                        40,                                                     // button height
                                        hwnd,                                                   // parent window
                                        (HMENU)IDC_MouseAbility,                                // ID
                                        GetModuleHandle(NULL),
                                        NULL);

            hwndButton[2] = CreateWindow(
                                        "BUTTON",                                               // predefined class
                                        "Rectangle",                                            // button text
                                        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,               // styles
                                        450,                                                    // starting x position
                                        100,                                                    // starting y position
                                        160,                                                    // button width
                                        40,                                                     // button height
                                        hwnd,                                                   // parent window
                                        (HMENU)IDC_Rectangle,                                   // ID
                                        GetModuleHandle(NULL),
                                        NULL);

            hwndButton[3] = CreateWindow(
                                        "BUTTON",                                               // predefined class
                                        "Ellipse",                                              // button text
                                        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,               // styles
                                        450,                                                    // starting x position
                                        160,                                                    // starting y position
                                        160,                                                    // button width
                                        40,                                                     // button height
                                        hwnd,                                                   // parent window
                                        (HMENU)IDC_Ellipse,                                     // ID
                                        GetModuleHandle(NULL),
                                        NULL);

            hwndButton[4] = CreateWindow(
                                        "BUTTON",                                               // predefined class
                                        "Bezier curve",                                         // button text
                                        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,               // styles
                                        450,                                                    // starting x position
                                        220,                                                    // starting y position
                                        160,                                                    // button width
                                        40,                                                     // button height
                                        hwnd,                                                   // parent window
                                        (HMENU)IDC_Bezier,                                      // ID
                                        GetModuleHandle(NULL),
                                        NULL);
        }
        break;

        // Setting the window min/max limits
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
            switch(LOWORD(wParam))
            {
                case IDC_CLEAR:
                {
                    // Reseting buffers and cleaning the working area; 'DrawStartupObjects' flag will prevent the built-in figures to be redrawn
                    DrawStartupObjects = 0;
                    ClicksCounter = 0;
                    DrawFigure = 0;
                    NrRectangles = 0;
                    NrEllipses = 0;
                    NrBezCurves = 0;
                    InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    InvalidateRect(hwnd, &WorkingAreaInfoRect, TRUE);
                }
                break;

                case IDC_MouseAbility:
                {
                    if (MouseAbility == 0)
                    {
                        // Changing the button caption to "Mouse: Drag and Drop"
                        SendMessage(hwndButton[1], WM_SETTEXT, 0, (LPARAM)"Mouse: Click");
                        MouseAbility = 1;

                        // Reseting buffers
                        ClicksCounter = 0;
                        DrawFigure = 0;
                        InvalidateRect(hwnd, &WorkingAreaInfoRect, TRUE);
                    }
                    else if (MouseAbility == 1)
                    {
                        // Changing the button caption to "Mouse: Click"
                        SendMessage(hwndButton[1], WM_SETTEXT, 0, (LPARAM)"Mouse: Drag and Drop");
                        MouseAbility = 0;

                        // Reseting buffers
                        ClicksCounter = 0;
                        DrawFigure = 0;
                        InvalidateRect(hwnd, &WorkingAreaInfoRect, TRUE);
                    }
                }
                break;

                case IDC_Rectangle:
                {
                    DrawFigure = 1;
                }
                break;

                case IDC_Ellipse:
                {
                    DrawFigure = 2;
                }
                break;

                case IDC_Bezier:
                {
                    DrawFigure = 3;
                }
                break;
            }
        }
        break;

        // Left mouse button press event
        case WM_LBUTTONDOWN:
        {
            // General declarations
            int xPos = LOWORD(lParam);  // Horizontal position of cursor
            int yPos = HIWORD(lParam);  // Vertical position of cursor

            // Checking if cursor position isn't beyond the working area bounds
            if (xPos < 0 || xPos > 420 || yPos < 20 || yPos > 400)
            {
                break;
            }

            // Stores the coordinates of the first point for each figure
            switch(DrawFigure)
            {
                case 1:
                {
                    if (ClicksCounter < 2)
                    {
                        RectCoord[NrRectangles][ClicksCounter].x = xPos;
                        RectCoord[NrRectangles][ClicksCounter].y = yPos;
                        ClicksCounter++;
                        printf("%d\n", NrRectangles);
                        printf("%d\n", ClicksCounter);
                    }
                    if (ClicksCounter == 2)
                    {
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                }
                break;

                case 2:
                {
                    if (ClicksCounter < 2)
                    {
                        EllipseCoord[NrEllipses][ClicksCounter].x = xPos;
                        EllipseCoord[NrEllipses][ClicksCounter].y = yPos;
                        ClicksCounter++;
                        printf("%d\n", NrEllipses);
                        printf("%d\n", ClicksCounter);
                    }
                    if (ClicksCounter == 2)
                    {
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                }
                break;

                case 3:
                {
                    if (ClicksCounter < 4)
                    {
                        BezCurveCoord[NrBezCurves][ClicksCounter].x = xPos;
                        BezCurveCoord[NrBezCurves][ClicksCounter].y = yPos;
                        ClicksCounter++;
                        printf("%d\n", NrBezCurves);
                        printf("%d\n", ClicksCounter);
                    }
                    if (ClicksCounter == 4)
                    {
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                }
                break;
            }
        }
        break;

        // Left mouse button release event
        case WM_LBUTTONUP:
        {
            int xPos = LOWORD(lParam);  // Horizontal position of cursor
            int yPos = HIWORD(lParam);  // Vertical position of cursor

            // Restricting the zone where left mouse button release event can occur
            if (xPos < 0) {xPos = 0;}
            if (xPos > 420) {xPos = 420;}
            if (yPos < 20) {yPos = 20;}
            if (yPos > 400) {yPos = 400;}

            // Stores the coordinates of the second point. Only for "Drag and Drop" mouse ability method
            if (MouseAbility == 1 && ClicksCounter == 1)
            {
                switch(DrawFigure)
                {
                    case 1:
                    {
                        RectCoord[NrRectangles][ClicksCounter].x = xPos;
                        RectCoord[NrRectangles][ClicksCounter].y = yPos;
                        ClicksCounter++;
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                    break;

                    case 2:
                    {
                        EllipseCoord[NrEllipses][ClicksCounter].x = xPos;
                        EllipseCoord[NrEllipses][ClicksCounter].y = yPos;
                        ClicksCounter++;
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                    break;

                    case 3:
                    {
                        BezCurveCoord[NrBezCurves][ClicksCounter].x = xPos;
                        BezCurveCoord[NrBezCurves][ClicksCounter].y = yPos;
                        ClicksCounter++;
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                    break;
                }
            }

            // Specific case meant for Bezier curve drawing. Separated because of 'ClicksCounter', in order to avoid errors by making the condition available to other cases
            else if (MouseAbility == 1 && ClicksCounter == 3)
            {
                switch(DrawFigure)
                {
                    case 3:
                    {
                        BezCurveCoord[NrBezCurves][ClicksCounter].x = xPos;
                        BezCurveCoord[NrBezCurves][ClicksCounter].y = yPos;
                        ClicksCounter++;
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                    break;
                }
            }
        }
        break;

        // Mouse move event
        case WM_MOUSEMOVE:
        {
            // General declarations
            int xPos = LOWORD(lParam);  // Horizontal position of cursor
            int yPos = HIWORD(lParam);  // Vertical position of cursor

            // Current position of the cursor. Will be used in 'WM_PAINT' for real-time view of the figure during its drawing
            if (ClicksCounter >= 1 && MouseAbility == 0)
            {
                CurrentPos.x = xPos;
                CurrentPos.y = yPos;
                InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
            }
            else if (ClicksCounter >= 1 && MouseAbility == 1 && wParam == MK_LBUTTON)
            {
                CurrentPos.x = xPos;
                CurrentPos.y = yPos;
                InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
            }
        }
        break;

        // Painting
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

            // Entitling the working area
            DrawText(hdc, "Working area:" , lstrlen("Working area:"), &WorkingAreaTitleRect, DT_CENTER);

            // Switch cases for real-time view of the figure during its drawing
            switch(MouseAbility)
            {
                case 0:
                {
                    switch(DrawFigure)
                    {
                        case 0:
                        {
                            // Writing some information below the working area
                            DrawText(hdc, "Info: Mouse ability is set to 'Click'." , lstrlen("Info: Mouse ability is set to 'Click'."), &WorkingAreaInfoRect, DT_LEFT);
                        }
                        break;

                        case 1:
                        {
                            if (ClicksCounter == 1)
                            {
                                Rectangle(hdc, RectCoord[NrRectangles][0].x, RectCoord[NrRectangles][0].y, CurrentPos.x, CurrentPos.y);
                            }
                            else if (ClicksCounter == 2)
                            {
                                NrRectangles++;
                                ClicksCounter = 0;
                                DrawFigure = 0;
                            }
                        }
                        break;

                        case 2:
                        {
                            if (ClicksCounter == 1)
                            {
                                Arc(hdc, EllipseCoord[NrEllipses][0].x, EllipseCoord[NrEllipses][0].y, CurrentPos.x, CurrentPos.y, 0, 0, 0, 0);
                            }
                            else if (ClicksCounter == 2)
                            {
                                NrEllipses++;
                                ClicksCounter = 0;
                                DrawFigure = 0;
                            }
                        }
                        break;

                        case 3:
                        {
                            POINT* LocBezCurve = new POINT[4];
                            switch(ClicksCounter)
                            {
                                case 1:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = CurrentPos.x;
                                    LocBezCurve[3].y = CurrentPos.y;
                                }
                                break;

                                case 2:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = BezCurveCoord[NrBezCurves][1].x;
                                    LocBezCurve[3].y = BezCurveCoord[NrBezCurves][1].y;
                                    LocBezCurve[1].x = CurrentPos.x;
                                    LocBezCurve[1].y = CurrentPos.y;
                                }
                                break;

                                case 3:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = BezCurveCoord[NrBezCurves][1].x;
                                    LocBezCurve[3].y = BezCurveCoord[NrBezCurves][1].y;
                                    LocBezCurve[1].x = BezCurveCoord[NrBezCurves][2].x;
                                    LocBezCurve[1].y = BezCurveCoord[NrBezCurves][2].y;
                                    LocBezCurve[2].x = CurrentPos.x;
                                    LocBezCurve[2].y = CurrentPos.y;
                                }
                                break;

                                case 4:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = BezCurveCoord[NrBezCurves][1].x;
                                    LocBezCurve[3].y = BezCurveCoord[NrBezCurves][1].y;
                                    LocBezCurve[1].x = BezCurveCoord[NrBezCurves][2].x;
                                    LocBezCurve[1].y = BezCurveCoord[NrBezCurves][2].y;
                                    LocBezCurve[2].x = BezCurveCoord[NrBezCurves][3].x;
                                    LocBezCurve[2].y = BezCurveCoord[NrBezCurves][3].y;

                                    int *i = new int;
                                    for (*i=0; (*i)<ClicksCounter; (*i)++)
                                    {
                                        BezCurveCoord[NrBezCurves][*i].x = LocBezCurve[*i].x;
                                        BezCurveCoord[NrBezCurves][*i].y = LocBezCurve[*i].y;
                                    }
                                    delete i;

                                    NrBezCurves++;
                                    ClicksCounter = 0;
                                    DrawFigure = 0;
                                }
                                break;
                            }
                            PolyBezier(hdc, LocBezCurve, 4);
                            delete [] LocBezCurve;
                        }
                        break;
                    }
                }
                break;

                case 1:
                {
                    switch(DrawFigure)
                    {
                        case 0:
                        {
                            // Writing some information below the working area
                            DrawText(hdc, "Info: Mouse ability is set to 'Drag and Drop'." , lstrlen("Info: Mouse ability is set to 'Drag and Drop'."), &WorkingAreaInfoRect, DT_LEFT);
                        }
                        break;

                        case 1:
                        {
                            if (ClicksCounter == 1)
                            {
                                Rectangle(hdc, RectCoord[NrRectangles][0].x, RectCoord[NrRectangles][0].y, CurrentPos.x, CurrentPos.y);
                            }
                            else if (ClicksCounter == 2)
                            {
                                NrRectangles++;

                                ClicksCounter = 0;
                                DrawFigure = 0;
                            }
                        }
                        break;

                        case 2:
                        {
                            if (ClicksCounter == 1)
                            {
                                Arc(hdc, EllipseCoord[NrEllipses][0].x, EllipseCoord[NrEllipses][0].y, CurrentPos.x, CurrentPos.y, 0, 0, 0, 0);
                            }
                            else if (ClicksCounter == 2)
                            {
                                NrEllipses++;

                                ClicksCounter = 0;
                                DrawFigure = 0;
                            }
                        }
                        break;

                        case 3:
                        {
                            POINT* LocBezCurve = new POINT[4];
                            switch(ClicksCounter)
                            {
                                case 1:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = CurrentPos.x;
                                    LocBezCurve[3].y = CurrentPos.y;
                                }
                                break;

                                case 2:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = BezCurveCoord[NrBezCurves][1].x;
                                    LocBezCurve[3].y = BezCurveCoord[NrBezCurves][1].y;
                                    LocBezCurve[1].x = CurrentPos.x;
                                    LocBezCurve[1].y = CurrentPos.y;
                                }
                                break;

                                case 3:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = BezCurveCoord[NrBezCurves][1].x;
                                    LocBezCurve[3].y = BezCurveCoord[NrBezCurves][1].y;
                                    LocBezCurve[1].x = BezCurveCoord[NrBezCurves][2].x;
                                    LocBezCurve[1].y = BezCurveCoord[NrBezCurves][2].y;
                                    LocBezCurve[2].x = CurrentPos.x;
                                    LocBezCurve[2].y = CurrentPos.y;
                                }
                                break;

                                case 4:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = BezCurveCoord[NrBezCurves][1].x;
                                    LocBezCurve[3].y = BezCurveCoord[NrBezCurves][1].y;
                                    LocBezCurve[1].x = BezCurveCoord[NrBezCurves][2].x;
                                    LocBezCurve[1].y = BezCurveCoord[NrBezCurves][2].y;
                                    LocBezCurve[2].x = BezCurveCoord[NrBezCurves][3].x;
                                    LocBezCurve[2].y = BezCurveCoord[NrBezCurves][3].y;

                                    int *i = new int;
                                    for (*i=0; (*i)<ClicksCounter; (*i)++)
                                    {
                                        BezCurveCoord[NrBezCurves][*i].x = LocBezCurve[*i].x;
                                        BezCurveCoord[NrBezCurves][*i].y = LocBezCurve[*i].y;
                                    }
                                    delete i;

                                    NrBezCurves++;
                                    ClicksCounter = 0;
                                    DrawFigure = 0;
                                }
                                break;
                            }
                            PolyBezier(hdc, LocBezCurve, 4);
                            delete [] LocBezCurve;
                        }
                        break;
                    }
                }
                break;
            }

            // With this block of code, the recorded figures will be redrawn each time window refresh occurs
            int *i = new int;
            for (*i=0; (*i)<NrRectangles; (*i)++)
            {
               Rectangle(hdc, RectCoord[*i][0].x, RectCoord[*i][0].y, RectCoord[*i][1].x, RectCoord[*i][1].y);
            }
            for (*i=0; (*i)<NrEllipses; (*i)++)
            {
               Arc(hdc, EllipseCoord[*i][0].x, EllipseCoord[*i][0].y, EllipseCoord[*i][1].x, EllipseCoord[*i][1].y, 0, 0, 0, 0);
            }
            for (*i=0; (*i)<NrBezCurves; (*i)++)
            {
               PolyBezier(hdc, BezCurveCoord[*i], 4);
            }
            delete i;

            // Reseting the figures counter to avoid overflow, which will in turn clean the window
            if (NrRectangles == 20 || NrEllipses == 20 || NrBezCurves == 20)
            {
                NrRectangles = 0; NrEllipses = 0; NrBezCurves = 0;
            }

            SelectObject (hdc, hPenOld);
            DeleteObject(hPen);

            // Conditioning the redraw of the manually created figures
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

                // Drawing a Bezier Curve
                BezCurveBI[0].x = 50;
                BezCurveBI[0].y = 200;
                BezCurveBI[1].x = 50;
                BezCurveBI[1].y = 380;
                BezCurveBI[2].x = 150;
                BezCurveBI[2].y = 300;
                BezCurveBI[3].x = 350;
                BezCurveBI[3].y = 200;
                PolyBezier(hdc, BezCurveBI, 4);
            }

            EndPaint (hwnd, &ps);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

return 0;
}


// Performs program entry point operations
void Initialization(void)
{
    // Setting the working area title rectangle coordinates
    WorkingAreaTitleRect.left = 0;
    WorkingAreaTitleRect.top = 0;
    WorkingAreaTitleRect.right = 420;
    WorkingAreaTitleRect.bottom = 20;

    // Setting the working area rectangle coordinates
    WorkingAreaRect.left = 0;
    WorkingAreaRect.top = 20;
    WorkingAreaRect.right = 420;
    WorkingAreaRect.bottom = 400;

    // Setting the working area info rectangle coordinates
    WorkingAreaInfoRect.left = 0;
    WorkingAreaInfoRect.top = 400;
    WorkingAreaInfoRect.right = 420;
    WorkingAreaInfoRect.bottom = 420;
}


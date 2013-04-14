#include <windows.h>
#include <iostream>
#include <cstdio>
#include <string.h>
#include <math.h>





/*///////////////////////////////////////////
1.Function prototypes & global declaratons://
///////////////////////////////////////////*/


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Initialization(void);
void Erease(int x, int y);

#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

#define WndWidth  640
#define WndHeight 460
#define IDC_CLEAR 20
#define IDC_MouseAbility 21
#define IDC_Rectangle 22
#define IDC_Ellipse 23
#define IDC_Bezier 24
HWND hwndMain;                                                      // Handle of the main window
HWND hwndButton[5];                                                 // Handles of the buttons from the main window
HDC hdc;
PAINTSTRUCT ps;
HPEN hPenOld;
RECT GeneralPurposeRect;
RECT WorkingAreaRect;
RECT WorkingAreaTitleRect;
RECT WorkingAreaInfoRect;
POINT CurrentPos;                                                   // Used to pass the current cursor coordinates from 'WM_MOUSEMOVE' to 'WM_PAINT'
POINT BezCurveBI[4];                                                // Keeps the coordinates required to draw a Bezier curve. Reserved for the built-in Bezier curve
POINT** RectCoord = new POINT* [20];                                // Keeps the coordinates required to draw a rectangle
POINT** EllipseCoord = new POINT* [20];                             // Keeps the coordinates required to draw an ellipse
POINT** BezCurveCoord = new POINT* [20];                            // Keeps the coordinates required to draw a Bezier curve
int NrRectangles;
int NrBezCurves;
int NrEllipses;
int DrawStartupObjects = 1;                                         // {1 - yes; 0 - no}
int MouseAbility = 0;                                               // {0 - Use mouse clicks to draw figures; 1 - use drag and drop method to draw figures}
int DrawFigure = 0;                                                 // {1 - Draw a rectangle; 2 - Draw an ellipse; 3 - Draw a bezier curve; 9 - Ereaser}
int ClicksCounter = 0;                                              // Based on the number of clicks, the program determines what figure needs to be drawn
int ZoomFlag;                                                       // {0 - initial state; 1 - figure selection state; 2 - zoomed state}
int ZoomIndex;                                                      // Index value of the currently selected figure. Ranges from 0 -> NrRectangles+NrEllipses+NrBezCurves





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
                              "Laboratory Work #3",                 // Window title
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

    // Registering hotkeys
    RegisterHotKey(hwndMain, 1, NULL, 0x0D);                        // Enter(carriage return) key
    RegisterHotKey(hwndMain, 2, NULL, VK_LEFT);                     // Left Arrow key
    RegisterHotKey(hwndMain, 3, NULL, VK_RIGHT);                    // Right Arrow key

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

        // System buttons
        case WM_SYSCOMMAND:
            switch(wParam)
            {
                // Disabling the window "Maximize" functionality
                case SC_MAXIMIZE:
                break;

                default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        break;

        // Title bar double click event. Disabling.
        case WM_NCLBUTTONDBLCLK:
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
                    ZoomIndex = 0;
                    ZoomFlag = 0;
                    InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    InvalidateRect(hwnd, &WorkingAreaInfoRect, TRUE);

                    // Logging the "Clear Screen" event in the console
                    printf("Clear Screen \n");
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
                    ZoomIndex = 0;
                    ZoomFlag = 0;
                    InvalidateRect(hwnd, &WorkingAreaInfoRect, TRUE);

                    if (ClicksCounter > 0)
                    {
                        ClicksCounter = 0;
                        RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                        printf("Cancel \n");
                    }

                }
                break;

                case IDC_Ellipse:
                {
                    DrawFigure = 2;
                    ZoomIndex = 0;
                    ZoomFlag = 0;
                    InvalidateRect(hwnd, &WorkingAreaInfoRect, TRUE);

                    if (ClicksCounter > 0)
                    {
                        ClicksCounter = 0;
                        RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                        printf("Cancel \n");
                    }
                }
                break;

                case IDC_Bezier:
                {
                    DrawFigure = 3;
                    ZoomIndex = 0;
                    ZoomFlag = 0;
                    InvalidateRect(hwnd, &WorkingAreaInfoRect, TRUE);

                    if (ClicksCounter > 0)
                    {
                        ClicksCounter = 0;
                        RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                        printf("Cancel \n");
                    }
                }
                break;
            }
        }
        break;

        // Processes keyboard key combinations
        case WM_HOTKEY:
            switch(wParam)
            {
                case 1:  // Enter(carriage return) key
                {
                    if      (ZoomFlag == 0) {ZoomFlag = 1; ZoomIndex = 0;}
                    else if (ZoomFlag == 1) {ZoomFlag = 2;}
                    else if (ZoomFlag == 2) {ZoomFlag = 0; ZoomIndex = 0;}
                    InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                }
                break;

                case 2:  // Left Arrow key
                {
                    if (ZoomFlag == 1)
                    {
                        if (ZoomIndex == 0)
                        {
                            // Setting the index from the min value to the max possible value, for cycling
                            ZoomIndex = (NrRectangles + NrEllipses + NrBezCurves - 1);
                        }
                        else {ZoomIndex--;}
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                }
                break;

                case 3:  // Right Arrow key
                {
                   if (ZoomFlag == 1)
                    {
                        if (ZoomIndex == (NrRectangles + NrEllipses + NrBezCurves - 1))
                        {
                            // Setting the index from the max possible value to the min value, for cycling
                            ZoomIndex = 0;
                        }
                        else {ZoomIndex++;}
                        InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
                    }
                }
                break;
            }
        break;

        // Left mouse button press event
        case WM_LBUTTONDOWN:
        {
            // General declarations
            int xPos = LOWORD(lParam);  // Horizontal position of cursor
            int yPos = HIWORD(lParam);  // Vertical position of cursor

            // Checking if cursor position isn't beyond the working area bounds
            if (xPos < 0 || xPos > 420 || yPos < 20 || yPos > 400) {break;}

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

                        // Writing some information in the console
                        if (ClicksCounter == 1)
                        {
                            printf("Nr of constructed rectangles: %d \n", NrRectangles);
                        }
                        printf("Clicks counter: %d \n", ClicksCounter);
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

                        // Writing some information in the console
                        if (ClicksCounter == 1)
                        {
                            printf("Nr of constructed ellipses: %d \n", NrEllipses);
                        }
                        printf("Clicks counter: %d \n", ClicksCounter);
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

                        // Writing some information in the console
                        if (ClicksCounter == 1)
                        {
                            printf("Nr of constructed Bezier curves: %d \n", NrBezCurves);
                        }
                        printf("Clicks counter: %d \n", ClicksCounter);
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

        // Right mouse button press event
        case WM_RBUTTONDOWN:
        {
            // General declarations
            int xPos = LOWORD(lParam);  // Horizontal position of cursor
            int yPos = HIWORD(lParam);  // Vertical position of cursor

            // Checking if cursor position isn't beyond the working area bounds
            if (xPos < 0 || xPos > 420 || yPos < 20 || yPos > 400) {break;}

            // Isolating "Cancel" event from the "Erease" event. Preventing figures deletion while zooming
            if (DrawFigure == 0 && ClicksCounter == 0 && ZoomFlag!=2)
            {
                Erease(xPos, yPos);
                InvalidateRect(hwnd, &WorkingAreaRect, TRUE);
            }
            else
            {
                // Cancel feature
                DrawFigure = 0;
                ClicksCounter = 0;
                ZoomFlag = 0;
                RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                printf("Cancel \n");
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

                // Logging the mouse left button release event for the "Drag and Drop" method in the console
                printf("Clicks counter: %d \n", ClicksCounter);
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

                        // Logging the mouse left button release event for the "Drag and Drop" method in the console
                        printf("Clicks counter: %d \n", ClicksCounter);
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
                InvalidateRect(hwnd, &WorkingAreaRect, FALSE);
            }
            else if (ClicksCounter >= 1 && MouseAbility == 1 && wParam == MK_LBUTTON)
            {
                CurrentPos.x = xPos;
                CurrentPos.y = yPos;
                InvalidateRect(hwnd, &WorkingAreaRect, FALSE);
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
                            // Writing some information below the working area
                            DrawText(hdc, "Info: Rectangle figure selected. Mouse ability is set to *Click*." ,
                                     lstrlen("Info: Rectangle figure selected. Mouse ability is set to *Click*."), &WorkingAreaInfoRect, DT_LEFT);

                            if (ClicksCounter == 1)
                            {
                                Rectangle(hdc, RectCoord[NrRectangles][0].x, RectCoord[NrRectangles][0].y, CurrentPos.x, CurrentPos.y);
                            }
                            else if (ClicksCounter == 2)
                            {
                                NrRectangles++;
                                ClicksCounter = 0;
                                DrawFigure = 0;

                                // Redrawing the 'WorkingAreaInfoRect' rectangle
                                RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                            }
                        }
                        break;

                        case 2:
                        {
                            // Writing some information below the working area
                            DrawText(hdc, "Info: Ellipse figure selected. Mouse ability is set to *Click*." ,
                                     lstrlen("Info: Ellipse figure selected. Mouse ability is set to *Click*."), &WorkingAreaInfoRect, DT_LEFT);

                            if (ClicksCounter == 1)
                            {
                                Arc(hdc, EllipseCoord[NrEllipses][0].x, EllipseCoord[NrEllipses][0].y, CurrentPos.x, CurrentPos.y, 0, 0, 0, 0);
                            }
                            else if (ClicksCounter == 2)
                            {
                                NrEllipses++;
                                ClicksCounter = 0;
                                DrawFigure = 0;

                                // Redrawing the 'WorkingAreaInfoRect' rectangle
                                RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                            }
                        }
                        break;

                        case 3:
                        {
                            // Writing some information below the working area
                            DrawText(hdc, "Info: Bezier curve selected. Mouse ability is set to *Click*." ,
                                     lstrlen("Info: Bezier curve selected. Mouse ability is set to *Click*."), &WorkingAreaInfoRect, DT_LEFT);

                            POINT* LocBezCurve = new POINT[4];
                            switch(ClicksCounter)
                            {
                                case 1:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = CurrentPos.x;
                                    LocBezCurve[3].y = CurrentPos.y;
                                    // Setting the midpoints for unknown values, making the line straight
                                    LocBezCurve[1].x = (BezCurveCoord[NrBezCurves][0].x + CurrentPos.x)/2;
                                    LocBezCurve[1].y = (BezCurveCoord[NrBezCurves][0].y + CurrentPos.y)/2;
                                    LocBezCurve[2].x = (BezCurveCoord[NrBezCurves][0].x + CurrentPos.x)/2;
                                    LocBezCurve[2].y = (BezCurveCoord[NrBezCurves][0].y + CurrentPos.y)/2;

                                    PolyBezier(hdc, LocBezCurve, 4);
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
                                    // Setting the midpoints for unknown values, making the line straight
                                    LocBezCurve[2].x = (BezCurveCoord[NrBezCurves][0].x + CurrentPos.x)/2;
                                    LocBezCurve[2].y = (BezCurveCoord[NrBezCurves][0].y + CurrentPos.y)/2;

                                    PolyBezier(hdc, LocBezCurve, 4);
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

                                    PolyBezier(hdc, LocBezCurve, 4);
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

                                    PolyBezier(hdc, LocBezCurve, 4);

                                    // Redrawing the 'WorkingAreaInfoRect' rectangle
                                    RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                                }
                                break;
                            }
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
                            DrawText(hdc, "Info: Mouse ability is set to 'Drag and Drop'." ,
                                     lstrlen("Info: Mouse ability is set to 'Drag and Drop'."), &WorkingAreaInfoRect, DT_LEFT);
                        }
                        break;

                        case 1:
                        {
                            // Writing some information below the working area
                            DrawText(hdc, "Info: Rectangle figure selected. Mouse ability is set to *D and D*.",
                                     lstrlen("Info: Rectangle figure selected. Mouse ability is set to *D and D*."), &WorkingAreaInfoRect, DT_LEFT);

                            if (ClicksCounter == 1)
                            {
                                Rectangle(hdc, RectCoord[NrRectangles][0].x, RectCoord[NrRectangles][0].y, CurrentPos.x, CurrentPos.y);
                            }
                            else if (ClicksCounter == 2)
                            {
                                NrRectangles++;
                                ClicksCounter = 0;
                                DrawFigure = 0;

                                // Redrawing the 'WorkingAreaInfoRect' rectangle
                                RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                            }
                        }
                        break;

                        case 2:
                        {
                            // Writing some information below the working area
                            DrawText(hdc, "Info: Ellipse figure selected. Mouse ability is set to *D and D*.",
                                     lstrlen("Info: Ellipse figure selected. Mouse ability is set to *D and D*."), &WorkingAreaInfoRect, DT_LEFT);

                            if (ClicksCounter == 1)
                            {
                                Arc(hdc, EllipseCoord[NrEllipses][0].x, EllipseCoord[NrEllipses][0].y, CurrentPos.x, CurrentPos.y, 0, 0, 0, 0);
                            }
                            else if (ClicksCounter == 2)
                            {
                                NrEllipses++;
                                ClicksCounter = 0;
                                DrawFigure = 0;

                                // Redrawing the 'WorkingAreaInfoRect' rectangle
                                RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                            }
                        }
                        break;

                        case 3:
                        {
                            // Writing some information below the working area
                            DrawText(hdc, "Info: Bezier curve selected. Mouse ability is set to *D and D*.",
                                     lstrlen("Info: Bezier curve selected. Mouse ability is set to *D and D*."), &WorkingAreaInfoRect, DT_LEFT);

                            POINT* LocBezCurve = new POINT[4];
                            switch(ClicksCounter)
                            {
                                case 1:
                                {
                                    LocBezCurve[0].x = BezCurveCoord[NrBezCurves][0].x;
                                    LocBezCurve[0].y = BezCurveCoord[NrBezCurves][0].y;
                                    LocBezCurve[3].x = CurrentPos.x;
                                    LocBezCurve[3].y = CurrentPos.y;
                                    // Setting the midpoints for unknown values, making the line straight
                                    LocBezCurve[1].x = (BezCurveCoord[NrBezCurves][0].x + CurrentPos.x)/2;
                                    LocBezCurve[1].y = (BezCurveCoord[NrBezCurves][0].y + CurrentPos.y)/2;
                                    LocBezCurve[2].x = (BezCurveCoord[NrBezCurves][0].x + CurrentPos.x)/2;
                                    LocBezCurve[2].y = (BezCurveCoord[NrBezCurves][0].y + CurrentPos.y)/2;

                                    PolyBezier(hdc, LocBezCurve, 4);
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
                                    // Setting the midpoints for unknown values, making the line straight
                                    LocBezCurve[2].x = (BezCurveCoord[NrBezCurves][0].x + CurrentPos.x)/2;
                                    LocBezCurve[2].y = (BezCurveCoord[NrBezCurves][0].y + CurrentPos.y)/2;

                                    PolyBezier(hdc, LocBezCurve, 4);
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

                                    PolyBezier(hdc, LocBezCurve, 4);
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

                                    PolyBezier(hdc, LocBezCurve, 4);

                                    // Redrawing the 'WorkingAreaInfoRect' rectangle
                                    RedrawWindow(hwnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
                                }
                                break;
                            }
                            delete [] LocBezCurve;
                        }
                        break;
                    }
                }
                break;
            }

            // Section of code responsible for zooming and figures drawing
            if (ZoomFlag == 1)
            {
                // Highlight the figure focused for zooming
                qColor = RGB(0, 255, 0);
                hPen = CreatePen(PS_SOLID, 7, qColor);
                hPenOld = (HPEN)SelectObject(hdc, hPen);
                if (ZoomIndex < NrRectangles)
                {
                    Rectangle(hdc, RectCoord[ZoomIndex][0].x, RectCoord[ZoomIndex][0].y, RectCoord[ZoomIndex][1].x, RectCoord[ZoomIndex][1].y);
                }
                else if ((ZoomIndex >= NrRectangles) && (ZoomIndex < NrRectangles+NrEllipses))
                {
                    Arc(hdc, EllipseCoord[ZoomIndex-NrRectangles][0].x, EllipseCoord[ZoomIndex-NrRectangles][0].y,
                        EllipseCoord[ZoomIndex-NrRectangles][1].x, EllipseCoord[ZoomIndex-NrRectangles][1].y, 0, 0, 0, 0);
                }
                else if ((ZoomIndex >= NrRectangles+NrEllipses) && (ZoomIndex < NrRectangles+NrEllipses+NrBezCurves))
                {
                    PolyBezier(hdc, BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses], 4);
                }
                SelectObject (hdc, hPenOld);
                DeleteObject(hPen);
            }
            if (ZoomFlag == 2)
            {
                // Declarations
                float *ratio = new float;
                float **corrections = new float* [3];
                int *i = new int;
                // Setting the initial values
                for ((*i)=0;(*i)<3;(*i)++) {corrections[*i] = new float [2];}

                // Rectangles
                if (ZoomIndex < NrRectangles)
                {
                    // Smallest number from the ratio of Screen_Dim/Object_Dim over X axys and Y axys
                    *ratio = min(420.0/(max(RectCoord[ZoomIndex][0].x, RectCoord[ZoomIndex][1].x)
                                        -min(RectCoord[ZoomIndex][0].x, RectCoord[ZoomIndex][1].x)),
                                 380.0/(max(RectCoord[ZoomIndex][0].y, RectCoord[ZoomIndex][1].y)
                                        -min(RectCoord[ZoomIndex][0].y, RectCoord[ZoomIndex][1].y)));

                    // Logging the current "scale ratio" in the console
                    printf("Scale ratio = %f\n", *ratio);

                    // Coordinates of the object after centering and scaling {0 - along the X axys; 1 - along the Y axys}
                    corrections[0][0] = 210 - (*ratio*(max(RectCoord[ZoomIndex][0].x, RectCoord[ZoomIndex][1].x)
                                                    -min(RectCoord[ZoomIndex][0].x, RectCoord[ZoomIndex][1].x))/2.0);
                    corrections[0][1] = 190 - (*ratio*(max(RectCoord[ZoomIndex][0].y, RectCoord[ZoomIndex][1].y)
                                                    -min(RectCoord[ZoomIndex][0].y, RectCoord[ZoomIndex][1].y))/2.0);

                    // Drawing the object with some indent
                    Rectangle(hdc, corrections[0][0]+20, corrections[0][1]+40, 420-20-corrections[0][0], 380-corrections[0][1]);
                }

                // Ellipses
                else if ((ZoomIndex >= NrRectangles) && (ZoomIndex < NrRectangles+NrEllipses))
                {
                    // Smallest number from the ratio of Screen_Dim/Object_Dim over X axys and Y axys
                    *ratio = min(420.0/(max(EllipseCoord[ZoomIndex-NrRectangles][0].x, EllipseCoord[ZoomIndex-NrRectangles][1].x)
                                        -min(EllipseCoord[ZoomIndex-NrRectangles][0].x, EllipseCoord[ZoomIndex-NrRectangles][1].x)),
                                 380.0/(max(EllipseCoord[ZoomIndex-NrRectangles][0].y, EllipseCoord[ZoomIndex-NrRectangles][1].y)
                                        -min(EllipseCoord[ZoomIndex-NrRectangles][0].y, EllipseCoord[ZoomIndex-NrRectangles][1].y)));

                    // Logging the current "scale ratio" in the console
                    printf("Scale ratio = %f\n", *ratio);

                    // Coordinates of the object after centering and scaling {0 - along the X axys; 1 - along the Y axys}
                    corrections[0][0] = 210 - (*ratio*(max(EllipseCoord[ZoomIndex-NrRectangles][0].x, EllipseCoord[ZoomIndex-NrRectangles][1].x)
                                                    -min(EllipseCoord[ZoomIndex-NrRectangles][0].x, EllipseCoord[ZoomIndex-NrRectangles][1].x))/2.0);
                    corrections[0][1] = 190 - (*ratio*(max(EllipseCoord[ZoomIndex-NrRectangles][0].y, EllipseCoord[ZoomIndex-NrRectangles][1].y)
                                                    -min(EllipseCoord[ZoomIndex-NrRectangles][0].y, EllipseCoord[ZoomIndex-NrRectangles][1].y))/2.0);

                    // Drawing the object with some indent
                    Arc(hdc, corrections[0][0]+20, corrections[0][1]+40, 420-20-corrections[0][0], 380-corrections[0][1], 0, 0, 0, 0);
                }

                // Bezier curves
                else if ((ZoomIndex >= NrRectangles+NrEllipses) && (ZoomIndex < NrRectangles+NrEllipses+NrBezCurves))
                {
                    // Declarations
                    float *midpoint = new float[2];
                    float *MaxOffset = new float[2];
                    POINT* LocBezCurve = new POINT[4];
                    POINT* PointsOffset = new POINT[4];
                    // Setting the initial values
                    MaxOffset[0] = 0;
                    MaxOffset[1] = 0;

                    // Computing the midpoint for the position points "0 & 3" along X axys and Y axys
                    midpoint[0] = (max(BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][0].x, BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][3].x)
                                    +min(BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][0].x, BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][3].x))/2;
                    midpoint[1] = (max(BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][0].y, BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][3].y)
                                    +min(BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][0].y, BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][3].y))/2;

                    // Computing the distance and direction between the midpoint and curve points. Determining the maximum offset
                    for ((*i)=0;(*i)<4;(*i)++)
                    {
                        PointsOffset[*i].x = BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][*i].x - midpoint[0];
                        PointsOffset[*i].y = BezCurveCoord[ZoomIndex-NrRectangles-NrEllipses][*i].y - midpoint[1];

                        // Calculating the maximum offset
                        MaxOffset[0] = max(MaxOffset[0], abs(PointsOffset[*i].x));  // Along the X axys
                        MaxOffset[1] = max(MaxOffset[1], abs(PointsOffset[*i].y));  // Along the Y axys
                    }

                    // Calculating the ratio. Reducing the real Width/Height by 20pt for making a small space between the figure endpoints and client area bounds
                    *ratio = min(380.0/(MaxOffset[0]), 360.0/(MaxOffset[1]));

                    // Logging the current "scale ratio" in the console
                    printf("Scale ratio = %f\n", *ratio);

                    // Centering and scaling the figure
                    for ((*i)=0;(*i)<4;(*i)++)
                    {
                        LocBezCurve[*i].x = 210.0+((*ratio)*PointsOffset[*i].x)/2.0;
                        LocBezCurve[*i].y = 200.0+((*ratio)*PointsOffset[*i].y)/2.0;
                    }

                    // Drawing the object
                    PolyBezier(hdc, LocBezCurve, 4);

                    // Removing the locally created variables
                    delete [] midpoint;
                    delete [] MaxOffset;
                    delete [] LocBezCurve;
                    delete [] PointsOffset;
                }

                // Removing the locally created variables
                delete ratio;
                for ((*i)=0;(*i)<3;(*i)++) {delete [] corrections[*i];}
                delete [] corrections;
                delete i;
            }
            // Section of code responsible for figures drawing. Figures are always drawn if 'ZoomFlag' !=2
            else
            {
                // Declarations
                int *i = new int;

                // With this block of code, the recorded figures will be redrawn each time window refresh occurs
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

                // Reseting the figures counter & nulling the coordinates buffers to avoid overflow, which will in turn clean the window
                if (NrRectangles == 20 || NrEllipses == 20 || NrBezCurves == 20)
                {
                    for (*i=0; (*i)<20; (*i)++)
                    {
                        RectCoord[*i][0].x = NULL;
                        RectCoord[*i][0].y = NULL;
                        RectCoord[*i][1].x = NULL;
                        RectCoord[*i][1].y = NULL;

                        EllipseCoord[*i][0].x = NULL;
                        EllipseCoord[*i][0].y = NULL;
                        EllipseCoord[*i][1].x = NULL;
                        EllipseCoord[*i][1].y = NULL;

                        BezCurveCoord[*i][0].x = NULL;
                        BezCurveCoord[*i][0].y = NULL;
                        BezCurveCoord[*i][1].x = NULL;
                        BezCurveCoord[*i][1].y = NULL;
                        BezCurveCoord[*i][2].x = NULL;
                        BezCurveCoord[*i][2].y = NULL;
                        BezCurveCoord[*i][3].x = NULL;
                        BezCurveCoord[*i][3].y = NULL;
                    }
                    NrRectangles = 0; NrEllipses = 0; NrBezCurves = 0;

                    // Also reseting the zoom buffers
                    ZoomFlag = 0;
                    ZoomIndex = 0;
                }

                // Removing the locally created variables
                delete i;

                // Conditioning the redraw of the built-in startup figures
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

                    // Drawing a gradient filled rectangle
                    RECT* FRect = new RECT;
                    float* Step = new float;
                    int *i = new int;
                    HBRUSH Brush;
                    Rectangle(hdc, 180, 110, 250, 140);
                    *Step = 30.0f / 256.0f;
                    for (*i = 0; (*i) < 256; (*i)++)
                    {
                        SetRect(FRect,
                        180,                                // Upper left X
                        (int)(110+(*i)*(*Step)),            // Upper left Y
                        250,                                // Lower right X
                        (int)(110+((*i)+1)*(*Step)));       // Lower right Y
                        // Setting the brush color
                        Brush = CreateSolidBrush(RGB(0, 0, (255 - (*i))));
                        // Filling the rectangle
                        FillRect(hdc, FRect, Brush);
                        // Reseting the brush color
                        DeleteObject(Brush);
                    }
                    //Removing the locally created variables
                    DeleteObject(Brush);
                    delete i;
                    delete Step;
                    delete FRect;

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
    // Printing program description in the console
    printf("***************************************************\n");
    printf("Title:    Laboratory Work #3                 ******\n");
    printf("                                             ******\n");
    printf("Functions:                                   ******\n");
    printf("          RClick:        Cancel/Delete       ******\n");
    printf("          Mouse Drawing: Clicks/Drag and Drop******\n");
    printf("          Zoom:          Enter,Arrow keys    ******\n");
    printf("                                             ******\n");
    printf("Figures:  Rectangle, Ellipse, Bezier curve   ******\n");
    printf("***************************************************\n");
    printf("\n\n");
    printf("***************************************************\n");
    printf("******                 Log:                  ******\n");
    printf("***************************************************\n");
    printf("\n");

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

    for (int i=0; i<20; i++)
    {
        RectCoord[i] = new POINT [2];
        EllipseCoord[i] = new POINT [2];
        BezCurveCoord[i] = new POINT [4];
    }
}


// Provides the ability to erease objects with one click on the right mouse button
void Erease(int x, int y)
{
    int i = 0;
    int j = 0;
    int ContinueFlag;
    int DeletedRectangles = 0;
    int DeletedEllipses = 0;
    int DeletedBezCourves = 0;
    int RecentFlag = 0;

    do{
        ContinueFlag = 0;   // This flag will indicate whether there might be an overlapped object with matching coordinates on other indexes. {1 - Until all objects are verified}
        RecentFlag = 0;     /* This flag indicates whether there was an object subjected to deletion.
                                Used for determining if there is a possibility that a matching coordinates object was translated from the next index to the current one */

        if (i<NrRectangles)
        {
            // Verifying if the coordinates match
            if ((RectCoord[i][0].x < x && RectCoord[i][1].x > x) || (RectCoord[i][0].x > x && RectCoord[i][1].x < x))
            {
                if ((RectCoord[i][0].y < y && RectCoord[i][1].y > y) || (RectCoord[i][0].y > y && RectCoord[i][1].y < y))
                {
                    printf("The rectangle #%d, with coordinates (%d,%d),(%d,%d) was deleted \n", i,RectCoord[i][0].x,RectCoord[i][0].y,RectCoord[i][1].x,RectCoord[i][1].y);

                    // Reallocating array cells
                    for (j=i; j<NrRectangles; j++)
                    {
                        // Checking if the last element was reached
                        if ((j+1) == NrRectangles)
                        {
                            // Nulling data cells of the deleted elements
                            RectCoord[j][0].x = NULL;
                            RectCoord[j][0].y = NULL;
                            RectCoord[j][1].x = NULL;
                            RectCoord[j][1].y = NULL;

                            // Couning the number of rectangles subjected to deletion
                            DeletedRectangles++;
                            // A rectangle was subjected to deletion
                            RecentFlag = 1;
                            break;
                        }
                        RectCoord[j][0].x = RectCoord[j+1][0].x;
                        RectCoord[j][0].y = RectCoord[j+1][0].y;
                        RectCoord[j][1].x = RectCoord[j+1][1].x;
                        RectCoord[j][1].y = RectCoord[j+1][1].y;
                    }
                }
            }
            // Checking for other rectangles with matching coordinates
            ContinueFlag = 1;
        }

        if (i<NrEllipses)
        {
            if ((EllipseCoord[i][0].x < x && EllipseCoord[i][1].x > x) || (EllipseCoord[i][0].x > x && EllipseCoord[i][1].x < x))
            {
                if ((EllipseCoord[i][0].y < y && EllipseCoord[i][1].y > y) || (EllipseCoord[i][0].y > y && EllipseCoord[i][1].y < y))
                {
                    printf("The ellipse #%d, with coordinates (%d,%d),(%d,%d) was deleted \n", i,EllipseCoord[i][0].x,EllipseCoord[i][0].y,EllipseCoord[i][1].x,EllipseCoord[i][1].y);
                    // Reallocating array cells
                    for (j=i; j<NrEllipses; j++)
                    {
                        // Checking if the last element was reached
                        if ((j+1) == NrEllipses)
                        {
                            // Nulling data cells of the deleted elements
                            EllipseCoord[j][0].x = NULL;
                            EllipseCoord[j][0].y = NULL;
                            EllipseCoord[j][1].x = NULL;
                            EllipseCoord[j][1].y = NULL;

                            // Couning the number of ellipses subjected to deletion
                            DeletedEllipses++;
                            // An ellipse was subjected to deletion
                            RecentFlag = 1;
                            break;
                        }
                        EllipseCoord[j][0].x = EllipseCoord[j+1][0].x;
                        EllipseCoord[j][0].y = EllipseCoord[j+1][0].y;
                        EllipseCoord[j][1].x = EllipseCoord[j+1][1].x;
                        EllipseCoord[j][1].y = EllipseCoord[j+1][1].y;
                    }
                }
            }
            // Checking for other ellipses with matching coordinates
            ContinueFlag = 1;
        }

        if (i<NrBezCurves)
        {
            if ((BezCurveCoord[i][0].x < x && BezCurveCoord[i][3].x > x) || (BezCurveCoord[i][0].x > x && BezCurveCoord[i][3].x < x) || (BezCurveCoord[i][0].x == x && BezCurveCoord[i][3].x == x))
            {
                if ((BezCurveCoord[i][0].y < y && BezCurveCoord[i][3].y > y) || (BezCurveCoord[i][0].y > y && BezCurveCoord[i][3].y < y) || (BezCurveCoord[i][0].y == y && BezCurveCoord[i][3].y == y))
                {
                    printf("The Bezier curve #%d, with position coordinates (%d,%d),(%d,%d) was deleted \n", i,BezCurveCoord[i][0].x,BezCurveCoord[i][0].y,BezCurveCoord[i][3].x,BezCurveCoord[i][3].y);
                    // Reallocating array cells
                    for (j=i; j<NrBezCurves; j++)
                    {
                        // Checking if the last element was reached
                        if ((j+1) == NrBezCurves)
                        {
                            // Nulling data cells of the deleted elements
                            BezCurveCoord[j][0].x = NULL;
                            BezCurveCoord[j][0].y = NULL;
                            BezCurveCoord[j][1].x = NULL;
                            BezCurveCoord[j][1].y = NULL;
                            BezCurveCoord[j][2].x = NULL;
                            BezCurveCoord[j][2].y = NULL;
                            BezCurveCoord[j][3].x = NULL;
                            BezCurveCoord[j][3].y = NULL;

                            // Couning the number of Bezier courves subjected to deletion
                            DeletedBezCourves++;
                            // A Bezier courve was subjected to deletion
                            RecentFlag = 1;
                            break;
                        }
                        BezCurveCoord[j][0].x = BezCurveCoord[j+1][0].x;
                        BezCurveCoord[j][0].y = BezCurveCoord[j+1][0].y;
                        BezCurveCoord[j][1].x = BezCurveCoord[j+1][1].x;
                        BezCurveCoord[j][1].y = BezCurveCoord[j+1][1].y;
                        BezCurveCoord[j][2].x = BezCurveCoord[j+1][2].x;
                        BezCurveCoord[j][2].y = BezCurveCoord[j+1][2].y;
                        BezCurveCoord[j][3].x = BezCurveCoord[j+1][3].x;
                        BezCurveCoord[j][3].y = BezCurveCoord[j+1][3].y;
                    }
                }
            }
            // Checking for other Bezier courves with matching coordinates
            ContinueFlag = 1;
        }

        if (ContinueFlag & !RecentFlag) {i++;}
    }while(ContinueFlag);

    // Updating the index
    NrRectangles = NrRectangles-DeletedRectangles;
    NrEllipses = NrEllipses-DeletedEllipses;
    NrBezCurves = NrBezCurves-DeletedBezCourves;
}

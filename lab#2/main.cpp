#include <windows.h>
#include <iostream>
#include <cstdio>
#include <string.h>

#include "resource.h"


// 1.Function prototypes & global declaratons:

BOOL CALLBACK DialogProc(HWND H,UINT M,WPARAM W,LPARAM L);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HBRUSH SelectColor(int nr);


#define IDC_DIALOG   100
#define IDC_LISTBOX  101
#define MENU_INFO    102
#define MENU_DISPLAY 103
#define MENU_CLOSE   104

#define WndWidth  420
#define WndHeight 260

HWND hwndMain, hwndDialogButton, hwndDisplayWnd, hwndListBox;
RECT WndRect;
RECT DisplayWndRect;
RECT DisplayWndRectText[4];







// 2.Main function:

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{

// Declarations:
    MSG msg;
    WNDCLASSEX wcx;



// Standard definitions:
    wcx.cbSize = sizeof(WNDCLASSEX);                                // Must always be sizeof(WNDCLASSEX)
    wcx.style = CS_DBLCLKS;                                         // Class styles
    wcx.lpfnWndProc = MainWndProc;                                  // Pointer to callback procedure
    wcx.cbClsExtra = 0;                                             // Extra bytes to allocate following the wndclassex structure
    wcx.cbWndExtra = 0;                                             // Extra bytes to allocate following an instance of the structure
    wcx.hInstance = hInst;                                          // Instance of the application
    wcx.hIcon = LoadIcon(NULL, IDI_WARNING);                        // Class Icon
    wcx.hCursor = LoadCursor(NULL, IDC_APPSTARTING);                // Class cursor
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



// Creating the menu:
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING, MENU_INFO, "Info");
    AppendMenu(hFileMenu, MF_STRING, MENU_DISPLAY, "Display  [ALT+D]");
    AppendMenu(hFileMenu, MF_STRING, MENU_CLOSE, "Close     [ALT+X]");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hFileMenu, "File");

    SetMenu(hwndMain,hMenu);



// Make the window visible
    ShowWindow(hwndMain,SW_SHOW);



// Registering hotkeys
    RegisterHotKey(hwndMain, 1, MOD_ALT, 0x44);
    RegisterHotKey(hwndMain, 2, MOD_ALT, 0x58);



// Process messages coming to this window
    while (GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}




// 3.Functions:

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

            hwndDialogButton = CreateWindow(
                                            "BUTTON",                                               // predefined class
                                            "Dialog Box",                                           // button text
                                            WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,               // styles
                                            0,                                                      // starting x position
                                            WndHeight - 95,                                         // starting y position
                                            WndWidth,                                               // button width
                                            40,                                                     // button height
                                            hwnd,                                                   // parent window
                                            (HMENU)IDC_DIALOG,                                      // ID
                                            GetModuleHandle(NULL),                                  // HDC
                                            NULL);


            hwndListBox = CreateWindow(
                                    "LISTBOX",                                              // predefined class
                                    NULL,                                                   // button text
                                    WS_BORDER | WS_CHILD | WS_VISIBLE |                     // styles
                                    ES_AUTOVSCROLL | LBS_NOTIFY,
                                    0,                                                      // starting x position
                                    60,                                                     // starting y position
                                    90,                                                     // ListBox width
                                    60,                                                     // ListBox height
                                    hwnd,                                                   // parent window
                                    (HMENU)IDC_LISTBOX,                                     // ID
                                    GetModuleHandle(NULL),                                  // HDC
                                    NULL);


            for (int i = 0; i < 3; i++)
            {
                switch(i)
                {
                    case 0:
                        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)"Allign Left       ");
                    break;

                    case 1:
                        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)"Allign Center    ");
                    break;

                    case 2:
                        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)"Allign Right     ");
                    break;
                }
            }

        break;



// Window min/max limits and dependancy on resize
        case WM_GETMINMAXINFO:

            MINMAXINFO *lpMinMaxInfo;
            lpMinMaxInfo = (MINMAXINFO*) lParam;

            // Making the main window non resizable
            lpMinMaxInfo->ptMinTrackSize.x = WndWidth;
            lpMinMaxInfo->ptMinTrackSize.y = WndHeight;
            lpMinMaxInfo->ptMaxTrackSize.x = WndWidth;
            lpMinMaxInfo->ptMaxTrackSize.y = WndHeight;
        break;


// Buttons
        case WM_COMMAND:

            switch(LOWORD(wParam))
            {
               case IDC_DIALOG:
                    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_Scroll), hwndMain, DialogProc);
               break;

               case MENU_CLOSE:
                    PostQuitMessage(0);
               break;

               case MENU_INFO:
                    MessageBox(hwnd, "Laboratory Work #2", "Message", MB_OK | MB_ICONINFORMATION);
               break;

               case MENU_DISPLAY:
                    hwndDisplayWnd = CreateWindowEx(
                                                   WS_EX_PALETTEWINDOW,
                                                   "MDICLIENT",                                  // Window class name
                                                   "Display",                                    // Window title
                                                   WS_SYSMENU,                                   // Window style
                                                   CW_USEDEFAULT, CW_USEDEFAULT,                 // (x,y) pos of the window
                                                   243, 150,                                     // Width and height of the window
                                                   hwndMain,                                     // HWND of the parent window
                                                   NULL,                                         // Handle to menu
                                                   GetModuleHandle(NULL),                        // Handle to application instance
                                                   NULL);


                    // These rectangles will be used for objects and text positioning in the 'ExtendWnd'
                    GetClientRect(hwndDisplayWnd, &DisplayWndRect);                              // This rectangle will be for reference

                    // Specifying position where Text will be drawn
                    GetClientRect(hwndDisplayWnd, &DisplayWndRectText[0]);
                    GetClientRect(hwndDisplayWnd, &DisplayWndRectText[1]);
                    GetClientRect(hwndDisplayWnd, &DisplayWndRectText[2]);
                    GetClientRect(hwndDisplayWnd, &DisplayWndRectText[3]);
                    DisplayWndRectText[0].top = 10;//DisplayWndRect.top + 10;
                    DisplayWndRectText[1].top = 50;//DisplayWndRect.top + 50;
                    DisplayWndRectText[2].top = 10;//DisplayWndRect.top + 10;
                    DisplayWndRectText[3].top = 50;//DisplayWndRect.top + 50;

                    ShowWindow(hwndDisplayWnd, SW_SHOW);
               break;
            }


            switch(HIWORD(wParam))
            {
                case LBN_SELCHANGE:                                             // ListBox selection change
                    switch(SendMessage(hwndListBox, LB_GETANCHORINDEX, 0, 0) )
                    {
                        case 0:
                            SetWindowPos(hwndListBox,HWND_TOP,0,60,90,60,SWP_SHOWWINDOW);
                        break;

                        case 1:
                            SetWindowPos(hwndListBox,HWND_TOP,156,60,90,60,SWP_SHOWWINDOW);
                        break;

                        case 2:
                            SetWindowPos(hwndListBox,HWND_TOP,314,60,90,60,SWP_SHOWWINDOW);
                        break;
                    }
                break;
            }
        break;



        case WM_HOTKEY:
            switch(wParam)
            {
                case 1:     // ALT + D
                    SendMessage(hwndMain, WM_COMMAND, MENU_DISPLAY, 0);
                break;

                case 2:     // ALT + X
                    PostQuitMessage(0);
                break;
            }


        break;


// Text positioning and fonts application
        case WM_PAINT:
        {
            HFONT font;
            HDC hdc = GetDC(hwndDisplayWnd);
            font = CreateFont(-14, 0, 0, 0, 0, ANSI_CHARSET, 0U, 0U, 0U, 0U, 0U, 0U, 0U, TEXT("Times New Roman"));


            SelectObject(hdc, font);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, 0x0000FF00);  // Font color (green)

            DrawText(hdc, "Current Position(x):" , lstrlen("Current Position(x):"), &DisplayWndRectText[0], DT_LEFT);
            DrawText(hdc, "Current Position(y):" , lstrlen("Current Position(y):"), &DisplayWndRectText[1], DT_LEFT);

            // Adjusting position for Width/Height data
            SetTextColor(hdc, 0x00FF0000);  // Font color (blue)
            DisplayWndRectText[2].left = 120;
            DisplayWndRectText[3].left = 120;

            char* tmp = new char[10];
            GetWindowRect(hwndDisplayWnd, &DisplayWndRect);

            sprintf(tmp, "%d", DisplayWndRect.left);
            DrawText(hdc, tmp, lstrlen(tmp), &DisplayWndRectText[2], DT_LEFT);
            sprintf(tmp, "%d", DisplayWndRect.top);
            DrawText(hdc, tmp, lstrlen(tmp), &DisplayWndRectText[3], DT_LEFT);

            RedrawWindow(hwndDisplayWnd, NULL, NULL, true);
            ReleaseDC(hwnd, hdc);
            DeleteObject(font);
            delete [] tmp;
        }
        UpdateWindow(hwnd);
        break;



        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }


return 0;
}


// DialogBox procedure
BOOL CALLBACK DialogProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
     int nScrollCode = (int) LOWORD(wParam);    // Scroll bar value
     int nPos = (int) HIWORD(wParam);           // scroll box position
     HWND hwndScrollBar = (HWND) lParam;        // handle of scroll bar


     SetScrollRange(
                    hwndScrollBar,      // handle of window with scroll bar
                    SB_CTL,             // scroll bar flag
                    0,                  // minimum scrolling position
                    7,                  // maximum scrolling position
                    FALSE               // redraw flag
                    );


  switch(msg)
  {

    case WM_INITDIALOG:
        SetDlgItemText(hwnd,ED_Color,"Change Color:");
        SetDlgItemText(hwnd,ED_Width,"Move Left/Right:");
        SetDlgItemText(hwnd,ED_Height,"Move Down/Up:");
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case BTN_OK:
                EndDialog(hwnd,0);
                return TRUE;
        }
    break;

    case (WM_HSCROLL):
        switch(GetDlgCtrlID(hwndScrollBar))
        {
            case ColorScr:
               switch (nScrollCode)
               {
                   case SB_LINERIGHT:
                        GetWindowRect(hwndMain, &WndRect);
                        if (GetScrollPos(hwndScrollBar, SB_CTL) < 7)
                        {
                            // Sets the ScrollBar position and changes the Main Window background color
                            SetScrollPos(hwndScrollBar, SB_CTL, GetScrollPos(hwndScrollBar, SB_CTL) + 1, TRUE);
                            SetClassLongPtr(hwndMain, GCLP_HBRBACKGROUND, (LONG)SelectColor(GetScrollPos(hwndScrollBar, SB_CTL)) );

                            //Used for refreshing the window
                            ShowWindow(hwndMain, SW_HIDE);
                            ShowWindow(hwndMain, SW_SHOW);
                        }
                   break;

                   case SB_LINELEFT:
                        GetWindowRect(hwndMain, &WndRect);
                        if (GetScrollPos(hwndScrollBar, SB_CTL) > 0)
                        {
                            SetScrollPos(hwndScrollBar, SB_CTL, GetScrollPos(hwndScrollBar, SB_CTL) - 1, TRUE);
                            SetClassLongPtr(hwndMain, GCLP_HBRBACKGROUND, (LONG)SelectColor(GetScrollPos(hwndScrollBar, SB_CTL)) );

                            ShowWindow(hwndMain, SW_HIDE);
                            ShowWindow(hwndMain, SW_SHOW);
                        }
                   break;
               }
            break;

            case HorzScr:
               switch (nScrollCode)
               {
                   case SB_LINERIGHT:
                        GetWindowRect(hwndMain, &WndRect);
                        if (GetScrollPos(hwndScrollBar, SB_CTL) < 7)
                        {
                            SetWindowPos(hwndMain,HWND_TOP,WndRect.left+1,WndRect.top,WndRect.right-WndRect.left,WndRect.bottom-WndRect.top,SWP_SHOWWINDOW);
                            SetScrollPos(hwndScrollBar, SB_CTL, GetScrollPos(hwndScrollBar, SB_CTL) + 1, TRUE);
                        }
                   break;

                   case SB_LINELEFT:
                        GetWindowRect(hwndMain, &WndRect);
                        if (GetScrollPos(hwndScrollBar, SB_CTL) > 0)
                        {
                            SetWindowPos(hwndMain,HWND_TOP,WndRect.left-1,WndRect.top,WndRect.right-WndRect.left,WndRect.bottom-WndRect.top,SWP_SHOWWINDOW);
                            SetScrollPos(hwndScrollBar, SB_CTL, GetScrollPos(hwndScrollBar, SB_CTL) - 1, TRUE);
                        }
                   break;
               }
            break;

            case VertScr:
               switch (nScrollCode)
               {
                   case SB_LINERIGHT:
                        GetWindowRect(hwndMain, &WndRect);
                        if (GetScrollPos(hwndScrollBar, SB_CTL) < 7)
                        {
                            SetWindowPos(hwndMain,HWND_TOP,WndRect.left,WndRect.top-1,WndRect.right-WndRect.left,WndRect.bottom-1 - WndRect.top,SWP_SHOWWINDOW);
                            SetScrollPos(hwndScrollBar, SB_CTL, GetScrollPos(hwndScrollBar, SB_CTL) + 1, TRUE);
                        }
                   break;

                   case SB_LINELEFT:
                        GetWindowRect(hwndMain, &WndRect);
                        if (GetScrollPos(hwndScrollBar, SB_CTL) > 0)
                        {
                            SetWindowPos(hwndMain,HWND_TOP,WndRect.left,WndRect.top + 1,WndRect.right-WndRect.left,WndRect.bottom+1-WndRect.top,SWP_SHOWWINDOW);
                            SetScrollPos(hwndScrollBar, SB_CTL, GetScrollPos(hwndScrollBar, SB_CTL) - 1, TRUE);
                        }
                   break;
               }
            break;
        }
        break;
  }
  return FALSE;
}




//Selecting the appropriate color:
HBRUSH SelectColor(int nr)
{
    switch(nr)
    {
        case 0:
            return CreateSolidBrush(0x00000000);    // Black

        case 1:
            return CreateSolidBrush(0x0000FF00);    // Green

        case 2:
            return CreateSolidBrush(0x00FF0000);    // Bright Blue

        case 3:
            return CreateSolidBrush(0x000000FF);    // Bright Red

        case 4:
            return CreateSolidBrush(0x007F7F7F);    // Gray

        case 5:
            return CreateSolidBrush(0x00FFFF00);    // Bright Yellow

        case 6:
            return CreateSolidBrush(0x00FF7F7F);    // Red Pastel

        case 7:
            return CreateSolidBrush(0x00FFFFFF);    // White
    }
}

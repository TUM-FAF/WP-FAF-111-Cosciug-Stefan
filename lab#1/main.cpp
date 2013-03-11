#include <windows.h>
#include <iostream>
#include <string.h>

// 1.Function prototypes & global declaratons:

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define MAIN_DISPLAY 100
#define IDC_LEFT 102
#define IDC_RIGHT 103
#define IDT_TIMER1 104

#define WndWidth 294
#define WndHeight 236

HFONT font;
HFONT fontStatus;
RECT wndrect;
RECT rcStatus;
RECT rcCode;
RECT rcFunc;
int FontSizeProp = 1;
int flag = 0;

char *str = TEXT(" Welcome !!!");
HWND hwndMain, hwndButton[2], hwndMainDisplay, hwndNewCreated;





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
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);                    // Class Icon
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);                      // Class cursor
    wcx.hbrBackground = (HBRUSH) (COLOR_WINDOW);                    // Background brush
    wcx.lpszMenuName = NULL;                                        // Menu Resource
    wcx.lpszClassName = "DrawLite";                                 // Name of this class
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);                  // Small icon for this class

// Register this window class with MS-Windows
    if (!RegisterClassEx(&wcx)) return 0;



// Creating the main window:
    hwndMain = CreateWindow(  "DrawLite",                                                     // Window class name
                              "A simple win32 application",                                   // Window title
                              WS_OVERLAPPEDWINDOW,                                            // Window style
                              CW_USEDEFAULT, CW_USEDEFAULT,                                   // (x,y) pos of the window
                              WndWidth, WndHeight,                                            // Width and height of the window
                              HWND_DESKTOP,                                                   // HWND of the parent window (can be null also)
                              NULL,                                                           // Handle to menu
                              hInst,                                                          // Handle to application instance
                              NULL);




// Check if window creation was successful
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








// 3.Functions:

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
        break;


// Changing default window actions (Maximize & Minimize)
        case WM_SYSCOMMAND:
            switch(wParam)
            {
                case SC_MAXIMIZE:
                    str = "Wrong button :)";
                    GetWindowRect(hwnd, &wndrect);
                    SetWindowPos(hwnd,HWND_TOP,wndrect.left+1,wndrect.top,wndrect.right-wndrect.left,wndrect.bottom-wndrect.top,SWP_SHOWWINDOW);
                break;

                case SC_MINIMIZE:
                    str = "No :P";
                    GetWindowRect(hwnd, &wndrect);
                    SetWindowPos(hwnd,HWND_TOP,wndrect.left-1,wndrect.top,wndrect.right-wndrect.left,wndrect.bottom-wndrect.top,SWP_SHOWWINDOW);
                break;

                default:
                    return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        break;


// Changing default window actions (Title bar double click)
        case WM_NCLBUTTONDBLCLK:
            str = "To exit type '-quit'";
            GetWindowRect(hwnd, &wndrect);
            SetWindowPos(hwnd,HWND_TOP,wndrect.left-1,wndrect.top,wndrect.right-wndrect.left,wndrect.bottom-wndrect.top,SWP_SHOWWINDOW);
        break;


// Buttons
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_LEFT:
                    EnableWindow(hwndButton[0], false);
                    flag = 1;
                    GetWindowRect(hwnd, &wndrect);
                    SetWindowPos(hwnd,HWND_TOP,wndrect.left+1,wndrect.top,wndrect.right-wndrect.left,wndrect.bottom-wndrect.top,SWP_SHOWWINDOW);
                    EnableWindow(hwndButton[1], true);
                break;

                case IDC_RIGHT:
                {
                    char *label = new char[11];
                    int *Numlabel = new int;
                    SendMessage(hwndButton[1],WM_GETTEXT,11,(LPARAM)label);

                    if(strcmp(label, "Deactivate") == 0) {*Numlabel = 0;}
                    else if(strcmp(label, "Enter") == 0) {*Numlabel = 1;}

                    switch(*Numlabel)
                    {
                        case 0:
                            EnableWindow(hwndButton[1], false);
                            flag = 0;
                            GetWindowRect(hwnd, &wndrect);
                            SetWindowPos(hwnd,HWND_TOP,wndrect.left-1,wndrect.top,wndrect.right-wndrect.left,wndrect.bottom-wndrect.top,SWP_SHOWWINDOW);
                            EnableWindow(hwndButton[0], true);
                        break;

                        case 1:
                        {
                           char *buffer = new char[100];
                           char *cmd=NULL;

                           SendMessage(hwndMainDisplay,WM_GETTEXT,100,(LPARAM)buffer);
                           cmd = buffer;
                           cmd = strtok(cmd,": ");
                           if (strtok(NULL,": "))
                           {
                                SendMessage(hwndMainDisplay,WM_GETTEXT,100,(LPARAM)buffer);
                                cmd = buffer;
                                cmd = strtok(cmd,"-");
                                cmd = strtok(NULL,"-");
                                if(!cmd)
                                {
                                    str = "Commands start with '-'.";
                                    GetWindowRect(hwndMain, &wndrect);
                                    SetWindowPos(hwndMain,HWND_TOP,wndrect.left-1,wndrect.top,wndrect.right-wndrect.left,wndrect.bottom-wndrect.top,SWP_SHOWWINDOW);
                                    SetFocus(hwndMain);

                                    SendMessage(hwndButton[1],WM_SETTEXT,0,(LPARAM)"Deactivate");
                                    EnableWindow(hwndButton[0], true);
                                    EnableWindow(hwndButton[1], false);
                                    break;
                                }

                                if(strcmp(cmd, "quit") == 0)
                                {
                                    PostQuitMessage(0);
                                }

                                else if(strcmp(cmd, "create") == 0)
                                {
                                      hwndNewCreated = CreateWindow(
                                                                    "EDIT",                                                 // predefined class
                                                                    "1",                                                    // Text
                                                                    WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL |                // styles
                                                                     ES_LEFT | ES_AUTOVSCROLL,
                                                                    10,                                                     // starting x position
                                                                    (14+24+50)*FontSizeProp,                                // starting y position
                                                                    20,                                                     // button width
                                                                    20,                                                     // button height
                                                                    hwnd,                                                   // parent window
                                                                    (HMENU)MAIN_DISPLAY,                                    // ID
                                                                    GetModuleHandle(NULL),
                                                                    NULL);
                                }

                                else if(strcmp(cmd, "destroy") == 0)
                                {
                                    DestroyWindow(hwndNewCreated);
                                }

                                else
                                {
                                    str = "Type '-quit' to exit.";
                                    GetWindowRect(hwndMain, &wndrect);
                                    SetWindowPos(hwndMain,HWND_TOP,wndrect.left-1,wndrect.top,wndrect.right-wndrect.left,wndrect.bottom-wndrect.top,SWP_SHOWWINDOW);
                                    SetFocus(hwndMain);
                                }

                           }
                           else
                           {
                                str = "Enter a command first.";
                                GetWindowRect(hwndMain, &wndrect);
                                SetWindowPos(hwndMain,HWND_TOP,wndrect.left-1,wndrect.top,wndrect.right-wndrect.left,wndrect.bottom-wndrect.top,SWP_SHOWWINDOW);
                                SetFocus(hwndMain);
                           }
                           delete [] buffer;

                            SendMessage(hwndButton[1],WM_SETTEXT,0,(LPARAM)"Deactivate");
                            EnableWindow(hwndButton[0], true);
                            EnableWindow(hwndButton[1], false);
                        }
                        break;

                    }
                    delete [] label;
                    delete Numlabel;
                }
                break;

                case MAIN_DISPLAY:
                    SendMessage(hwndButton[1],WM_SETTEXT,0,(LPARAM)"Enter");
                    EnableWindow(hwndButton[1], true);
                    EnableWindow(hwndButton[0], false);
                    DWORD TextSize;
                    TextSize=GetWindowTextLength(hwndMainDisplay);

                    SendMessage(hwndMainDisplay,EM_SCROLLCARET,0,0);
                    SendMessage(
                                (HWND) hwndMainDisplay,
                                (UINT) EM_SETSEL,
                                (WPARAM) TextSize,
                                (LPARAM) TextSize);

                break;

            }
        break;


// Window min/max limits and dependancy on resize
        case WM_GETMINMAXINFO:
            MINMAXINFO *lpMinMaxInfo;

            GetWindowRect(hwnd, &wndrect);

            lpMinMaxInfo = (MINMAXINFO*) lParam;
            lpMinMaxInfo->ptMinTrackSize.x = 294;
            lpMinMaxInfo->ptMinTrackSize.y = 236;


            lpMinMaxInfo->ptMaxTrackSize.x = (wndrect.bottom-wndrect.top)*586/346;
        break;


// Mainly used for objects positioning
        case WM_SIZE:
        {
            int nWidth = LOWORD(lParam);            // width of client area
            int nHeight = HIWORD(lParam);           // height of client area

            SetWindowPos(hwndMainDisplay,HWND_TOP,0,(14+24+80)*FontSizeProp,nWidth,20,SWP_SHOWWINDOW);
            SetWindowPos(hwndButton[0],HWND_TOP,0,nHeight-40,(nWidth/2),40,SWP_SHOWWINDOW);
            SetWindowPos(hwndButton[1],HWND_TOP,nWidth/2,nHeight-40,nWidth/2,40,SWP_SHOWWINDOW);

            DestroyWindow(hwndNewCreated);
        }
        break;


// Main objects initialization
        case WM_CREATE:

          hwndMainDisplay = CreateWindow(
                                        "EDIT",                                                 // predefined class
                                        "Console: ",                                            // button text
                                        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL |                // styles
                                        ES_LEFT  | ES_AUTOVSCROLL,
                                        0,                                                      // starting x position
                                        68,                                                     // starting y position
                                        258,                                                    // button width
                                        40,                                                     // button height
                                        hwnd,                                                   // parent window
                                        (HMENU)MAIN_DISPLAY,                                    // ID
                                        GetModuleHandle(NULL),
                                        NULL);

            hwndButton[0] = CreateWindow(
                                    "BUTTON",                                               // predefined class
                                    "Activate",                                             // button text
                                    WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,               // styles
                                    0,                                                      // starting x position
                                    WndHeight - 80,                                         // starting y position
                                    WndWidth/2,                                             // button width
                                    40,                                                     // button height
                                    hwnd,                                                   // parent window
                                    (HMENU)IDC_LEFT,                                        // ID
                                    GetModuleHandle(NULL),
                                    NULL);


            hwndButton[1] = CreateWindow(
                                    "BUTTON",                                               // predefined class
                                    "Deactivate",                                           // button text
                                    WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,               // styles
                                    WndWidth-WndWidth/2,                                    // starting x position
                                    WndHeight - 80,                                         // starting y position
                                    WndWidth/2,                                             // button width
                                    40,                                                     // button height
                                    hwnd,                                                   // parent window
                                    (HMENU)IDC_RIGHT,                                       // ID
                                    GetModuleHandle(NULL),
                                    NULL);
            EnableWindow(hwndButton[1], false);
        break;


// Text positioning and fonts application
        case WM_PAINT:
        {
            HDC hdc = GetDC(hwnd);

            GetWindowRect(hwnd, &wndrect);
            FontSizeProp = (wndrect.right-wndrect.left)/WndWidth;
            if(FontSizeProp > 1) {}
            else FontSizeProp = 1;

            fontStatus = CreateFont(-14*FontSizeProp, 0, 0, 0, 0, ANSI_CHARSET, 0U, 0U, 0U, 0U, 0U, 0U, 0U, TEXT("Times New Roman"));
            font = CreateFont(-24*FontSizeProp, 0, 0, 0, 0, ANSI_CHARSET, 0U, 0U, 0U, 0U, 0U, 0U, 0U, TEXT("Times New Roman"));


            SelectObject(hdc, fontStatus);
            GetClientRect(hwnd, &rcStatus);
            SetBkMode(hdc, TRANSPARENT);
            rcStatus.left = 0; rcStatus.top = 0; rcStatus.right = lstrlen("Status: ")*10*FontSizeProp;
            DrawText(hdc, "Status:" , lstrlen("Status:"), &rcStatus, DT_CENTER);
            rcStatus.left = rcStatus.right; rcStatus.right = rcStatus.left + lstrlen("active")*10*FontSizeProp;
            switch(flag)
            {
                case 0:
                    DrawText(hdc, "disabled" , lstrlen("disabled"), &rcStatus, DT_LEFT);
                break;
                case 1:
                    DrawText(hdc, "enabled" , lstrlen("enabled"), &rcStatus, DT_LEFT);
                break;
            }


            SelectObject(hdc, font);
            GetClientRect(hwnd, &rcFunc);
            rcFunc.left = 0; rcFunc.top = (14 + 20)*FontSizeProp; rcFunc.right = lstrlen(str)*10*FontSizeProp;
            rcCode.left = 0; rcCode.top = (14+24+30)*FontSizeProp;

            SetTextColor(hdc, 0x0000FF00);  // Font color (green)
            DrawText(hdc, str, lstrlen(str), &rcFunc, DT_CENTER);

            ReleaseDC(hwnd, hdc);
            DeleteObject(fontStatus);
            DeleteObject(font);
        }
        UpdateWindow(hwnd);
        break;



        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}






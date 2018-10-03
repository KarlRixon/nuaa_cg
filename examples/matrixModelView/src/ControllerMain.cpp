///////////////////////////////////////////////////////////////////////////////
// ControllerMain.cpp
// ==================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-09
// UPDATED: 2018-03-01
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <commctrl.h>                   // common controls
#include "ControllerMain.h"
#include "Log.h"

using namespace Win;


bool CALLBACK enumerateChildren(HWND childHandle, LPARAM lParam);


int ControllerMain::command(int id, int cmd, LPARAM msg)
{
    return 0;
}



int ControllerMain::close()
{
    Win::log(""); // blank line
    Win::log("Closing the application...");

    // close all child windows first
    ::EnumChildWindows(handle, (WNDENUMPROC)enumerateChildren, (LPARAM)WM_CLOSE);

    ::DestroyWindow(handle);                    // close itself
    return 0;
}



int ControllerMain::destroy()
{
    ::PostQuitMessage(0);       // exit the message loop
    Win::log("Main windows is destroyed.");
    return 0;
}


int ControllerMain::create()
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
// the width and height are for the client area
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::size(int width, int height, WPARAM wParam)
{
    RECT rect;

    // get dim of glDialog
    ::GetWindowRect(formHandle, &rect);
    int formHeight = rect.bottom - rect.top;

    // compute dim of gl window
    int glHeight = height - formHeight;

    // resize glWin and reposition glDialog
    ::SetWindowPos(glHandle, 0, 0, 0, width, glHeight, SWP_NOZORDER);
    ::SetWindowPos(formHandle, 0, 0, glHeight, width, formHeight, SWP_NOZORDER);
    ::InvalidateRect(formHandle, 0, TRUE);      // force to repaint

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// enumerate all child windows
///////////////////////////////////////////////////////////////////////////////
bool CALLBACK enumerateChildren(HWND handle, LPARAM lParam)
{
    if(lParam == WM_CLOSE)
    {
        ::SendMessage(handle, WM_CLOSE, 0, 0);  // close child windows
    }

    return true;
}




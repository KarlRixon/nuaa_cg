///////////////////////////////////////////////////////////////////////////////
// ControllerMain.cpp
// ==================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-09
// UPDATED: 2017-10-27
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <commctrl.h>                   // common controls
#include "ControllerMain.h"
#include "Log.h"

using namespace Win;


bool CALLBACK enumerateChildren(HWND childHandle, LPARAM lParam);



int ControllerMain::close()
{
    Win::log("");
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



int ControllerMain::size(int w, int h, WPARAM wParam)
{
    RECT rect;

    // get client dim of mainWin
    ::GetClientRect(handle, &rect);
    int mainClientWidth = rect.right - rect.left;
    int mainClientHeight = rect.bottom - rect.top;
    Win::log(L"Main client dim: %dx%d", mainClientWidth, mainClientHeight);

    // get dim of form glDialog
    ::GetWindowRect(formHandle, &rect);
    int formHeight = rect.bottom - rect.top;

    // compute dims of gl window
    int glWidth = mainClientWidth;
    int glHeight = mainClientHeight - formHeight;
    //Win::log(L"gl client: %dx%d", glWidth, glHeight);

    // resize glWin and reposition form
    ::SetWindowPos(glHandle, 0, 0, 0, glWidth, glHeight, SWP_NOZORDER);
    ::SetWindowPos(formHandle, 0, 0, glHeight, mainClientWidth, formHeight, SWP_NOZORDER);
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


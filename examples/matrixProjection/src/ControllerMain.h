///////////////////////////////////////////////////////////////////////////////
// ControllerMain.h
// ================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-09
// UPDATED: 2017-10-27
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_MAIN_H
#define WIN_CONTROLLER_MAIN_H

#include "Controller.h"

namespace Win
{
    class ControllerMain :public Controller
    {
    public:
        ControllerMain() {};
        ~ControllerMain() {};

        int close();
        int create();                               // for WM_CRERATE
        int destroy();                              // for WM_DESTROY
        int size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

        void setGLHandle(HWND handle)               { glHandle = handle; }
        void setFormHandle(HWND handle)             { formHandle = handle; }

    private:
        HWND glHandle;                              // handle of gl window
        HWND formHandle;                            // handle of form dialog

    };
}

#endif

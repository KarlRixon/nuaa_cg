///////////////////////////////////////////////////////////////////////////////
// ViewGL.cpp
// ==========
// View component of OpenGL window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-10
// UPDATED: 2018-06-07
///////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <GL/gl.h>
#include "ViewGL.h"
#include "Controller.h"         // for dummy GL window
#include "Window.h"             // for dummy window
#include "resource.h"
#include "glext.h"
#include "wglext.h"             // for wgl extensions after glext.h
#include "glExtension.h"
#include "Log.h"
using namespace Win;

///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ViewGL::ViewGL() : hdc(0), hglrc(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// default dtor
///////////////////////////////////////////////////////////////////////////////
ViewGL::~ViewGL()
{
}



///////////////////////////////////////////////////////////////////////////////
// delete OpenGL RC and release DC
///////////////////////////////////////////////////////////////////////////////
void ViewGL::closeContext(HWND handle)
{
    if(!hdc || !hglrc)
        return;

    // delete DC and RC
    ::wglMakeCurrent(0, 0);
    ::wglDeleteContext(hglrc);
    ::ReleaseDC(handle, hdc);

    hdc = 0;
    hglrc = 0;

    Win::log(L"Closed OpenGL rendering context (RC).");
}



///////////////////////////////////////////////////////////////////////////////
// create OpenGL rendering context
///////////////////////////////////////////////////////////////////////////////
bool ViewGL::createContext(HWND handle, int colorBits, int depthBits, int stencilBits, int msaaSamples)
{
    // retrieve a handle to a display device context
    hdc = ::GetDC(handle);

    // find best pixel format
    int pixelFormat;
    if(msaaSamples <= 0)
        pixelFormat= findPixelFormat(hdc, colorBits, depthBits, stencilBits);
    else
        pixelFormat = findPixelFormatWithAttributes(colorBits, depthBits, stencilBits, msaaSamples);

    if(pixelFormat <= 0)
    {
        ::MessageBox(0, L"Failed to choose OpenGL pixel format.", L"Error", MB_ICONEXCLAMATION | MB_OK);
        ::ReleaseDC(handle, hdc);
        return false;
    }

    // set the pixel format
    PIXELFORMATDESCRIPTOR pfd;
    ::DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd); // fill PFD attributes
    if(!::SetPixelFormat(hdc, pixelFormat, &pfd))
    {
        ::MessageBox(0, L"Cannot set a suitable pixel format.", L"Error", MB_ICONEXCLAMATION | MB_OK);
        ::ReleaseDC(handle, hdc);
        return false;
    }

    // create a new OpenGL rendering context
    hglrc = ::wglCreateContext(hdc);

    ::ReleaseDC(handle, hdc);
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// use existing RC instead creating new RC
// NOTE: the color pixel format must be same
///////////////////////////////////////////////////////////////////////////////
bool ViewGL::setContext(HWND handle, HGLRC rc, int pixelFormat)
{
    hdc = ::GetDC(handle);  // get the device context of this window
    hglrc = rc;             // use existing RC, not creating new RC

    // set the pixel format of DC
    PIXELFORMATDESCRIPTOR pfd;
    ::DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd); // fill PFD attributes
    if(!::SetPixelFormat(hdc, pixelFormat, &pfd))
    {
        ::MessageBox(0, L"Cannot set a suitable pixel format.", L"Error", MB_ICONEXCLAMATION | MB_OK);
        ::ReleaseDC(handle, hdc);
        return false;
    }

    ::ReleaseDC(handle, hdc);
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// make the rendering context active for this window
///////////////////////////////////////////////////////////////////////////////
void ViewGL::activateContext()
{
    ::wglMakeCurrent(hdc, hglrc);
}



///////////////////////////////////////////////////////////////////////////////
// choose pixel format
// By default, pdf.dwFlags is set PFD_DRAW_TO_WINDOW, PFD_DOUBLEBUFFER and PFD_SUPPORT_OPENGL.
///////////////////////////////////////////////////////////////////////////////
bool ViewGL::setPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
    PIXELFORMATDESCRIPTOR pfd;

    // find out the best matched pixel format
    int pixelFormat = findPixelFormat(hdc, colorBits, depthBits, stencilBits);
    if(pixelFormat == 0)
        return false;

    // set members of PIXELFORMATDESCRIPTOR with given mode ID
    ::DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);

    // set the pixel format
    if(!::SetPixelFormat(hdc, pixelFormat, &pfd))
        return false;

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// find the best pixel format
///////////////////////////////////////////////////////////////////////////////
int ViewGL::findPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
    int currMode;                               // pixel format mode ID
    int bestMode = 0;                           // return value, best pixel format
    int currScore = 0;                          // points of current mode
    int bestScore = 0;                          // points of best candidate
    PIXELFORMATDESCRIPTOR pfd;

    // search the available formats for the best mode
    bestMode = 0;
    bestScore = 0;
    for(currMode = 1; ::DescribePixelFormat(hdc, currMode, sizeof(pfd), &pfd) > 0; ++currMode)
    {
        // ignore if cannot support opengl
        if(!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            continue;

        // ignore if cannot render into a window
        if(!(pfd.dwFlags & PFD_DRAW_TO_WINDOW))
            continue;

        // ignore if cannot support rgba mode
        if((pfd.iPixelType != PFD_TYPE_RGBA) || (pfd.dwFlags & PFD_NEED_PALETTE))
            continue;

        // ignore if not double buffer
        if(!(pfd.dwFlags & PFD_DOUBLEBUFFER))
            continue;

        // try to find best candidate
        currScore = 0;

        // colour bits
        if(pfd.cColorBits >= colorBits) ++currScore;
        if(pfd.cColorBits == colorBits) ++currScore;

        // depth bits
        if(pfd.cDepthBits >= depthBits) ++currScore;
        if(pfd.cDepthBits == depthBits) ++currScore;

        // stencil bits
        if(pfd.cStencilBits >= stencilBits) ++currScore;
        if(pfd.cStencilBits == stencilBits) ++currScore;

        // alpha bits
        if(pfd.cAlphaBits > 0) ++currScore;

        // check if it is best mode so far
        if(currScore > bestScore)
        {
            bestScore = currScore;
            bestMode = currMode;
        }
    }

    return bestMode;
}



///////////////////////////////////////////////////////////////////////////////
// find the best pixel format with wglChoosePixelFormatARB() and attributes
// WGL_ARB_pixel_format extension must be supported to use this function.
// If the driver does not support, fall back to the the coventional method.
///////////////////////////////////////////////////////////////////////////////
int ViewGL::findPixelFormatWithAttributes(int colorBits, int depthBits, int stencilBits, int msaaSamples)
{
    // create a dummy window to check if WGL_ARB_pixel_format supported
    Win::log("Creating a dummy window to use WGL_ARB_pixel_format extension.");
    Win::Controller tmpCtrl;
    Win::Window tmpWin(::GetModuleHandle(0), L"tmp", 0, &tmpCtrl);
    tmpWin.setClassStyle(CS_OWNDC);
    HWND tmpHandle = tmpWin.create();
    HDC tmpHdc = ::GetDC(tmpHandle);                // retrieve a handle to the device context

    // first, set a pixel format using a traditional way
    int pixelFormat = findPixelFormat(tmpHdc, colorBits, depthBits, stencilBits);
    PIXELFORMATDESCRIPTOR pfd;
    ::DescribePixelFormat(tmpHdc, pixelFormat, sizeof(pfd), &pfd);
    if(!::SetPixelFormat(tmpHdc, pixelFormat, &pfd))
    {
        ::MessageBox(0, L"Cannot set a suitable pixel format for TMP window.", L"Error", MB_ICONEXCLAMATION | MB_OK);
        ::ReleaseDC(tmpHandle, tmpHdc);             // remove device context
        ::DestroyWindow(tmpHandle);
        return 0;
    }

    // create a dummy opengl RC
    HGLRC tmpHglrc = ::wglCreateContext(tmpHdc);    // create a new OpenGL rendering context
    ::wglMakeCurrent(tmpHdc, tmpHglrc);

    // validate sample value, should be power of 2
    if((msaaSamples & (msaaSamples - 1)) != 0)
    {
        // round to the next highest power of 2, referenced from "Bit Twiddling Hacks"
        msaaSamples--;
        msaaSamples |= msaaSamples >> 1;
        msaaSamples |= msaaSamples >> 2;
        msaaSamples |= msaaSamples >> 4;
        msaaSamples |= msaaSamples >> 8;
        msaaSamples |= msaaSamples >> 16;
        msaaSamples++;
    }

    // In WGL_ARB_pixel_format, colorBits excludes alphaBits
    int alphaBits = 0;
    if(colorBits == 32)
    {
        colorBits = 24;
        alphaBits = 8;
    }

    // attribute list for wglChoosePixelFormatARB()
    int attributes[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,     colorBits,
        WGL_ALPHA_BITS_ARB,     alphaBits,
        WGL_DEPTH_BITS_ARB,     depthBits,
        WGL_STENCIL_BITS_ARB,   stencilBits,
        WGL_SAMPLE_BUFFERS_ARB, 1,                  // must be 1 for MSAA
        WGL_SAMPLES_ARB,        msaaSamples,        // must be power of 2: 2,4,8,16
        0
    };

    // if WGL_ARB_pixel_format is not supported, you cannot use wglChoosePixelFormatARB()
    // if WGL_ARB_multisample is not supported, you cannot use MSAA
    glExtension& extension = glExtension::getInstance();
    if(extension.isSupported("WGL_ARB_pixel_format") && extension.isSupported("WGL_ARB_multisample"))
    {
        Win::log("WGL_ARB_pixel_format and WGL_ARB_multisample are supported.");
        Win::log("Use wglChoosePixelFormatARB() to find a pixel format with MSAA.");
        unsigned int formatCount = 0;
        BOOL result = FALSE;
        int samples = 0;
        for(samples = msaaSamples; samples > 0; samples /= 2)
        {
            // find pixel format with wglChoosePixelFormatARB()
            attributes[21] = samples;
            result = wglChoosePixelFormatARB(tmpHdc,        // handle to device context
                                             attributes,    // pointer to int attributes
                                             0,             // pointer to float attributes
                                             1,             // max pixel formats to be returned
                                             &pixelFormat,  // pointer to pixelFormat
                                             &formatCount); // returns the number of matching formats

            if(result == TRUE && formatCount > 0)
                break;
        }
        Win::log("MSAA mode: %dx", samples);
    }
    else
    {
        Win::log("[WARNING] WGL_ARB_pixel_format and WGL_ARB_multisample are NOT supported.");
        Win::log("[WARNING] MSAA cannot be enabled.");
    }
    Win::log("Pixel Format: %d", pixelFormat);

    // destroy the dummy opengl window
    ::wglMakeCurrent(0, 0);
    ::wglDeleteContext(tmpHglrc);
    ::ReleaseDC(tmpHandle, tmpHdc);
    ::DestroyWindow(tmpHandle);
    Win::log("A dummy window is destroyed.");

    return pixelFormat;
}



///////////////////////////////////////////////////////////////////////////////
// swap OpenGL frame buffers
///////////////////////////////////////////////////////////////////////////////
void ViewGL::swapBuffers()
{
    ::SwapBuffers(hdc);
}




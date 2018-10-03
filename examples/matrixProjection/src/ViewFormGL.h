///////////////////////////////////////////////////////////////////////////////
// ViewFormGL.h
// ============
// View component of OpenGL dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2008-10-02
// UPDATED: 2017-10-30
///////////////////////////////////////////////////////////////////////////////

#ifndef VIEW_FORM_GL_H
#define VIEW_FORM_GL_H

#include <windows.h>
#include "Controls.h"
#include "ModelGL.h"

namespace Win
{
    class ViewFormGL
    {
    public:
        ViewFormGL(ModelGL* model);
        ~ViewFormGL();

        void initControls(HWND handle);         // init all controls
        void updateTrackbars(HWND handle, int position);
        void setProjection(float l, float r, float b, float t, float n, float f);
        void setProjectionMode(int mode);
        void updateProjectionMatrix();

    protected:

    private:
        ModelGL* model;
        HWND parentHandle;

        // controls
        Win::RadioButton radioOrthographic;
        Win::RadioButton radioPerspective;
        Win::RadioButton radioFill;
        Win::RadioButton radioLine;
        Win::RadioButton radioPoint;
        Win::Button      buttonReset;
        Win::Button      buttonAbout;
        Win::TextBox     textLeft;
        Win::TextBox     textRight;
        Win::TextBox     textBottom;
        Win::TextBox     textTop;
        Win::TextBox     textNear;
        Win::TextBox     textFar;
        Win::Trackbar    sliderLeft;
        Win::Trackbar    sliderRight;
        Win::Trackbar    sliderBottom;
        Win::Trackbar    sliderTop;
        Win::Trackbar    sliderNear;
        Win::Trackbar    sliderFar;
        Win::TextBox     m[16];         // projection matrix
        Win::Picture     pictureMatrix;
        HBITMAP          bitmapPerspective;
        HBITMAP          bitmapOrthographic;

    };
}

#endif

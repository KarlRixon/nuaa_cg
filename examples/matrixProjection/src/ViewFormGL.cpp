///////////////////////////////////////////////////////////////////////////////
// ViewFormGL.cpp
// ==============
// View component of OpenGL dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2008-10-02
// UPDATED: 2017-10-30
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include <cmath>
#include "ViewFormGL.h"
#include "resource.h"
#include "Log.h"
#include "wcharUtil.h"
using namespace Win;

const int   SLIDER_RANGE = 100;         // total # of ticks
const float SLIDER_SCALE = 10.0f;       // # of ticks per unit
const int   SLIDER_SHIFT = 50;          // units from min to center


///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ViewFormGL::ViewFormGL(ModelGL* model) : model(model), parentHandle(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// default dtor
///////////////////////////////////////////////////////////////////////////////
ViewFormGL::~ViewFormGL()
{
    // release bitmap handles
    ::DeleteObject(bitmapPerspective);
    ::DeleteObject(bitmapOrthographic);
}



///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::initControls(HWND handle)
{
    // remember the handle to parent window
    parentHandle = handle;

    // set all controls
    radioOrthographic.set(handle, IDC_RADIO_ORTHO);
    radioPerspective.set(handle, IDC_RADIO_PERSPECTIVE);
    radioPerspective.check();

    radioFill.set(handle, IDC_RADIO_FILL);
    radioLine.set(handle, IDC_RADIO_LINE);
    radioPoint.set(handle, IDC_RADIO_POINT);
    radioFill.check();

    buttonReset.set(handle, IDC_BUTTON_RESET);
    buttonAbout.set(handle, IDC_BUTTON_ABOUT);
    buttonAbout.setImage(::LoadIcon(0, IDI_INFORMATION));

    textLeft.set(handle, IDC_LABEL_LEFT);
    textRight.set(handle, IDC_LABEL_RIGHT);
    textBottom.set(handle, IDC_LABEL_BOTTOM);
    textTop.set(handle, IDC_LABEL_TOP);
    textNear.set(handle, IDC_LABEL_NEAR);
    textFar.set(handle, IDC_LABEL_FAR);

    bitmapPerspective = (HBITMAP)::LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDB_PERSPECTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    bitmapOrthographic = (HBITMAP)::LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDB_ORTHO), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    pictureMatrix.set(handle, IDC_PICTURE_MATRIX);
    pictureMatrix.setImage(bitmapPerspective);

    sliderLeft.set(handle, IDC_SLIDER_LEFT);
    sliderLeft.setRange(0, SLIDER_RANGE);
    sliderLeft.setPos((int)(-0.5f * SLIDER_SCALE + SLIDER_SHIFT));
    sliderLeft.setPos(0);
    textLeft.setText(toWchar((sliderLeft.getPos() - SLIDER_SHIFT) / SLIDER_SCALE));

    sliderRight.set(handle, IDC_SLIDER_RIGHT);
    sliderRight.setRange(0, SLIDER_RANGE);
    sliderRight.setPos((int)(0.5f * SLIDER_SCALE + SLIDER_SHIFT));
    textRight.setText(toWchar((sliderRight.getPos() - SLIDER_SHIFT) / SLIDER_SCALE));

    sliderBottom.set(handle, IDC_SLIDER_BOTTOM);
    sliderBottom.setRange(0, SLIDER_RANGE);
    sliderBottom.setPos((int)(-0.5f * SLIDER_SCALE + SLIDER_SHIFT));
    textBottom.setText(toWchar((sliderBottom.getPos() - SLIDER_SHIFT) / SLIDER_SCALE));

    sliderTop.set(handle, IDC_SLIDER_TOP);
    sliderTop.setRange(0, SLIDER_RANGE);
    sliderTop.setPos((int)(0.5f * SLIDER_SCALE + SLIDER_SHIFT));
    textTop.setText(toWchar((sliderTop.getPos() - SLIDER_SHIFT) / SLIDER_SCALE));

    sliderNear.set(handle, IDC_SLIDER_NEAR);
    sliderNear.setRange(0, 100);
    sliderNear.setPos(1 * 10);
    textNear.setText(toWchar(sliderNear.getPos() / SLIDER_SCALE));

    sliderFar.set(handle, IDC_SLIDER_FAR);
    sliderFar.setRange(0, 100);
    sliderFar.setPos(10 * 10);
    textFar.setText(toWchar(sliderFar.getPos() / SLIDER_SCALE));

    // elements of projection matrix
    m[0].set(handle, IDC_M0);
    m[1].set(handle, IDC_M1);
    m[2].set(handle, IDC_M2);
    m[3].set(handle, IDC_M3);
    m[4].set(handle, IDC_M4);
    m[5].set(handle, IDC_M5);
    m[6].set(handle, IDC_M6);
    m[7].set(handle, IDC_M7);
    m[8].set(handle, IDC_M8);
    m[9].set(handle, IDC_M9);
    m[10].set(handle, IDC_M10);
    m[11].set(handle, IDC_M11);
    m[12].set(handle, IDC_M12);
    m[13].set(handle, IDC_M13);
    m[14].set(handle, IDC_M14);
    m[15].set(handle, IDC_M15);

    // increase font size
    for(int i = 0; i < 16; ++i)
    {
        m[i].setFont(L"MS Shell Dlg 2", 9);
    }
}



///////////////////////////////////////////////////////////////////////////////
// update trackbars
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::updateTrackbars(HWND handle, int position)
{
    float value = (position - SLIDER_SHIFT) / SLIDER_SCALE;
    if(handle == sliderLeft.getHandle())
    {
        sliderLeft.setPos(position);
        textLeft.setText(toWchar(value));
        model->setProjectionLeft(value);
    }
    else if(handle == sliderRight.getHandle())
    {
        sliderRight.setPos(position);
        textRight.setText(toWchar(value));
        model->setProjectionRight(value);
    }
    else if(handle == sliderBottom.getHandle())
    {
        sliderBottom.setPos(position);
        textBottom.setText(toWchar(value));
        model->setProjectionBottom(value);
    }
    else if(handle == sliderTop.getHandle())
    {
        sliderTop.setPos(position);
        textTop.setText(toWchar(value));
        model->setProjectionTop(value);
    }
    else if(handle == sliderNear.getHandle())
    {
        sliderNear.setPos(position);
        value = position / SLIDER_SCALE;
        textNear.setText(toWchar(value));
        model->setProjectionNear(value);
    }
    else if(handle == sliderFar.getHandle())
    {
        sliderFar.setPos(position);
        value = position / SLIDER_SCALE;
        textFar.setText(toWchar(value));
        model->setProjectionFar(value);
    }

    updateProjectionMatrix();
}



///////////////////////////////////////////////////////////////////////////////
// set projection matrix entries
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::setProjection(float l, float r, float b, float t, float n, float f)
{
    sliderLeft.setPos((int)(l * SLIDER_SCALE + SLIDER_SHIFT));
    textLeft.setText(toWchar((sliderLeft.getPos() - SLIDER_SHIFT) / SLIDER_SCALE));

    sliderRight.setPos((int)(r * SLIDER_SCALE + SLIDER_SHIFT));
    textRight.setText(toWchar((sliderRight.getPos() - SLIDER_SHIFT) / SLIDER_SCALE));

    sliderBottom.setPos((int)(b * SLIDER_SCALE + SLIDER_SHIFT));
    textBottom.setText(toWchar((sliderBottom.getPos() - SLIDER_SHIFT) / SLIDER_SCALE));

    sliderTop.setPos((int)(t * SLIDER_SCALE + SLIDER_SHIFT));
    textTop.setText(toWchar((sliderTop.getPos() - SLIDER_SHIFT) / SLIDER_SCALE));

    sliderNear.setPos((int)(n * SLIDER_SCALE));
    textNear.setText(toWchar(sliderNear.getPos() / SLIDER_SCALE));

    sliderFar.setPos((int)(f * SLIDER_SCALE));
    textFar.setText(toWchar(sliderFar.getPos() / SLIDER_SCALE));

    // make sure Model's matrix was updated before calling this
    updateProjectionMatrix();
}



///////////////////////////////////////////////////////////////////////////////
// set projection mode
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::setProjectionMode(int mode)
{
    if(mode == 1)
    {
        pictureMatrix.setImage(bitmapOrthographic);
    }
    else
    {
        pictureMatrix.setImage(bitmapPerspective);
    }
}



///////////////////////////////////////////////////////////////////////////////
// update Projection Matrix
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::updateProjectionMatrix()
{
    std::wstringstream wss;
    int i;

    // convert number to string with limited decimal points
    wss << std::fixed << std::setprecision(2);

    const float* matrix = model->getProjectionMatrixElements();
    for(i = 0; i < 16; ++i)
    {
        wss.str(L"");
        wss << matrix[i] << std::ends;
        m[i].setText(wss.str().c_str());
    }

    // unset floating format
    wss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
}


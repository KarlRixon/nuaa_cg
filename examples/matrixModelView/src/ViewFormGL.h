///////////////////////////////////////////////////////////////////////////////
// ViewFormGL.h
// ============
// View component of OpenGL dialog window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2008-09-15
// UPDATED: 2018-03-01
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
        void setViewMatrix(float x, float y, float z, float p, float h, float r);
        void setModelMatrix(float x, float y, float z, float rx, float ry, float rz);
        void updateMatrices();

    protected:

    private:
        ModelGL* model;
        HWND parentHandle;

        // controls for view section (camera)
        Win::Button   buttonResetView;
        Win::TextBox  textViewPosX;
        Win::TextBox  textViewPosY;
        Win::TextBox  textViewPosZ;
        Win::TextBox  textViewRotX;
        Win::TextBox  textViewRotY;
        Win::TextBox  textViewRotZ;
        Win::Trackbar sliderViewPosX;
        Win::Trackbar sliderViewPosY;
        Win::Trackbar sliderViewPosZ;
        Win::Trackbar sliderViewRotX;
        Win::Trackbar sliderViewRotY;
        Win::Trackbar sliderViewRotZ;
        Win::TextBox  textViewGL;

        // controls for model section
        Win::Button   buttonResetModel;
        Win::TextBox  textModelPosX;
        Win::TextBox  textModelPosY;
        Win::TextBox  textModelPosZ;
        Win::TextBox  textModelRotX;
        Win::TextBox  textModelRotY;
        Win::TextBox  textModelRotZ;
        Win::Trackbar sliderModelPosX;
        Win::Trackbar sliderModelPosY;
        Win::Trackbar sliderModelPosZ;
        Win::Trackbar sliderModelRotX;
        Win::Trackbar sliderModelRotY;
        Win::Trackbar sliderModelRotZ;
        Win::TextBox  textModelGL;

        Win::Button buttonAbout;

        // matrices
        Win::TextBox   mv[16];          // view matrix
        Win::TextBox   mm[16];          // model matrix
        Win::TextBox   mmv[16];         // modelview matrix
    };
}

#endif

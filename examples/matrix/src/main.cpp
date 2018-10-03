///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// Example of understanding OpenGL transform matrix(GL_MODELVIEW)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-03-17
// UPDATED: 2017-06-27
///////////////////////////////////////////////////////////////////////////////

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Matrices.h"
#include "Vectors.h"

using std::stringstream;
using std::cout;
using std::endl;
using std::ends;


// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

// CALLBACK function when exit() called ///////////////////////////////////////
void exitCB();

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void showInfo();
void toOrtho();
void toPerspective();
void drawAxis(float size=2.5f);
void drawGrid(float size=10.0f, float step=1.0f);
void drawModel();
void anglesToMatrix(const Vector3 angles, Matrix4& matrix);
Matrix4 setFrustum(float l, float r, float b, float t, float n, float f);
Matrix4 setFrustum(float fovY, float aspectRatio, float front, float back);
Matrix4 setOrthoFrustum(float l, float r, float b, float t, float n, float f);

// constants
const int   SCREEN_WIDTH    = 400;
const int   SCREEN_HEIGHT   = 300;
const float CAMERA_DISTANCE = 6.0f;
const int   TEXT_WIDTH      = 8;
const int   TEXT_HEIGHT     = 13;
const float DEG2RAD         = 3.141593f / 180;

// global variables
void *font = GLUT_BITMAP_8_BY_13;
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
int drawMode = 0;
Matrix4 matrixView;
Matrix4 matrixModel;
Matrix4 matrixModelView;    // = matrixView * matrixModel
Matrix4 matrixProjection;




///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Matrix3 usage example ==================================================
    Matrix3 m3_1(1,0,-2, -1,-2,-3, 1,1,0);  // column-major
    Matrix3 m3_2 = m3_1;                    // copy from m3_1
    m3_2.invert();                          // invert matrix
    std::cout << "3x3 original matrix:\n" << m3_1 << std::endl;
    std::cout << "3x3 inverse matrix:\n" << m3_2 << std::endl;

    // Matrix4 usage examples =================================================
    // Note that Matrix4 is column-major order
    std::cout << "\n ===== 4x4 Matrix =====\n" << std::endl;

    // create (construct) matrix
    float a[16] = { 2,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2 };
    Matrix4 m1;                                             // with default ctor
    Matrix4 m2(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);            // with 16 elements
    Matrix4 m3(a);                                          // with array
    Matrix4 m4(m3);                                         // with copy ctor, same as Matrix4 m4 = m3;
    //std::cout << m4 << std::endl;

    // accessors (getter/setter)
    m1.set(a);                                              // init with array
    m2.set(3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3);             // init with 16 elements
    m3 = m2;                                                // init with assignemt operator (=)
    const float* a1 = m1.get();                             // return pointer to matrix elements
    const float* a2 = m2.getTranspose();                    // return transposed matrix elements

    // subscript operator [] to access each element
    m1[0] = 3;
    std::cout << "FIRST ELEMENT: " << m1[0] << std::endl;
    std::cout << " LAST ELEMENT: " << m1[15] << "\n\n";

    // comparison (exact)
    if(m1 == m2)
        std::cout << "COMPARISON: EQUAL\n\n";
    else
        std::cout << "COMPARISON: NOT EQUAL\n\n";

    // addition
    m3 = m1 + m2;                                           // M3 = M1 + M2
    std::cout << "ADD:\n" << m3 << std::endl;

    // subtraction
    m3 = m1 - m2;                                           // M3 = M1 - M2
    std::cout << "SUBTRACT:\n" << m3 << std::endl;

    // multiplication
    m3 = m1 * m2;                                           // M3 = M1 * M2
    m3 *= m1;                                               // M3 = M3 * M1 (= glMultMatrixf(M1))
    std::cout << "MULTIPLY:\n" << m3 << std::endl;

    // scalar product (pre-mult only, not available M * s)
    m3 = 5 * m1;                                            // s * M
    std::cout << "SCALAR PRODUCT:\n" << m3 << std::endl;

    // vector multiplication
    Vector3 v1 = Vector3(2,2,2);
    Vector3 v2;
    v2 = m1 * v1;                                           // vector product: M * v
    std::cout << "VECTOR MULTIPLY 1: " << v2 << std::endl;
    v2 = v1 * m1;                                           // pre-product: v * M
    std::cout << "VECTOR MULTIPLY 2: " << v2 << "\n\n";

    // transpose
    m3.set(1,1,1,1, 2,2,2,2, 3,3,3,3, 4,4,4,4);
    m3.transpose();                                         // transpose
    std::cout << "TRANSPOSE:\n" << m3 << std::endl;

    // determinant
    m3.set(1,3,2,1, 0,0,1,0, 2,0,4,5, -1,5,-3,0);
    std::cout << "DETERMINANT:\n" << m3.getDeterminant() << "\n" << std::endl;

    // inverse of non-singluar
    m3.set(1,0,0,0, 0,2,0,0, 0,0,3,0, 0,0,0,4);             // non-singular M
    m3.invert();                                            // inverse
    std::cout << "INVERSE:\n" << m3 << std::endl;

    // invert Euclidean (rotation/reflection only)
    m3.set(-1,0,0,0, 0,0.70711f,0.70711f,0, 0,-0.70711f,0.70711f,0, 1,2,3,1);
    m3.invertEuclidean();                                   // inverse explicitly
    std::cout << "INVERSE EUCLIDEAN:\n" << m3 << std::endl;

    // translate transform
    m3.identity();
    m3.translate(1, 2, 3);                                  // = glTranslatef(x, y, z)
    //m3.translate(v1);
    std::cout << "TRANSLATE:\n" << m3 << std::endl;

    // rotate transform with degree
    m3.identity();
    m3.rotate(45, 0,1,0);                                   // = glRotatef(a, 1,0,0)
    std::cout << "ROTATE:\n" << m3 << std::endl;

    // rotate on basis axis (degree)
    m3.identity();
    m3.rotateX(45);                                         // = glRotatef(45, 1,0,0)
    //m3.rotateY(45);                                         // = glRotatef(45, 0,1,0)
    //m3.rotateZ(45);                                         // = glRotatef(45, 0,0,1)
    std::cout << "ROTATE:\n" << m3 << std::endl;
    std::cout << "ANGLE: " << m3.getAngle() << "\n\n";

    // scale
    m3.identity();
    m3.scale(1, 2, 3);                                      // = glScalef(x, y, z)
    std::cout << "SCALE:\n" << m3 << std::endl;

    // lookat
    m3.identity();
    m3.translate(0, 0, 0);
    m3.lookAt(1, 0, -1);
    std::cout << "LookAt:\n" << m3 << std::endl;
    //=========================================================================



    // init global vars
    initSharedMem();

    // register exit callback
    atexit(exitCB);

    // init GLUT and GL
    initGLUT(argc, argv);
    initGL();

    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
    glutMainLoop(); /* Start GLUT event-processing loop */

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// draw a grid on XZ-plane
///////////////////////////////////////////////////////////////////////////////
void drawGrid(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    // 20x20 grid
    glBegin(GL_LINES);

    glColor3f(0.5f, 0.5f, 0.5f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size, 0,  i);   // lines parallel to X-axis
        glVertex3f( size, 0,  i);
        glVertex3f(-size, 0, -i);   // lines parallel to X-axis
        glVertex3f( size, 0, -i);

        glVertex3f( i, 0, -size);   // lines parallel to Z-axis
        glVertex3f( i, 0,  size);
        glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(-i, 0,  size);
    }

    // x-axis
    glColor3f(1, 0, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f( size, 0, 0);

    // z-axis
    glColor3f(0,0,1);
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0,  size);

    glEnd();

    // enable lighting back
    glEnable(GL_LIGHTING);
}



///////////////////////////////////////////////////////////////////////////////
// draw the local axis of an object
///////////////////////////////////////////////////////////////////////////////
void drawAxis(float size)
{
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);

    // draw axis
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);

    // restore default settings
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}



///////////////////////////////////////////////////////////////////////////////
// draw a model (tetrahedron)
///////////////////////////////////////////////////////////////////////////////
void drawModel()
{
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.6667f, 0.6667f, 0.3334f);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 2);

    glNormal3f(-0.6667f, 0.6667f, 0.3334f);
    glVertex3f(-1, 0, 0);
    glVertex3f(0, 0, 2);
    glVertex3f(0, 1, 0);

    glNormal3f(0, 0, -1);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 0, 2);
    glVertex3f(-1, 0, 0);

    glNormal3f(0, -1, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(-1, 0, 0);
    glVertex3f(0, 1, 0);
    glEnd();
}



///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char **argv)
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode

    glutInitWindowSize(screenWidth, screenHeight);  // window size

    glutInitWindowPosition(100, 100);           // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    int handle = glutCreateWindow(argv[0]);     // param is the title of window

    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
    //glutIdleFunc(idleCB);                       // redraw whenever system is idle
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);

    return handle;
}



///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

     // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}



///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char *str, int x, int y, float color[4], void *font)
{
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);

    glColor4fv(color);          // set text color
    glRasterPos2i(x, y);        // place text position

    // loop all characters in the string
    while(*str)
    {
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// draw a string in 3D space
///////////////////////////////////////////////////////////////////////////////
void drawString3D(const char *str, float pos[3], float color[4], void *font)
{
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);

    glColor4fv(color);          // set text color
    glRasterPos3fv(pos);        // place text position

    // loop all characters in the string
    while(*str)
    {
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;

    mouseLeftDown = mouseRightDown = false;
    mouseX = mouseY = 0;

    cameraAngleX = cameraAngleY = 0;
    cameraDistance = CAMERA_DISTANCE;

    drawMode = 0; // 0:fill, 1: wireframe, 2:points

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// clean up shared memory
///////////////////////////////////////////////////////////////////////////////
void clearSharedMem()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 20, 1}; // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}



///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ)
{
    Vector3 eye(eyeX, eyeY, eyeZ);
    Vector3 target(targetX, targetY, targetZ);
    Vector3 up(0, 1, 0); // assume the camera is always straight up (roll=0)

    // compute the forward, left  and up vectors
    // NOTE: the direction is reversed (target to camera pos) because of camera transform
    Vector3 forward = eye - target;
    float distance = forward.length();  // remember the distance
    forward /= distance;    // normalize
    Vector3 left = up.cross(forward).normalize();
    up = forward.cross(left); // up vector is unit length here

    // set inverse of rotation matrix: M^-1 = M^T if it is Euclidean transform
    matrixView.identity();
    matrixView.setRow(0, left);
    matrixView.setRow(1, up);
    matrixView.setRow(2, forward);

    // set translation column
    Vector3 trans(0, 0, -distance);
    matrixView.setColumn(3, trans);
}



///////////////////////////////////////////////////////////////////////////////
// display info messages
///////////////////////////////////////////////////////////////////////////////
void showInfo()
{
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection

    float color[4] = {1, 1, 1, 1};

    stringstream ss;
    ss << std::fixed << std::setprecision(3);

    drawString("=== View Matrix ===", 0, screenHeight-TEXT_HEIGHT, color, font);
    ss << "[" << std::setw(8) << matrixView[0] << std::setw(8) << matrixView[4] << std::setw(8) << matrixView[8] << std::setw(8) << matrixView[12] << "]" << ends;
    drawString(ss.str().c_str(), 0, screenHeight-(2*TEXT_HEIGHT), color, font);
    ss.str("");
    ss << "[" << std::setw(8) << matrixView[1] << std::setw(8) << matrixView[5] << std::setw(8) << matrixView[9] << std::setw(8) << matrixView[13] << "]" << ends;
    drawString(ss.str().c_str(), 0, screenHeight-(3*TEXT_HEIGHT), color, font);
    ss.str("");
    ss << "[" << std::setw(8) << matrixView[2] << std::setw(8) << matrixView[6] << std::setw(8) << matrixView[10]<< std::setw(8) << matrixView[14] << "]" << ends;
    drawString(ss.str().c_str(), 0, screenHeight-(4*TEXT_HEIGHT), color, font);
    ss.str("");
    ss << "[" << std::setw(8) << matrixView[3] << std::setw(8) << matrixView[7] << std::setw(8) << matrixView[11]<< std::setw(8) << matrixView[15]<< "]" << ends;
    drawString(ss.str().c_str(), 0, screenHeight-(5*TEXT_HEIGHT), color, font);
    ss.str("");

    drawString("=== Model Matrix ===", 0, 4*TEXT_HEIGHT, color, font);
    ss << "[" << std::setw(8) << matrixModel[0] << std::setw(8) << matrixModel[4] << std::setw(8) << matrixModel[8] << std::setw(8) << matrixModel[12] << "]" << ends;
    drawString(ss.str().c_str(), 0, 3*TEXT_HEIGHT, color, font);
    ss.str("");
    ss << "[" << std::setw(8) << matrixModel[1] << std::setw(8) << matrixModel[5] << std::setw(8) << matrixModel[9] << std::setw(8) << matrixModel[13] << "]" << ends;
    drawString(ss.str().c_str(), 0, 2*TEXT_HEIGHT, color, font);
    ss.str("");
    ss << "[" << std::setw(8) << matrixModel[2] << std::setw(8) << matrixModel[6] << std::setw(8) << matrixModel[10]<< std::setw(8) << matrixModel[14] << "]" << ends;
    drawString(ss.str().c_str(), 0, TEXT_HEIGHT, color, font);
    ss.str("");
    ss << "[" << std::setw(8) << matrixModel[3] << std::setw(8) << matrixModel[7] << std::setw(8) << matrixModel[11]<< std::setw(8) << matrixModel[15] << "]" << ends;
    drawString(ss.str().c_str(), 0, 0, color, font);
    ss.str("");

    // unset floating format
    ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}



///////////////////////////////////////////////////////////////////////////////
// set projection matrix as orthogonal
///////////////////////////////////////////////////////////////////////////////
void toOrtho()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set orthographic viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



///////////////////////////////////////////////////////////////////////////////
// convert Euler angles(x,y,z) to matrix4
// Each column of the rotation matrix represents left, up and forward axis.
// The order of rotation is Roll->Yaw->Pitch (Rx*Ry*Rz)
// Rx: rotation about X-axis, pitch
// Ry: rotation about Y-axis, yaw(heading)
// Rz: rotation about Z-axis, roll
//    Rx           Ry          Rz
// |1  0   0| | Cy  0 Sy| |Cz -Sz 0|   | CyCz        -CySz         Sy  |
// |0 Cx -Sx|*|  0  1  0|*|Sz  Cz 0| = | SxSyCz+CxSz -SxSySz+CxCz -SxCy|
// |0 Sx  Cx| |-Sy  0 Cy| | 0   0 1|   |-CxSyCz+SxSz  CxSySz+SxCz  CxCy|
///////////////////////////////////////////////////////////////////////////////
void anglesToMatrix(const Vector3 angles, Matrix4& matrix)
{
    const float DEG2RAD = 3.141593f / 180;
    float sx, sy, sz, cx, cy, cz, theta;
    Vector3 left, up, forward;

    // rotation angle about X-axis (pitch)
    theta = angles.x * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);

    // rotation angle about Y-axis (yaw)
    theta = angles.y * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);

    // rotation angle about Z-axis (roll)
    theta = angles.z * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);

    // determine left axis
    left.x = cy*cz;
    left.y = sx*sy*cz + cx*sz;
    left.z = -cx*sy*cz + sx*sz;

    // determine up axis
    up.x = -cy*sz;
    up.y = -sx*sy*sz + cx*cz;
    up.z = cx*sy*sz + sx*cz;

    // determine forward axis
    forward.x = sy;
    forward.y = -sx*cy;
    forward.z = cx*cy;

    // write back to matrix
    matrix.setColumn(0, left);
    matrix.setColumn(1, up);
    matrix.setColumn(2, forward);
}



///////////////////////////////////////////////////////////////////////////////
// set a perspective frustum with 6 params similar to glFrustum()
// (left, right, bottom, top, near, far)
///////////////////////////////////////////////////////////////////////////////
Matrix4 setFrustum(float l, float r, float b, float t, float n, float f)
{
    Matrix4 mat;
    mat[0]  =  2 * n / (r - l);
    mat[5]  =  2 * n / (t - b);
    mat[8]  =  (r + l) / (r - l);
    mat[9]  =  (t + b) / (t - b);
    mat[10] = -(f + n) / (f - n);
    mat[11] = -1;
    mat[14] = -(2 * f * n) / (f - n);
    mat[15] =  0;
    return mat;
}



///////////////////////////////////////////////////////////////////////////////
// set a symmetric perspective frustum with 4 params similar to gluPerspective
// (vertical field of view, aspect ratio, near, far)
///////////////////////////////////////////////////////////////////////////////
Matrix4 setFrustum(float fovY, float aspectRatio, float front, float back)
{
    float tangent = tanf(fovY/2 * DEG2RAD);   // tangent of half fovY
    float height = front * tangent;           // half height of near plane
    float width = height * aspectRatio;       // half width of near plane

    // params: left, right, bottom, top, near, far
    return setFrustum(-width, width, -height, height, front, back);
}



///////////////////////////////////////////////////////////////////////////////
// set a orthographic frustum with 6 params similar to glOrtho()
// (left, right, bottom, top, near, far)
///////////////////////////////////////////////////////////////////////////////
Matrix4 setOrthoFrustum(float l, float r, float b, float t, float n, float f)
{
    Matrix4 mat;
    mat[0]  =  2 / (r - l);
    mat[5]  =  2 / (t - b);
    mat[10] = -2 / (f - n);
    mat[12] = -(r + l) / (r - l);
    mat[13] = -(t + b) / (t - b);
    mat[14] = -(f + n) / (f - n);
    return mat;
}









//=============================================================================
// CALLBACKS
//=============================================================================

void displayCB()
{
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform camera
    matrixView.identity();
    matrixView.rotate(cameraAngleY, 0, 1, 0);
    matrixView.rotate(cameraAngleX, 1, 0, 0);
    matrixView.translate(0, 0, -cameraDistance);
    //@@ the equivalent code for using OpenGL routine is:
    //@@ glTranslatef(0, 0, -cameraDistance);
    //@@ glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    //@@ glRotatef(cameraAngleY, 0, 1, 0);   // heading

    // copy view matrix to OpenGL
    glLoadMatrixf(matrixView.get());

    drawGrid();                         // draw XZ-grid with default size

    // compute model matrix
    matrixModel.identity();
    //matrixModel.rotateZ(45);        // rotate 45 degree on Z-axis
    matrixModel.rotateY(45);        // rotate 45 degree on Y-axis
    //matrixModel.rotateX(45);        // rotate 45 degree on Y-axis
    //matrixModel.translate(0, 1, 0); // move 2 unit up

    // compute modelview matrix
    matrixModelView = matrixView * matrixModel;

    // copy modelview matrix to OpenGL
    glLoadMatrixf(matrixModelView.get());

    drawAxis();
    drawModel();

    // draw info messages
    showInfo();

    glPopMatrix();

    glutSwapBuffers();
}



void reshapeCB(int w, int h)
{
    screenWidth = w;
    screenHeight = h;

    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    matrixProjection = setFrustum(45, (float)w/h, 1.0f, 100.0f);
    glLoadMatrixf(matrixProjection.get());
    //@@ the equivalent OpenGL call
    //@@ gluPerspective(45.0f, (float)(w)/h, 1.0f, 100.0f); // FOV, AspectRatio, NearClip, FarClip

    // DEBUG
    std::cout << "===== Projection Matrix =====\n";
    std::cout << matrixProjection << std::endl;

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
}


void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}


void idleCB()
{
    glutPostRedisplay();
}


void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27: // ESCAPE
        exit(0);
        break;

    case ' ':
        break;

    case 'd': // switch rendering modes (fill -> wire -> point)
    case 'D':
        drawMode = ++drawMode % 3;
        if(drawMode == 0)        // fill mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        else if(drawMode == 1)  // wireframe mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else                    // point mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        break;

    default:
        ;
    }
}


void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }
}


void mouseMotionCB(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance -= (y - mouseY) * 0.2f;
        mouseY = y;
    }
}


void exitCB()
{
    clearSharedMem();
}

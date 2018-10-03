#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tgaload.h"

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 5.0f, 25.0f, 15.0f, 1.0f };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };


GLint list[1];
GLUquadricObj *sphere;
GLUquadricObj *cylinder;

GLuint texture_id[2];

GLfloat zrot;

void Sprint( int x, int y, char *st)
{
	int l,i;

	l=strlen( st );
	glRasterPos3i( x, y, 1);
	for( i=0; i < l; i++)
		{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
	}

}



static void TimeEvent(int te)
{
	int timent;
	int i;



	glutPostRedisplay();
	glutTimerFunc( 10, TimeEvent, 1);
}


draw_badguy()
{
glBindTexture ( GL_TEXTURE_2D, texture_id[0] );
glBegin ( GL_QUADS );
   // Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -2.0f,  0.0f);
		glTexCoord2f(2.0f, 0.0f); glVertex3f( 2.0f, -2.0f,  0.0f);
		glTexCoord2f(2.0f, 2.0f); glVertex3f( 2.0f,  2.0f,  0.0f);
		glTexCoord2f(0.0f, 2.0f); glVertex3f(-2.0f,  2.0f,  0.0f);
    glEnd();

}



void init(void)
{
   int ix,iy,iz;
   float n1[3];
   image_t   temp_image;


   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glLightfv(GL_LIGHT1 ,GL_AMBIENT, LightAmbient);
   glLightfv(GL_LIGHT1 ,GL_DIFFUSE, LightDiffuse);
   glLightfv(GL_LIGHT1 ,GL_POSITION, LightPosition);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);

   glEnable ( GL_TEXTURE_2D );
   glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
   glGenTextures (2, texture_id);

   glBindTexture ( GL_TEXTURE_2D, texture_id[0] );
// glBlendFunc(GL_SRC_ALPHA,GL_ONE);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
   glEnable(GL_BLEND);							// Enable Blending       (disable alpha testing)
// glAlphaFunc(GL_GREATER,0.1f);						// Set Alpha Testing     (disable blending)
// glEnable(GL_ALPHA_TEST);						// Enable Alpha Testing  (disable blending)

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   tgaLoad  ( "wall01.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

   glBindTexture ( GL_TEXTURE_2D, texture_id[1] );
// glBlendFunc(GL_SRC_ALPHA,GL_ONE);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
   glEnable(GL_BLEND);							// Enable Blending       (disable alpha testing)
// glAlphaFunc(GL_GREATER,0.1f);						// Set Alpha Testing     (disable blending)
// glEnable(GL_ALPHA_TEST);						// Enable Alpha Testing  (disable blending)

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   tgaLoad  ( "ogl2.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );





   glEnable ( GL_CULL_FACE );

   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   glNewList(list[0], GL_COMPILE);

   sphere = gluNewQuadric();
   gluQuadricDrawStyle( sphere, GLU_FILL);
   gluQuadricNormals( sphere, GLU_SMOOTH);
   gluQuadricOrientation( sphere, GLU_OUTSIDE);
   gluQuadricTexture( sphere, GL_TRUE);

   glColor3f(0.0, 1.0, 0.0);
   gluSphere( sphere, 1, 16, 16);

   glEndList();

   cylinder = gluNewQuadric();
   gluQuadricDrawStyle(cylinder, GLU_FILL);
   gluQuadricNormals( cylinder, GLU_SMOOTH);
   gluQuadricOrientation( cylinder, GLU_OUTSIDE);
   gluQuadricTexture(cylinder, GL_TRUE);
}


void display(void)
{
float i,j;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-5.0, 5.0, -5.0, 5.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  glColor3f(1.0, 1.0, 1.0);
  Sprint(1, 1, "OpenGL" );


  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1, 1.0, 50.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  glColorMaterial(GL_FRONT, GL_AMBIENT);
  glColor3f(0.75, 0.75, 0.75);
  glColorMaterial(GL_FRONT, GL_EMISSION);
  glColor3f(0.0, 0.0, 0.0);
  glColorMaterial(GL_FRONT, GL_SPECULAR);
  glColor3f(0.75, 0.75, 0.75);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glColor3f(0.75, 0.75, 0.75);


  glPushMatrix();
  glTranslatef( 0.0, 0.0, -20.0);
  glRotatef(90, 1.0, 0.0, 0.0);
  glRotatef(345, 0.0, 1.0, 0.0);
  glRotatef(zrot++, 0.0, 0.0, 1.0);
  if (zrot > 360) zrot = 0;

// glCallList( list[0] );
  glBindTexture ( GL_TEXTURE_2D, texture_id[0] );
  gluSphere( sphere, 5, 16, 16);
  glPopMatrix();




  glColorMaterial(GL_FRONT, GL_EMISSION);
  glEnable(GL_COLOR_MATERIAL);

  glColorMaterial(GL_FRONT, GL_AMBIENT);
  glColor3f(0.85, 0.85, 0.85);
  glColorMaterial(GL_FRONT, GL_EMISSION);
  glColor3f(0.0, 0.0, 0.0);
  glColorMaterial(GL_FRONT, GL_SPECULAR);
  glColor3f(0.75, 0.75, 0.75);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glColor3f(0.75, 0.75, 0.75);

  glPushMatrix();
  glTranslatef( -1.0, -2.5, -20.0);
  glRotatef(90, 1.0, 0.0, 0.0);
  glRotatef(345, 0.0, 1.0, 0.0);
  glRotatef(zrot++, 0.0, 0.0, 1.0);
  glBindTexture ( GL_TEXTURE_2D, texture_id[1] );
  glRotatef(180, 0.0, 1.0, 0.0);
  gluCylinder( cylinder, 6, 6, 5, 16, 16 );
  glPopMatrix();

  glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
//   glMatrixMode (GL_PROJECTION);
//   glLoadIdentity ();
//   gluPerspective(60.0,(GLfloat) w/(GLfloat) h, 1.0, 50.0);
//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();

}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }


}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (10, 10);
   glutCreateWindow (argv[0]);
   glutSetWindowTitle("GLball");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutTimerFunc( 10, TimeEvent, 1);
   glutMainLoop();
   return 0;
}




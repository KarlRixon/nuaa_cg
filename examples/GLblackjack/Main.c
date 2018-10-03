// GLblackjack.c
// By Eric Stringer 2002
// E-mail: nexusone@netzero.net
// A openGL example program, plays black jack (21)
//
// Keyboard inputs: [ESC] = quit
// 'L' = enables/disables lighting
// 'V' = toggle ortho/prespective view
// 'D' = Deal cards
// 'H' = Hit (add a card to player)
// 'S' = Stand


#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "tgaload.h"


#define CARD_TEXTURES 53

typedef struct
{
	int value;
	char suit[8];
	char name[8];
}Deck;


Deck Cards[52] = {{ 1,"Ace","Hearts"},{ 2, "Two","Hearts"}, { 3, "Three", "Hearts"}, { 4, "Four","Hearts"}, { 5,"Five","Hearts"},{ 6,"Six", "Hearts06"},
{ 7,"Seven","Hearts"},{ 8,"Eight","Hearts"},{ 9,"Nine","Hearts"},{ 10,"Ten","Hearts"},{10,"Jack","Hearts"},{10,"Queen","Hearts"},{10,"King","Hearts"},
{ 1,"Ace","Clubs"},{2, "Two", "Clubs"},{3,"Three","Clubs"},{4,"Four","Clubs"},{5,"Five","Clubs"},{6,"Six","Clubs"},{7,"Seven","Clubs"},{8,"Eight","Clubs"},
{ 9,"Nine","Clubs"},{10,"Ten","Clubs"},{10,"Jack","Clubs"},{10,"Queen","Clubs"},{10,"King","Clubs"},
{ 1,"Ace","Diamonds"},{2,"Two","Diamonds"},{3,"Three","Diamonds"},{4,"Four","Diamonds"},{5,"Five","Diamonds"},{6,"Six","Diamonds"},{7,"Seven","Diamonds"},
{ 8,"Eight","Diamonds"},{9,"Nine","Diamonds"},{10,"Ten","Diamonds"},{10,"Jack","Diamonds"},{10,"Queen","Diamonds"},{10,"King","Diamonds"},
{ 1,"Ace","Spades"},{ 2,"Two","Spades"},{3,"Three","Spades"},{4,"Four","Spades"},{5,"Five","Spades"},{6,"Six","Spades"},{7,"Seven","Spades"},
{ 8,"Eight","Spades"},{ 9,"Nine","Spades"},{10,"Ten","Spades"},{10,"Jack","Spades"},{10,"Queen","Spades"},{10,"King","Spades"}};


char *CardList[] = {"hearts01.tga","hearts02.tga","hearts03.tga","hearts04.tga","hearts05.tga","hearts06.tga",
"hearts07.tga","hearts08.tga","hearts09.tga","hearts10.tga","hearts11.tga","hearts12.tga","hearts13.tga",
"clubs01.tga", "clubs02.tga","clubs03.tga","clubs04.tga","clubs05.tga","clubs06.tga","clubs07.tga","clubs08.tga",
"clubs09.tga","clubs10.tga","clubs11.tga","clubs12.tga","clubs13.tga",
"diamonds01.tga","diamonds02.tga","diamonds03.tga","diamonds04.tga","diamonds05.tga","diamonds06.tga","diamonds07.tga",
"diamonds08.tga","diamonds09.tga","diamonds10.tga","diamonds11.tga","diamonds12.tga","diamonds13.tga",
"spades01.tga","spades02.tga","spades03.tga","spades04.tga","spades05.tga","spades06.tga","spades07.tga",
"spades08.tga","spades09.tga","spades10.tga","spades11.tga","spades12.tga","spades13.tga",
"cover.tga"};

GLint deck_store_textures[53];

int card_index[52];

static int card_count = 52;

int Player_cards_location[6][2] = {{ -6,-2 },{ -4,-2},{-2,-2},{0,-2},{2,-2},{4,-2}};

int Dealer_cards_location[6][2] = {{ -6,3 },{ -4,3},{-2,3},{0,3},{2,3},{4,3}};


int Player_cards[8];
static int Player_index = 0;

int Dealer_cards[8];
static int Dealer_index = 0;


static int bust = 0; // 0 = no, 1 = Dealer, 2 = Player

static int win = 0; // 0 = no, 1 = Dealer, 2 = Player

static int blackjack = 0; // 0 = no, 1 = Dealer, 2 = Player

static int card_over = 0;

static int Player_cash = 100;
static int bet = 0;

int startup;

// define glu objects


struct tm *newtime;
time_t ltime;

GLfloat rx, ry, rz, angle;

// lighting
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 5.0f, 25.0f, 15.0f, 1.0f };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

static int light_state = 1; // light on = 1, light off = 0
static int view_state = 1; // Ortho view = 1, Perspective = 0


void Sprint( int x, int y, char *st)
{
	int l,i;

	l=strlen( st );
	glRasterPos3i( x, y, -1);
	for( i=0; i < l; i++)
		{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
	}

}

void TimeEvent( void )
{
	int timent;
	int i;

	rx = 30 * cos( angle );
	ry = 30 * sin( angle );
	rz = 30 * cos( angle );
	angle += 0.01;
	if (angle > M_TWOPI) angle = 0;


	glutPostRedisplay();
}


void Check_hands( void )
{

if (Dealer_total() == Player_total())
   {
	win = 1;
	bet = 0;
   }

if ((Dealer_total() < 22) && (Dealer_total() > Player_total()))
   {
   win = 1;
   bet = 0;
   }

if ((Player_total() < 22) && (Player_total() > Dealer_total()))
   {
   win = 2;
   Player_cash += bet * 2;
   bet = 0;
   }

if (Dealer_total() > 21)
   {
	bust = 1;
	win = 2;
	Player_cash += bet * 2;
	bet = 0;
   }

if (Player_total() > 21)
   {
   bust = 2;
   win = 1;
   bet = 0;
   }



}

void Blackjack( void )
{

if (Dealer_total() == 21)
	{
    blackjack = 1;
	win = 1;
	card_over = 1;
	bet = 0;
    }

if (Player_total() == 21)
   {
   blackjack = 2;
   win = 2;
   card_over = 1;
   Player_cash += bet * 2;
   bet = 0;
   }


}



void Reset_deck(void)
{
int i;

 for(i = 0; i < 52; i++)
	 {
	 card_index[i] = 1;
	}
card_count = 52;

}

int Pick_card( void )
{

float rndf;
int p;

rndf = rand();
rndf = (rndf / RAND_MAX);
p = rndf * 51;

if (p > 51) p = 51;
if (p < 0 ) p = 0;

return( (int) p);
}

int Player_total( void )
{
int i;
int total = 0;
int ace = 0;


for (i = 0; i < Player_index; i++)
	{
	if ((Cards[Player_cards[i]].value == 1) && ( total <= 11))
		{
		total += 11;
		ace = 1;
	    }else total += Cards[Player_cards[i]].value;
    }

if ( (total > 21) && ( ace == TRUE )) total = total - 10;

return(total);
}


int Dealer_total( void )
{
int i;
int total = 0;
int ace = 0;

for (i = 0; i < Dealer_index; i++)
	{
    if ((Cards[Dealer_cards[i]].value == 1) && ( total <= 11))
		{
		total += 11;
		ace = 1;
	    }else total += Cards[Dealer_cards[i]].value;
    }

if ( (total > 21) && ( ace == TRUE )) total = total - 10;

return(total);
}


void Deal_hit_dealer(void)
{
int p;

p = 0;

while( (card_index[ p ] == 0))
		{
		p = Pick_card();
	    }

Dealer_cards[Dealer_index] = p ;
card_index[ p ] = 0;
Dealer_index++;
card_count--;

}



void Deal_hit_player(void)
{
int p;

p = 0;
while( (card_index[ p ] == 0))
		{
		p = Pick_card();
	    }

Player_cards[Player_index] = p ;
card_index[ p ] = 0;
Player_index++;
card_count--;

}

void Deal_cards(void)
{
int i, p;

Player_index = 0;
Dealer_index = 0;
blackjack = 0;
win = 0;
bust = 0;
card_over = 0;
p = 0;

for(i=0; i < 2; i++)
	{
	while( (card_index[ p ] == 0))
		{
		p = Pick_card();
	    }

	Player_cards[i] = p ;
	card_index[ p ] = 0;
	Player_index++;
	card_count--;
   }

for(i=0; i < 2; i++)
	{
	while( (card_index[ p ] == 0))
		{
		p = Pick_card();
	    }

	Dealer_cards[i] = p ;
	card_index[ p ] = 0;
	Dealer_index++;
	card_count--;
   }

}


void Load_card_images(void)
{

int i;

char filename[32];

image_t temp_image;



glEnable( GL_TEXTURE_2D );

glPixelStorei( GL_UNPACK_ALIGNMENT,1 );
glGenTextures( CARD_TEXTURES, deck_store_textures );


for( i=0; i < CARD_TEXTURES; i++)
	{
    glBindTexture( GL_TEXTURE_2D, deck_store_textures[i] );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	strcpy( filename, "images/" );
    strncat( filename, CardList[i], 32 - strlen(filename));
	tgaLoad( filename, &temp_image, TGA_FREE | TGA_LOW_QUALITY );
}



}


void Draw_card( int x, int y, int z, int card )
{

glPushMatrix();
glTranslatef( x, y, z);
glEnable( GL_TEXTURE_2D );
glBindTexture( GL_TEXTURE_2D, deck_store_textures[card] );
glBegin(GL_QUADS);
glTexCoord2f(0.0f,0.0f ); glNormal3f(0.0, 0.0, 1.0); glVertex3f(-1.0f,-2.0f,0.0f);
glTexCoord2f(1.0f,0.0f ); glNormal3f(0.0, 0.0, 1.0); glVertex3f( 1.0f,-2.0f,0.0f);
glTexCoord2f(1.0f,1.0f ); glNormal3f(0.0, 0.0, 1.0); glVertex3f( 1.0f, 2.0f,0.0f);
glTexCoord2f(0.0f,1.0f ); glNormal3f(0.0, 0.0, 1.0); glVertex3f(-1.0f, 2.0f,0.0f);
glEnd();
glDisable( GL_TEXTURE_2D );
glPopMatrix();
}


void init(void)
{
int i;

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   // Lighting is added to scene
   glLightfv(GL_LIGHT1 ,GL_AMBIENT, LightAmbient);
   glLightfv(GL_LIGHT1 ,GL_DIFFUSE, LightDiffuse);
   glLightfv(GL_LIGHT1 ,GL_POSITION, LightPosition);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT1);


   Load_card_images();

 srand( (unsigned)time( NULL )); // Random seed

 // setup deck clear cards of being in use.
 for(i = 0; i < 52; i++)
	 {
	 card_index[i] = 1;
	}
for(i = 0; i < 6; i++)
	{
	Player_cards[i] = 0;
	Dealer_cards[i] = 0;
    }

	startup = 1;
//Deal_cards();
//Blackjack();
}


void Start_screen(void)
{
int i;

glDisable(GL_LIGHTING);
glDisable(GL_COLOR_MATERIAL);
glColor3f(1.0, 0.0, 1.0);
Sprint(-3,1,"GL BlackJack V1.0");
Sprint(-3,0,"By Eric Stringer 2002");
Sprint(-3,-1,"nexusone@netzero.net");
Sprint(-3,-7,"Press B to start");

glEnable(GL_LIGHTING);
glEnable(GL_COLOR_MATERIAL);

glColor3f(1.0, 1.0, 1.0);

for(i = 0; i < 6;i++)
	{
	Draw_card( Dealer_cards_location[i][0],Dealer_cards_location[i][1]+1,-13, Pick_card());
    Draw_card( Player_cards_location[i][0],Player_cards_location[i][1]-1,-13, Pick_card());
    }
}


void display(void)
{
  int i, pt;
  char pts[10];
  pt = 0;
  time(&ltime); // Get time
  newtime = localtime(&ltime); // Convert to local time

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// easy way to put text on the screen.
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-8.0, 8.0, -8.0, 8.0, 1.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

// Put view state on screen
glColor3f( 1.0, 1.0, 1.0);

if(startup == 0)
  {
itoa( Dealer_total(), pts, 10 );
if( (card_over == 1) ) Sprint(-4,6, pts );
glColor3f(1.0, 0.0, 0.0);
if( blackjack == 1) Sprint(-4, 3, "BlackJack!!!");
glColor3f(1.0, 1.0, 1.0);
if( win == 1) Sprint(-4, 5, "House Wins!!!");
if( bust == 1) Sprint(-4, 5, "BUST!!!");

itoa( Player_total(), pts, 10 );
  Sprint(-4,-5, pts );
glColor3f(1.0, 0.0, 0.0);
if( blackjack == 2) Sprint(-4, -2, "BlackJack!!!");
glColor3f(1.0, 1.0, 1.0);
if( win == 2) Sprint(0, -5, "Winner!!!");
if( win == 1) Sprint(0, -5, "Loser!!!");
if( bust == 2) Sprint(-4, -5, "BUST!!!");

 itoa( bet, pts, 10 );
 Sprint(-6,-6,"Bet=$");
 Sprint(-1,-6, pts);
 Sprint(3, -6,"Cash:$");
 itoa( Player_cash, pts, 10 );
 Sprint(6, -6,pts);
 Sprint(-7,-7,"[N]ew  [B]et   [D]eal   [H]it   [S]tand");
}

// Turn Perspective mode on/off
if (view_state == 0)
   {
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();

   gluPerspective(60.0, 1, 1.0, 60.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt( 0.0, 0.0, 0.0, 0.0, 0.0, -14.0, 0, 1, 0);
   }

if (light_state == 1)
   {
   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);  // Enable for lighing

   glColorMaterial(GL_FRONT, GL_AMBIENT);
   glColor3f(0.95, 0.95, 0.95);
   glColorMaterial(GL_FRONT, GL_EMISSION);
   glColor3f(0.0, 0.0, 0.0);
   glColorMaterial(GL_FRONT, GL_SPECULAR);
   glColor3f(0.75, 0.75, 0.75);
   glColorMaterial(GL_FRONT, GL_DIFFUSE);
   glColor3f(0.75, 0.75, 0.75);
   }else
   {
   glDisable(GL_LIGHTING);
   glDisable(GL_COLOR_MATERIAL);
   }

if (startup == 1) Start_screen();

for( i=0; i < Player_index; i++)
	{
    Draw_card(Player_cards_location[i][0],Player_cards_location[i][1], -14, Player_cards[i] );
//	pt +=Cards[Player_cards[i]].value;
	}


for( i=0; i < Dealer_index; i++)
	{
	if( (card_over == 0) && (i == 0))
	  {
      Draw_card(Dealer_cards_location[i][0],Dealer_cards_location[i][1], -14, 52 );
	  }else Draw_card(Dealer_cards_location[i][0],Dealer_cards_location[i][1], -14, Dealer_cards[i] );
	}


glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key)
   {
	  case 'B':
	  case 'b':
	     if(Player_cash > 0)
			 {
			 bet++;
	     	 Player_cash--;
			 if (bet == 1)
				 {
				 Dealer_index = 0;
				 Player_index = 0;
				 win = 0;
				 blackjack = 0;
				 bust = 0;
				 startup = 0;
				 }
			  }
		 break;
	  case 'D':
	  case 'd':
		 if (card_count < 3) Reset_deck();
		 if ((bet > 0) && ( Player_index < 2))
			 {
		     Deal_cards();
		     Blackjack();
			 }
		 break;
	  case 'H':
	  case 'h':
		 if (card_count < 3) Reset_deck();
		 if (Player_index > 1)
			 {
		     if( Player_total() < 22) Deal_hit_player();
		     while( Dealer_total() < 16 ) Deal_hit_dealer();
		     if( Player_total() > 21 ) Check_hands();
		     card_over = 1;
			}
		 break;
	  case 'S':
	  case 's':
		 if (card_count < 3) Reset_deck();
		 if (Player_index > 1)
			 {
		     while( Dealer_total() < 16 ) Deal_hit_dealer();
		     Check_hands();
		     card_over = 1;
			}
		 break;
	  case 'L':
	  case 'l':
	     light_state = abs(light_state - 1);
		 break;
	  case 'V':
	  case 'v':
	     view_state = abs(view_state - 1);
		 break;
	  case 'N':
	  case 'n':
	     Player_cash = 100;
		 break;
	  case 27:
         exit(0); // exit program when [ESC] key presseed
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
   glutSetWindowTitle("GL BlackJack");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutIdleFunc( TimeEvent );
   glutMainLoop();
   return 0;
}


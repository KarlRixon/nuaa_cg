/* Mystuff.h

  by Eric Stringer

  */


typedef struct
{
	GLfloat xyz[3];

}Point3F;


typedef struct
{
	int Sides;
	GLfloat x, y, z;
	GLfloat Scale;
	GLfloat Color3f[3];
	Point3F* Vertex3f;
}Primitive3F;


typedef struct   // this structure definded for the location of each pyramid
{
	int shape;
	float x;
	float y;
	float z;
	int dir_x;
	int dir_y;
	int dir_z;
}XYZ_BUFFER;


typedef struct
{
	XYZ_BUFFER lz;
	char *name;
	Point3F *Vertex;
	int speed;
	int r_rate;
	XYZ_BUFFER target;
}MyGL_object;


struct XYZ_BOUND
{
	float ax;
	float bx;
	int timex;
	float ay;
	float by;
	int timey;
	float az;
	float bz;
	int timez;
};




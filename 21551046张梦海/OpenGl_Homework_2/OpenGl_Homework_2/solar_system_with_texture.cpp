#pragma comment(lib, "legacy_stdio_definitions.lib")
# include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <gl/glaux.h>
# include <gl/glut.h>
#include <string>

static double year = 0, day = 0, startYear = 0;
static double moonday = 0, moonmonth = 0;
static double secondyear = 0, secondday = 0;
static double thirdyear = 0, thirdday = 0;
static double thirdsatelliteyear = 0, thridsatelliteday = 0;

GLuint		    texture[4];
GLUquadricObj	*q;
GLUquadricObj    *p;
GLUquadricObj    *t;
GLUquadricObj    *r;
AUX_RGBImageRec *TextureImage[4];

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File = NULL;									// File Handle
	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}
	File = fopen(Filename, "r");							// Check To See If The File Exists
	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}
	return NULL;										// If Load Failed Return NULL
}

void initImage(char * imageName, int imageNum)
{
	if (TextureImage[imageNum] = LoadBMP(imageName))
	{
		glGenTextures(1, &texture[imageNum]);					  // Create The Texture

		glBindTexture(GL_TEXTURE_2D, texture[imageNum]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[imageNum]->sizeX, TextureImage[imageNum]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[imageNum]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (TextureImage[imageNum])						// If Texture Exists
		{
			if (TextureImage[imageNum]->data)			// If Texture Image Exists
			{
				free(TextureImage[imageNum]->data);		// Free The Texture Image Memory
			}

			free(TextureImage[imageNum]);				   // Free The Image Structure 
		}
	}
}

void init(void)
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0f };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	q = gluNewQuadric();								  // Create A New Quadratic
	
	gluQuadricNormals(q, GL_SMOOTH);					  // Generate Smooth Normals For The Quad
	gluQuadricTexture(q, GL_TRUE);						  // Enable Texture Coords For The Quad

	p = gluNewQuadric();
	gluQuadricNormals(p, GL_SMOOTH);					  // Generate Smooth Normals For The Quad
	gluQuadricTexture(p, GL_TRUE);

	t = gluNewQuadric();
	gluQuadricNormals(t, GL_SMOOTH);
	gluQuadricTexture(t, GL_TRUE);

	r = gluNewQuadric();
	gluQuadricNormals(r, GL_SMOOTH);
	gluQuadricTexture(r, GL_TRUE);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping

	/*******************Init Texture**************************************/

	memset(TextureImage, 0, sizeof(void *) * 1);
	std::string imagNames[4];
	imagNames[0] = std::string("DATA / jbBall.bmp");
	imagNames[1] = std::string("DATA/sun.bmp");
	imagNames[2] = std::string("DATA/jbball2.bmp");
	imagNames[3] = std::string("DATA/muxing.bmp");
	initImage("DATA/jbBall.bmp",0);
	initImage("DATA/sun.bmp", 1);
	initImage("DATA/jbball2.bmp", 2);
	initImage("DATA/muxing.bmp", 3);
		  // Set The Pointer To NULL

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}


void display(void)
{
	









	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	/*绘画太阳*/
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);//太阳自转
	//glutSolidSphere(1.01, 20, 16);
	gluSphere(p,2.01,20,16);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//glPushMatrix();
	/*绘制行星*/
	glPushMatrix();
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);//行星公转
	glTranslatef(6.0, 0.0, 0.0);
//	glColor3f(0.0, 0.5, 0.5);
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);//行星自转
	gluSphere(q,1.0,10,8);
	//glutSolidSphere(q ,0.3, 10, 8);
	//glutWireSphere(0.3, 10, 8);

	/*绘制卫星轨道*/
	glPushMatrix();
	glColor3f(1.0, 0.0, 1.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	//glRotatef(45.0f, 0.0, 0.0, 1.0);
	glutSolidTorus(0.005, 2.0, 10, 64);
	glPopMatrix();

	/*绘画卫星*/
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glRotatef((GLfloat)moonmonth, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);
	glRotatef((GLfloat)moonday, 0.0, 1.0, 0.0);
	//glColor3f(0.5, 0.6, 0.5);
	//glutSolidSphere(0.5, 10, 8);
	gluSphere(q,0.5,10,8);
	glPopMatrix();
	glPopMatrix();

	/*绘制行星轨道*/
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 6.0, 10, 64);
	glPopMatrix();

	/*绘制第二颗行星轨道
	//glPopMatrix();
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 10.0, 10, 64);
	glPopMatrix();*/

	/*绘制第二颗行星
	glPushMatrix();
	glRotatef((GLfloat)secondyear, 0.0, 1.0, 0.0);//行星公转
	glTranslatef(10.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glRotatef((GLfloat)secondday, 0.0, 1.0, 0.0);//行星自转
	glutSolidSphere(0.35, 10, 8);
	glPopMatrix();*/

	/*绘制第三颗行星轨道*/
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 11.0, 10, 64);
	glPopMatrix();

	/*绘制第三颗行星*/
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glRotatef((GLfloat)thirdyear, 0.0, 1.0, 0.0);//行星公转
	glTranslatef(11.0, 0.0, 0.0);
	glColor3f(0.5, 0.5, 1.0);
	glRotatef((GLfloat)thirdday, 0.0, 1.0, 0.0);//行星自转
	//glutSolidSphere(0.6, 10, 8);
	gluSphere(r, 0.6, 30, 8);

	/*绘制第三颗行星的卫星轨道*/
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor3f(1.0, 0.0, 1.0);
	glRotatef(-(GLfloat)thirdday, 0.0, 1.0, 0.0);//抵消行星自转影响
	glRotatef(90.0f, 1.0, 1.0, 0.0);
	glutSolidTorus(0.005, 1.3, 10, 64);

	/*绘制第三颗行星的卫星*/
	glRotatef(-(GLfloat)thirdsatelliteyear, 0.0, 0.0, 1.0);
	glTranslatef(1.3, 0.0, 0.0);
	glRotatef((GLfloat)thridsatelliteday, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.3, 0.2);
	glutSolidSphere(0.4, 10, 8);
	glPopMatrix();
	glPopMatrix();

	glRasterPos2f(0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 8.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0);
}

void idle()
{
	moonday = moonday + 0.1;
	if (moonday >= 360)
	{
		moonday -= 360;
	}
	moonmonth = moonmonth + 0.04;
	if (moonmonth >= 360)
	{
		moonmonth -= 360;
	}
	day = day + 0.05;
	if (day >= 360)
	{
		day -= 360;
	}
	year = year + 0.03;
	if (day >= 360)
	{
		year -= 360;
	}
	startYear = startYear + 0.02;
	if (startYear >= 360)
	{
		startYear -= 360;
	}
	secondday += 0.15;
	if (secondday>=360)
	{
		secondday -= 360;
	}
	secondyear += 0.03;
	if (secondyear>=360)
	{
		secondyear -= 360;
	}
	thirdday += 0.1;
	if (thirdday>=360)
	{
		thirdday -= 360;
	}
	thirdyear += 0.02;
	if (thirdyear>=360)
	{
		thirdyear -= 360;
	}
	thirdsatelliteyear += 0.25;
	if (thirdsatelliteyear>=360)
	{
		thirdsatelliteyear -= 360;
	}
	thridsatelliteday += 0.2;
	if (thridsatelliteday>=360)
	{
		thridsatelliteday -= 360;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutRotateFunc(rotate);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
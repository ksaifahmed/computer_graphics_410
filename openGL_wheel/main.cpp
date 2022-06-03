#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include "vect.h"


//Global camera data ===============================
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 160,0,0);
			glVertex3f(-160,0,0);

			glVertex3f(0,-160,0);
			glVertex3f(0, 160,0);

		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.8, 0.8, 0.8);	//grey
		glBegin(GL_LINES);{
			for(i=-16;i<=16;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -160, 0);
				glVertex3f(i*10,  160, 0);

				//lines parallel to X-axis
				glVertex3f(-160, i*10, 0);
				glVertex3f( 160, i*10, 0);
			}
		}glEnd();
	}
}

void drawRectangle(double l, double b)
{
    glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);{
		glVertex3f( l, b,0);
		glVertex3f( l,-b,0);
		glVertex3f(-l,-b,0);
		glVertex3f(-l, b,0);
	}glEnd();
}


void drawCylinderSplice(double radius, double height, int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=height*sin(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=radius*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=radius*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{

			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glColor3f(0.3, 0.3, 0.6);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glColor3f(0.7, 0.7, 0.0);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glColor3f(0.3, 0.3, 0.6);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glColor3f(0.7, 0.7, 0.0);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawWheelRim()
{
    glRotated(90, 0, 1, 0);
    drawCylinderSplice(26, 6, 24, 20);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'w':
			break;

		case 's':
			break;

		case 'a':
			break;

		case 'd':
			break;

		default:
			break;
	}
}




void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            cameraHeight -= 3;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
    drawWheelRim();

	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=100.0;
	cameraAngle=0.8;
	angle=0;


	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

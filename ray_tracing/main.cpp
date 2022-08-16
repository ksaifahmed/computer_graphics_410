#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include "classes.hpp"

#define pi (2*acos(0.0))


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;


//Global camera data ===============================
Vector3D c_pos, u, l, r;

//Global data
int recursion_level, screenWidth, screenHeight;
vector <Object> objects;
vector <PointLight> pointlights;
vector <SpotLight> spotlights;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			axis_rotation(&r, &l, &u, 3);
			break;

		case '2':
			axis_rotation(&r, &l, &u, -3);
			break;

		case '3':
			axis_rotation(&l, &u, &r, 3);
			break;

		case '4':
			axis_rotation(&l, &u, &r, -3);
			break;


		case '5':
			axis_rotation(&u, &r, &l, 3);
			break;

		case '6':
			axis_rotation(&u, &r, &l, -3);
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			c_pos = move_along_unit_vect(c_pos, l, -2);
			break;
		case GLUT_KEY_UP:		// up arrow key
			c_pos = move_along_unit_vect(c_pos, l, 2);
			break;

		case GLUT_KEY_RIGHT:
			c_pos = move_along_unit_vect(c_pos, r, 2);
			break;
		case GLUT_KEY_LEFT:
			c_pos = move_along_unit_vect(c_pos, r, -2);
			break;

		case GLUT_KEY_PAGE_UP:
            c_pos = move_along_unit_vect(c_pos, u, 2);
			break;
		case GLUT_KEY_PAGE_DOWN:
            c_pos = move_along_unit_vect(c_pos, u, -2);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;

		case GLUT_KEY_END:
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

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    gluLookAt(c_pos.x,c_pos.y,c_pos.z,    c_pos.x+l.x,c_pos.y+l.y,c_pos.z+l.z,   u.x,u.y,u.z);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;

	//camera vectors init=========================
    u.x = 0; u.y = 0; u.z = 1;
    r.x = -1/sqrt(2); r.y = 1/sqrt(2); r.z = 0;
    l.x = -1/sqrt(2); l.y = -1/sqrt(2); l.z = 0;
    c_pos.x = 100; c_pos.y = 100; c_pos.z = 0;
	//=============================================

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


void loadData(){

    ifstream ifs("E:\\CSE 410\\ray_tracing\\scene.txt");
    if(ifs.is_open()){
        ifs >> recursion_level >> screenHeight;
        int n; string type;
        ifs >> n;

        for(int i=0; i<n; i++){
            ifs >> type;
            if(!type.compare("sphere")) {
                Sphere sp;
                sp.read_sphere(ifs);
                sp.print();
                objects.push_back(sp);
            }
            else if(!type.compare("triangle")) {
                Triangle sp;
                sp.read_triangle(ifs);
                sp.print();
                objects.push_back(sp);
            }
            else if(!type.compare("general")) {
                General sp;
                sp.read_general(ifs);
                sp.print();
                objects.push_back(sp);
            }
        }
        screenWidth = screenHeight;
    } else {
        cout << "file not found!" << endl;
        exit(0);
    }


}
int main(int argc, char **argv){
    loadData();
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

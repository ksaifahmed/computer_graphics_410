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
#include "bitmap_image.hpp"

int drawgrid;
int drawaxes;


//Global camera data ===============================
Vector3D c_pos, u, l, r;

//Global data
int recursion_level, imageWidth, imageHeight;
int bmp_counter = 0;
vector <Object *> objects;
vector <PointLight *> pointlights;
vector <SpotLight *> spotlights;
int windowHeight = 500;
int windowWidth = 500;
double viewAngle = 80.0;


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


double rad(double deg)
{
    return (pi * deg) / 180.0;
}

// capture method =======================================
void capture()
{
    cout << "===================DATA DUMP===========================\n";
	cout << "capturing at pos: ..." << c_pos;
    cout << "l: " << l << "r: " << r << "u: " << u;
    cout << "viewAngle: " << viewAngle << endl;

	// Initializing bitmap image
	bitmap_image image(imageWidth, imageHeight);
	for (int i = 0; i < imageWidth; i++)
		for (int j = 0; j < imageHeight; j++)
			image.set_pixel(i, j, 0, 0, 0);

    double planeDistance = (windowHeight/2.0) / tan(rad(viewAngle/2.0));
    Vector3D topLeft = c_pos + l*planeDistance - r*(windowWidth/2.0) + u*(windowHeight/2.0);

    cout << "planeDistance: " << planeDistance << ", topLeft: " << topLeft;

    double du = (1.0*windowWidth)/(1.0*imageWidth);
    double dv = (1.0*windowHeight)/(1.0*imageHeight);

    cout << "du is: " << du << ", dv is: " << dv << endl;

    double *color = new double[3];
    Vector3D tl_pixel = topLeft + r*(du*0.5) - u*(dv*0.5);
    cout << "topLeft pixel: " << tl_pixel;

    int nearest;
    double t, tMin;
    for(int i=0; i<imageWidth; i++) {
        for(int j=0; j<imageHeight; j++) {
            Vector3D current_pixel = tl_pixel + r*(i*du) - u*(j*dv);
//            if(i+j <4) cout << "current pixel: " << current_pixel << endl;
            Ray ray(c_pos, current_pixel-c_pos);
//            if(i+j <4) cout << "ray: " << ray.dir;

            //nearest object
            nearest = INT_MAX;
            tMin=INF;
            for(int k=0; k<objects.size(); k++) {
                color[0] = 0.0; color[1] = 0.0; color[2] = 0.0;
                t = objects.at(k)->intersect(ray, color, 0);
                if(t>0.0 && t<tMin) {
                    tMin = t; nearest = k;
                }
            }

            //set color of nearest object if exists
            if(nearest < INT_MAX) {
                color[0] = 0.0; color[1] = 0.0; color[2] = 0.0;
                tMin = objects[nearest]->intersect(ray, color, 1);
                color[0] *= 255.0; color[1] *= 255.0; color[2] *= 255.0;
                image.set_pixel(i, j, round(color[0]), round(color[1]), round(color[2]));
            }
        }
    }

    delete[] color; // save memory :P
	cout << "saving image...";
    string image_name = "E:\\CSE 410\\ray_tracing\\output_" + std::to_string(++bmp_counter) + ".bmp";
	image.save_image(image_name);
    cout << "image saved\n===================CAPTURED===========================\n\n";
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

		case '0':
			capture();
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    gluLookAt(c_pos.x,c_pos.y,c_pos.z,    c_pos.x+l.x,c_pos.y+l.y,c_pos.z+l.z,   u.x,u.y,u.z);

	glMatrixMode(GL_MODELVIEW);

	//drawing in OpenGL
	drawAxes();
    for(int i=0; i<objects.size(); i++)
        objects.at(i)->draw();

    for(int i=0; i<pointlights.size(); i++)
        pointlights.at(i)->draw();

    for(int i=0; i<spotlights.size(); i++)
        spotlights.at(i)->draw();

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

	//camera vectors init=========================
    u.x = 0; u.y = 0; u.z = 1;
    r.x = -1/sqrt(2); r.y = 1/sqrt(2); r.z = 0;
    l.x = -1/sqrt(2); l.y = -1/sqrt(2); l.z = 0;
    c_pos.x = 123.7003901; c_pos.y = 110.7472717; c_pos.z = 20;
	//=============================================

	//clear the screen
	glClearColor(0,0,0,0);

	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(viewAngle,	1,	1,	1000.0);
}


void loadData(){

    ifstream ifs("E:\\CSE 410\\ray_tracing\\scene.txt");
    if(ifs.is_open()){
        ifs >> recursion_level >> imageHeight;
        imageWidth = imageHeight;

        int n; string type;
        ifs >> n;

        //reading objects
        for(int i=0; i<n; i++){
            ifs >> type;
            if(!type.compare("sphere")) {
                Sphere *sp = new Sphere();
                sp->read_sphere(ifs);
                //sp->print();
                objects.push_back(sp);
            }
            else if(!type.compare("triangle")) {
                Triangle *sp = new Triangle();
                sp->read_triangle(ifs);
                //sp->print();
                objects.push_back(sp);
            }
            else if(!type.compare("general")) {
                General *sp = new General();
                sp->read_general(ifs);
                //sp->print();
                objects.push_back(sp);
            }
        }

        ifs >> n;
        //reading point lights
        for(int i=0; i<n; i++){
            PointLight *p = new PointLight();
            p->read_pointlight(ifs);
            //p->print();
            pointlights.push_back(p);
        }

        ifs >> n;
        //reading spot lights
        for(int i=0; i<n; i++){
            SpotLight *p = new SpotLight();
            p->read_spotlight(ifs);
            //p->print();
            spotlights.push_back(p);
        }

        Floor *f = new Floor(1000, 20);
        f->setColor(1.0, 1.0, 1.0);
        f->setCoEfficients(0.2, 0.2, 0.2, 0.2);
        f->setShine(10);
        objects.push_back(f);

    } else {
        cout << "file not found!" << endl;
        exit(0);
    }
}

// for dealloc a vector of object pointers
template<typename Iterator>
void deleter(Iterator start, Iterator last)
{
   while ( start != last ){
       delete *start;
       start++;
   }
}

//dealloc the vectors
void clear_vectors()
{
    deleter(objects.begin(), objects.end());
    objects.clear();

    deleter(pointlights.begin(), pointlights.end());
    pointlights.clear();

    deleter(spotlights.begin(), spotlights.end());
    spotlights.clear();
}

int main(int argc, char **argv){
    //input from scene.txt
    std::cout << std::setprecision(7) << std::fixed;
    loadData();

	glutInit(&argc,argv);
	glutInitWindowSize(windowHeight, windowWidth);
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

    //clear/deallocate vectors
    clear_vectors();

	return 0;
}

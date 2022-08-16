#include <bits/stdc++.h>
using namespace std;

#include "vector.hpp"



class Object{
    public:
        double height, width, length;
        double color[3];
        double coEfficients[4];
        int shine;

        Object(){};
        virtual void draw(){}
        void setShine(int);
        void setColor(double, double, double);
        void setCoEfficients(double, double, double, double);
};

void Object::setColor(double r, double g, double b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Object::setShine(int s)
{
    shine = s;
}

void Object::setCoEfficients(double amb, double diff, double spec, double refl)
{
    coEfficients[0] = amb;
    coEfficients[1] = diff;
    coEfficients[2] = spec;
    coEfficients[3] = refl;
}

class Sphere : public Object{
    public:
        Vector3D centre;
        Sphere(Vector3D, double);
        void draw();
};

Sphere::Sphere(Vector3D center, double radius){
    this->centre = center;
    length = radius;
}

void Sphere::draw()
{
    glPushMatrix(); //=========================================
    glTranslated(centre.x, centre.y, centre.z);
    Vector3D points[100][100];
    int i,j;
    int slices = 24, stacks = 30;
    double h,r;
    glColor3f(color[0], color[1], color[2]);

    //generate points
    for(i=0;i<=stacks;i++)
    {
        h=length*sin(((double)i/(double)stacks)*(pi/2));
        r=length*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0;j<slices;j++)
        {
            glBegin(GL_QUADS);{
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }glEnd();
        }
    }

    glPopMatrix(); //=====================================================
}


class Triangle : public Object{
    public:
        Vector3D points[3];
        Triangle(Vector3D *);
        void draw();
};

Triangle::Triangle(Vector3D *p){
    for(int i=0; i<3; i++){
        points[i].x = p[i].x;
        points[i].y = p[i].y;
        points[i].z = p[i].z;
    }
}

void Triangle::draw()
{
    glColor3f(color[0], color[1], color[2]);
    //generate points
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(20,0,0);
        glVertex3f(0,20,0);
        glVertex3f(0,0,0);
    }
    glEnd();

}

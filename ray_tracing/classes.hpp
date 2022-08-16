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
        void print_obj();
        void read_obj(ifstream &);
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

void Object::read_obj(ifstream &ifs)
{
    ifs >> color[0] >> color[1] >> color[2];
    ifs >> coEfficients[0] >> coEfficients[1] >> coEfficients[2] >> coEfficients[3];
    ifs >> shine;
}

void Object::print_obj()
{
    cout << "colors: " << color[0] << "," << color[1] << "," << color[2] << endl;
    cout << "coeff: " << coEfficients[0] << "," << coEfficients[1] << ",";
    cout << coEfficients[2] << "," << coEfficients[3] << endl;
    cout << "shine: " << shine << endl;
}

class Sphere : public Object{
    public:
        Vector3D centre;
        Sphere(){};
        void draw();
        void print();
        void read_sphere(ifstream &);
};

void Sphere::read_sphere(ifstream &ifs)
{
    ifs >> centre >> length;
    read_obj(ifs);
}

void Sphere::print()
{
    cout << "\nSphere=================\n";
    cout << "centre: " << centre;
    cout << "rad: " << length;
    print_obj();
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
        Triangle(){};
        void draw();
        void print();
        void read_triangle(ifstream &);
};

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

void Triangle::read_triangle(ifstream &ifs)
{
    for(int i=0; i<3; i++)
        ifs >> points[i];
    read_obj(ifs);
}


void Triangle::print()
{
    cout << "\nTriangle=================\n";
    cout << "Vertices:" << endl;
    cout << points[0] << points[1] << points[2];
    print_obj();
}

class General : public Object{
    public:
        Vector3D centre;
        double a, b, c, d, e;
        double f, g, h, i, j;
        General(){};
        void draw(){};
        void print();
        void read_general(ifstream &);
};


void General::read_general(ifstream &ifs)
{
    ifs >> a >> b >> c >> d >> e;
    ifs >> f >> g >> h >> i >> j;
    ifs >> centre;
    ifs >> length >> width >> height;
    read_obj(ifs);
}


void General::print()
{
    cout << "\nGeneral Object=================\n";
    cout << "equation: " << endl;
    cout << "a: " << a << ", b: " << b << ", c: " << c << ", d: " << d << ", e: " << e << endl;
    cout << "f: " << f << ", g: " << g << ", h: " << h << ", i: " << i << ", j: " << j << endl;
    cout << "centre: " << centre << endl;
    cout << "length: " << length << ", width: " << width << ", height: " << height << endl;
    print_obj();
}


class PointLight{
    public:
        Vector3D light_pos;
        double color[3];

        PointLight(){}
        void read_pointlight(ifstream &);
};

void PointLight::read_pointlight(ifstream &ifs)
{
    ifs >> light_pos;
    ifs >> color[0] >> color[1] >> color[2];
}

class SpotLight{
    PointLight point_light;
    Vector3D light_direction;
    double cutoff_angle;

    SpotLight(){}
    void read_spotlight(ifstream &);
};

void SpotLight::read_spotlight(ifstream &ifs)
{
    point_light.read_pointlight(ifs);

}














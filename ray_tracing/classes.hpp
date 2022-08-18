#include <bits/stdc++.h>
using namespace std;
#include "vector.hpp"


class Ray{
    public:
        Vector3D start;
        Vector3D dir;
        Ray(Vector3D, Vector3D);
};

Ray::Ray(Vector3D s, Vector3D d)
{
    start = s; dir = d;
    dir.normalize();
}


class Object{
    public:
        double height, width, length;
        double color[3];
        double coEfficients[4];
        int shine;
        int obj_type;

        Object(){ obj_type = 0; }
        virtual void draw() = 0;
        void setShine(int);
        void print_obj();
        void read_obj(ifstream &);
        void setColor(double, double, double);
        void setCoEfficients(double, double, double, double);
        virtual double intersect(Ray ray, double *col, int level){
            return -1.0;
        }
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

        Sphere(){ obj_type = 1; }
        void print();
        void read_sphere(ifstream &);

        virtual void draw()
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

        virtual double intersect(Ray ray, double *col, int level) {
            double a, b, c, tMin;
            double discrim, sol1, sol2;
            ray.start = ray.start - centre; //translate using centre

            //coefficients of quad equation
            a = 1.0; // ray->dir DOT ray->dir AKA UNIT VECTOR LEN
            b = (ray.start^ray.dir) * 2.0;
            c = (ray.start^ray.start) - (length*length);

            //solving eq...
            discrim = b*b-4*a*c;
            if(discrim < 0) tMin = -1; //no intersection
            sol1 = (-b-sqrt(discrim))/(2*a);
            sol2 = (-b+sqrt(discrim))/(2*a);
            if(sol1 > 0) tMin = sol1;
            else if(sol2 > 0) tMin = sol2;
            else tMin = -1; //both -ve sol of t //no intersection

            Vector3D intersec_point = ray.start + ray.dir * tMin;
            //get intersection color?

            //Ambient lighting
            for(int i=0; i<3; i++)
                col[i] += color[i] * coEfficients[0]; //AMBIENT


            //Other codes here//


            //keep colors within range:
            for(int i=0; i<3; i++) if(col[i] > 1.0) col[i] = 1.0;
            for(int i=0; i<3; i++) if(col[i] < ZERO) col[i] = 0.0;

            return tMin;
        }

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
    cout << "rad: " << length << endl;
    print_obj();
}

class Triangle : public Object{
    public:
        int type = 2;
        Vector3D points[3];

        Triangle(){ obj_type = 2; };
        void print();
        void read_triangle(ifstream &);
        virtual void draw()
        {
            glColor3f(color[0], color[1], color[2]);
            //generate points
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(points[0].x, points[0].y, points[0].z);
                glVertex3f(points[1].x, points[1].y, points[1].z);
                glVertex3f(points[2].x, points[2].y, points[2].z);
            }
            glEnd();

        }

        virtual double intersect(Ray r, double *col, int level)
        {
            return -1.0;
        }
};



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
        General(){ obj_type = 3; }
        virtual void draw(){ return; }
        void print();
        void read_general(ifstream &);
        virtual double intersect(Ray *r, double *col, int level)
        {
            return -1.0;
        }
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
        void print();
        void read_pointlight(ifstream &);
};

void PointLight::print()
{
    cout << "\nPoint Light==========\n";
    cout << light_pos;
    cout << "colors: " << color[0] << "," << color[1] << "," << color[2] << endl;
}

void PointLight::read_pointlight(ifstream &ifs)
{
    ifs >> light_pos;
    ifs >> color[0] >> color[1] >> color[2];
}

class SpotLight{
    public:
        PointLight point_light;
        Vector3D light_direction;
        double cutoff_angle;

        SpotLight(){}
        void print();
        void read_spotlight(ifstream &);
};


void SpotLight::print()
{
    cout << "\nSpot Light==========\n";
    cout << point_light.light_pos;
    cout << "colors: " << point_light.color[0] << "," << point_light.color[1] << "," << point_light.color[2] << endl;
    cout << light_direction;
    cout << "cutoff angle: " << cutoff_angle << endl;
}
void SpotLight::read_spotlight(ifstream &ifs)
{
    point_light.read_pointlight(ifs);
    ifs >> light_direction >> cutoff_angle;
}


class Floor : public Object{
    public:
        Floor(int floorSize, int tileSize){
            length = floorSize;
            width = tileSize;
        }

        virtual void draw(){
            // floorSize is length, tileSize is width
            int row = length/width, col = length/width;
            int quad_length = length/2;
            for(int i=0; i<row; i++) {
                for(int j=0; j<col; j++) {
                    //xy-plane so z = 0
                    Vector3D tile_origin_point(width*j - quad_length, width*i - quad_length, 0);

                    //setting the color
                    if((i+j)%2 == 0) glColor3f(0, 0, 0);
                    else glColor3f(color[0], color[1], color[2]);

                    glBegin(GL_QUADS);
                    {
                        glVertex3f(tile_origin_point.x, tile_origin_point.y, 0);
                        glVertex3f(tile_origin_point.x+width, tile_origin_point.y, 0);
                        glVertex3f(tile_origin_point.x+width, tile_origin_point.y+width, 0);
                        glVertex3f(tile_origin_point.x, tile_origin_point.y+width, 0);
                    }
                    glEnd();
                }
            }
        }
};













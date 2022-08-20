#include <bits/stdc++.h>
using namespace std;
#include "vector.hpp"
int recursion_level;

//====================LIGHTS AND RAY CLASSES START==========================================
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

class PointLight{
    public:
        Vector3D light_pos;
        double color[3];
        PointLight(){}

        virtual void print()
        {
            cout << "\nPoint Light==========\n";
            cout << light_pos;
            cout << "colors: " << color[0] << "," << color[1] << "," << color[2] << endl;
        }

        void read_pointlight(ifstream &ifs)
        {
            ifs >> light_pos;
            ifs >> color[0] >> color[1] >> color[2];
        }

        virtual void draw()
        {
            glPushMatrix(); //==================
            glTranslated(light_pos.x, light_pos.y, light_pos.z);
            Vector3D points[100][100];
            int i,j;
            int slices = 24, stacks = 30;
            double h,r,length = 1.0;
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

            glPopMatrix(); //===============
        }
};


class SpotLight: public PointLight{
    public:
        Vector3D light_direction;
        double cutoff_angle;

        SpotLight(){}
        void print() override
        {
            cout << "\nSpot Light==========\n";
            cout << light_pos;
            cout << "colors: " << color[0] << "," << color[1] << "," << color[2] << endl;
            cout << light_direction;
            cout << "cutoff angle: " << cutoff_angle << endl;
        }

        void read_spotlight(ifstream &ifs)
        {
            PointLight::read_pointlight(ifs);
            ifs >> light_direction >> cutoff_angle;
            light_direction.normalize();
        }

        void draw() override
        {
            PointLight::draw();
            Vector3D from = light_pos;
            Vector3D to = light_pos + light_direction*10.0;
            glBegin(GL_LINES);{
                glVertex3f(from.x, from.y, from.z);
                glVertex3f(to.x, to.y, to.z);
            }glEnd();
        }
};



//Global Light Data ========
vector <PointLight *> lights_list;

//====================LIGHTS AND RAY CLASSES END==========================================





//====================OBJECT CLASS START==========================================
class Object;
vector <Object *> objects;
double floor_size, tile_size;

class Object{
    public:
        double height, width, length;
        double color[3];
        double coEfficients[4];
        int shine;
        int obj_type;

        Object(){ obj_type = 0; }
        virtual void draw() = 0;
        virtual Ray get_normal(Vector3D intersec_point, Ray incident_ray) = 0;
        void setShine(int);
        void print_obj();

        void read_obj(ifstream &);
        void setColor(double, double, double);
        void setCoEfficients(double, double, double, double);
        virtual double intersect(Ray ray, double *col, int level){
            return -1.0;
        }


        // GENERIC PHONG LIGHTING FUNCTION FOR ALL OBJECTS =========================================
        void illuminate(Ray ray, double *col, double tMin, bool isFloor=false)
        {
            Vector3D intersec_point = ray.start + ray.dir * tMin;
            double floor_col = 0;
            int tile_row, tile_col;
            if(isFloor)
            {
                tile_row = (intersec_point.x + floor_size/2.0) / tile_size;
                tile_col = (intersec_point.y + floor_size/2.0) / tile_size;
                if((tile_col+tile_row)%2 == 1) floor_col = 1; //white if odd
                else return; //LIGHTING NOT DONE FOR DARK TILES!!! ==============================
                //Ambient Lighting for Floor
                for(int i=0; i<3; i++)
                    col[i] += floor_col * coEfficients[0]; //AMBIENT
            } else {
                //Ambient Lighting for others
                for(int i=0; i<3; i++)
                    col[i] += color[i] * coEfficients[0]; //AMBIENT
            }

            //Specular and Diffuse Lighting
            for (int i=0; i<lights_list.size(); i++)
            {
                bool light_obstructed = false;
                PointLight *light = lights_list.at(i);
                Vector3D incident_vect = intersec_point - light->light_pos;

                //if spotlight, check if object is within cutoff angle
                if(SpotLight* spotlight = dynamic_cast<SpotLight*>(light)){
                    double angle = (incident_vect^spotlight->light_direction);
                    angle /= (incident_vect.get_length()*spotlight->light_direction.get_length());
                    angle = acos(angle);
                    if(angle > rad(spotlight->cutoff_angle)) continue;
                }

                double light_distance = incident_vect.get_length();
                if (light_distance < ZERO) //light source inside object
                    continue;

                //Ray L_ray = Ray of Incidence | N_ray = Ray of Normal | R_ray = Ray of Reflection
                Ray L_ray = Ray(light->light_pos, incident_vect);
                Ray N_ray = get_normal(intersec_point, L_ray);
                Vector3D R_dir = L_ray.dir - N_ray.dir * ((L_ray.dir^N_ray.dir)*2); //R = L - 2(L.N)N
                Ray R_ray = Ray(intersec_point, R_dir);

                //SHADOWS: is the incident ray being blocked by an object?
                for(int k=0; k<objects.size(); k++){
                    double t = objects.at(k)->intersect(L_ray, col, 0);
                    if (t > 0 && t + ZERO < light_distance){
                        light_obstructed = true;
                        break;
                    }
                }

                if(light_obstructed) continue; //light
                for (int i = 0; i < 3; i++) {
                    if(isFloor){
                        //diffuse lighting component
                        col[i] += light->color[i] * coEfficients[1] * max((-L_ray.dir)^N_ray.dir, 0.0) * floor_col;
                        //specular lighting component
                        col[i] += light->color[i] * coEfficients[2] * pow(max((-ray.dir)^R_ray.dir, 0.0), shine);
                    }else {
                        //diffuse lighting component
                        col[i] += light->color[i] * coEfficients[1] * max((-L_ray.dir)^N_ray.dir, 0.0) * color[i];
                        //specular lighting component
                        col[i] += light->color[i] * coEfficients[2] * pow(max((-ray.dir)^R_ray.dir, 0.0), shine);
                    }
                }


            }// end of diffuse, specular loop ========================

            //keep colors within range:
            for(int i=0; i<3; i++) if(col[i] > 1.0) col[i] = 1.0;
            for(int i=0; i<3; i++) if(col[i] < ZERO) col[i] = 0.0;

        }// end of function ================================================================




        // GENERIC RECURSIVE REFLECTION FUNCTION FOR ALL OBJECTS =========================================
        void recursive_reflection(Ray ray, double *col, int level, double tMin)
        {
            //the last bounce of light is done
            if(level >= recursion_level) return;

            //Ray L_ray = ray; given as parameter | N_ray = Ray of Normal | R_ray = Ray of Reflection
            Vector3D intersec_point = ray.start + ray.dir * tMin;
            Ray N_ray = get_normal(intersec_point, ray);
            Vector3D R_dir = ray.dir - N_ray.dir * ((ray.dir^N_ray.dir)*2); //R = L - 2(L.N)N
            Ray R_ray = Ray(intersec_point, R_dir);
            R_ray.start = R_ray.start + R_ray.dir * ZERO;

            double nearest = INT_MAX;
            double tmin_temp = INT_MAX;
            for(int k=0; k<objects.size(); k++) {
                double t = objects.at(k)->intersect(R_ray, col, 0);
                if(t>0.0 && t<tmin_temp) {
                    tmin_temp = t; nearest = k;
                }
            }

            if (nearest == INT_MAX) return; //no object reflects the light :(

            //empty color array to find reflected color
            double *color_temp = new double[3];
            for (int i = 0; i < 3; i++) color_temp[i] = 0;

            //sets the reflected color
            tmin_temp = objects[nearest]->intersect(R_ray, color_temp, level + 1);
            for (int i = 0; i < 3; i++) col[i] += color_temp[i] * coEfficients[3];

            //always clear memory :-D
            delete[] color_temp;
        }

};

// ============================================



//Global camera data =========
Vector3D c_pos, u, l, r;
// ================================


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
//====================OBJECT CLASS END==========================================




//====================SPHERE CLASS START==========================================
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

        double get_tMin(Ray ray)
        {
            ray.start = ray.start - centre; //translate using centre
            double td = -ray.start^ray.dir; //sign of cosine of Ro and Rd
            if(td < 0.0) return -1.0;

            double d_squared = (ray.start^ray.start) - td*td;
            if(d_squared > length*length) return -1.0; //length is radius

            double offset = sqrt(length*length - d_squared);
            double t = td - offset;
            if(t > 0.0) return t;

            t = td + offset;
            if(t > 0.0) return t;

            return -1.0;
        }

        virtual Ray get_normal(Vector3D intersec_point, Ray incident_ray)
        {
            return Ray(intersec_point, intersec_point - centre);
        }

        virtual double intersect(Ray ray, double *col, int level) {
            double tMin = get_tMin(ray);
            if(level == 0) return tMin;

            //illumination function same for all
            illuminate(ray, col, tMin);

            //recursive reflection of light
            recursive_reflection(ray, col, level, tMin);

            //return min val of intersection equation parameter: t
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
//====================SPHERE CLASS END==========================================




//=======================TRIANGLE CLASS START==========================================
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

        virtual Ray get_normal(Vector3D intersec_point, Ray incident_ray)
        {
            Vector3D normal = (points[1] - points[0]).cross(points[2] - points[0]);
            if ((-incident_ray.dir^normal) >= 0) //cosine is +ve
                return Ray(intersec_point, normal);
            else
                return Ray(intersec_point, -normal);
        }

        double get_tMin(Ray ray)
        {
            Vector3D A_B = points[1] - points[0];
            Vector3D C_B = points[2] - points[0];
            Vector3D D = -ray.dir;
            //if(c_pos.z<0.0) D=-D;

            Vector3D solutions = solve_tri_linears(A_B.x, C_B.x, D.x, ray.start.x-points[0].x,
                                                   A_B.y, C_B.y, D.y, ray.start.y-points[0].y,
                                                   A_B.z, C_B.z, D.z, ray.start.z-points[0].z);
            if(solutions.x + solutions.y > 1) return -1.0;
            if(solutions.x < 0) return -1.0;
            if(solutions.y < 0) return -1.0;
            //cout << "tmin is: " << solutions.z << endl;
            return solutions.z;
        }

        virtual double intersect(Ray ray, double *col, int level) {
            double tMin = get_tMin(ray);
            if(level == 0) return tMin;

            //illumination function same for all
            illuminate(ray, col, tMin);

            //recursive reflection of light
            recursive_reflection(ray, col, level, tMin);

            //return min val of intersection equation parameter: t
            return tMin;
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
//=======================TRIANGLE CLASS END==========================================





//=======================GENERAL QUADRIC SURFACE START==========================================
class General : public Object{
    public:
        Vector3D centre;
        double a, b, c, d, e;
        double f, g, h, i, j;
        General(){ obj_type = 3; }
        virtual void draw(){ return; }
        void print();
        void read_general(ifstream &);

        virtual Ray get_normal(Vector3D intersec_point, Ray incident_ray)
        {
            double x = intersec_point.x, y = intersec_point.y, z = intersec_point.z;
            double x_ = 2*a*x + d*y + e*z + g;
            double y_ = 2*b*y + d*x + f*z + h;
            double z_ = 2*c*z + e*x + f*y + i;
            return Ray(intersec_point, Vector3D(x_, y_, z_)); //point, dir
        }

        bool falls_inside_reference_cube(Vector3D point)
        {
            double dimensions[] = {length, width, height};
            double point_arr[] = {point.x, point.y, point.z};
            double ref_point[] = {centre.x, centre.y, centre.z};
            for(int i = 0; i < 3; i++)
            {
                if(fabs(dimensions[i]) < ZERO) continue; //no clipping required if dim == 0
                if(point_arr[i] < ref_point[i]) return false;
                if(point_arr[i] > ref_point[i]+dimensions[i]) return false;
            }
            return true;
        }

        double get_tMin(Ray ray)
        {
            //for simple code :)
            double rox = ray.start.x;
            double roy = ray.start.y;
            double roz = ray.start.z;
            double rdx = ray.dir.x;
            double rdy = ray.dir.y;
            double rdz = ray.dir.z;

            //so much work for solving a quadratic equation :)
            double coeff_A = a*rdx*rdx + b*rdy*rdy + c*rdz*rdz;
            coeff_A += d*rdx*rdy + e*rdx*rdz + f*rdy*rdz;

            double coeff_B = 2*a*rox*rdx + 2*b*roy*rdy + 2*c*roz*rdz;
            coeff_B += d*rox*rdy + rdx*roy*d + e*rox*rdz + e*rdx*roz;
            coeff_B += f*roy*rdz + f*rdy*roz + g*rdx + h*rdy + i*rdz;

            double coeff_C = a*rox*rox + b*roy*roy + c*roz*roz + d*rox*roy;
            coeff_C += e*rox*roz + f*roy*roz + g*rox + h*roy + i*roz + j;

            Vector3D solutions = solve_quad(coeff_A, coeff_B, coeff_C);
            if(solutions.z < 0) return -1.0; // no solution
            //roots of equation
            double t1 = solutions.x, t2 = solutions.y;

            //take smallest +ve first aka t1
            Vector3D intersec_point;
            if(t1 > 0){
                intersec_point = ray.start + ray.dir * t1;
                if(falls_inside_reference_cube(intersec_point)) return t1;
            }
            if(t2 > 0){
                intersec_point = ray.start + ray.dir * t2;
                if(falls_inside_reference_cube(intersec_point)) return t2;
            }
            return -1.0;
        }

        virtual double intersect(Ray ray, double *col, int level) {
            double tMin = get_tMin(ray);
            if(level == 0) return tMin;

            //illumination function same for all
            illuminate(ray, col, tMin);

            //recursive reflection of light
            recursive_reflection(ray, col, level, tMin);

            //return min val of intersection equation parameter: t
            return tMin;
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
//=======================GENERAL QUADRIC SURFACE END==========================================





//=======================FLOOR CLASS START==========================================
class Floor : public Object{
    public:
        Floor(int floorSize, int tileSize){
            length = floorSize;
            width = tileSize;
            //global
            floor_size = floorSize;
            tile_size = tileSize;
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

        virtual Ray get_normal(Vector3D intersec_point, Ray incident_ray)
        {
            Vector3D normal(0, 0, 1);
            if (incident_ray.start.z > 0) return Ray(intersec_point, normal);
            return Ray(intersec_point, -normal);
        }

        double get_tMin(Ray ray)
        {
            Vector3D n(0, 0, 1);
            //if cosine -ve, take -ve z-axis aka eye below floor
            if(c_pos.z < 0.0) n = -n;

            double denom = n^ray.dir;
            if(denom == 0.0) return -1.0;

            //Po is orgin so ZERO!!
            double tMin = (-n^ray.start)/(denom);
            Vector3D intersec_point = ray.start + ray.dir * tMin;
            if (fabs(intersec_point.y) > length/2.0) return -1.0;   //length is floorSize :-D
            if (fabs(intersec_point.x) > length/2.0) return -1.0;
            return tMin;
        }


        virtual double intersect(Ray ray, double *col, int level) {
            double tMin = get_tMin(ray);
            if(level == 0) return tMin;

            //illumination function same for all
            illuminate(ray, col, tMin, true);

            //recursive reflection of light
            recursive_reflection(ray, col, level, tMin);

            //return min val of intersection equation parameter: t
            return tMin;
        }

};
//=======================FLOOR CLASS END==========================================











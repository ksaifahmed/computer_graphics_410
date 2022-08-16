#include <bits/stdc++.h>
using namespace std;
#define pi (2*acos(0.0))

class Vector3D{
    public:
    double x, y, z;
    Vector3D() {};
    Vector3D(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    friend ostream &operator<<(ostream &out, const Vector3D &vect)
    {
        out << "point is: (";
        out << vect.x << "," << vect.y << "," << vect.z;
        out << ")" << endl;
        return out;
    }

    friend istream &operator>>(istream &ins, Vector3D &vect)
    {
        ins >> vect.x >> vect.y >> vect.z;
        return ins;
    }
};




Vector3D get_Vector3D(double x, double y, double z)
{
    Vector3D p1;
    p1.x = x;
    p1.y = y;
    p1.z = z;
    return p1;
}

Vector3D vect_add(Vector3D p1, Vector3D p2)
{
    p1.x = p1.x + p2.x;
    p1.y = p1.y + p2.y;
    p1.z = p1.z + p2.z;
    return p1;
}

Vector3D vect_scale(Vector3D p1, double val)
{
    p1.x = p1.x * val;
    p1.y = p1.y * val;
    p1.z = p1.z * val;
    return p1;
}

Vector3D unit_vector(Vector3D p1)
{
    double mod = 1/sqrt(p1.x*p1.x + p1.y*p1.y + p1.z*p1.z);
    return vect_scale(p1, mod);
}

Vector3D move_along_vect(Vector3D p, Vector3D vect, double units)
{
    Vector3D dir = vect_add(vect, vect_scale(p, -1)); //direction of move
    dir = vect_scale(unit_vector(dir), units); //units of movement
    p = vect_add(p, dir);
    return p;
}

Vector3D move_along_unit_vect(Vector3D p, Vector3D uvect, double units)
{
    Vector3D dir = vect_scale(uvect, units); //units of movement
    p = vect_add(p, dir);
    return p;
}

Vector3D cross_product(Vector3D a, Vector3D b)
{
    Vector3D p;
    p.x = a.y*b.z - a.z*b.y;
    p.y = a.z*b.x - a.x*b.z;
    p.z = a.x*b.y - a.y*b.x;
    return p;
}


void axis_rotation(Vector3D *l, Vector3D *u, Vector3D *r, double degrees)
{
    double rad = degrees * pi / 180.0;
    *l = vect_add(vect_scale(*l, cos(rad)), vect_scale(*u, sin(rad)));
    *u = cross_product(*r, *l);
}

Vector3D rotate_about_Vector3D(Vector3D d, Vector3D o, double degrees)
{
    double rad = degrees * pi / 180.0;
    d.x -= o.x;
    d.y -= o.y;

    double temp = d.x*cos(rad) - d.y*sin(rad);
    d.y = d.x*sin(rad) + d.y*cos(rad);
    d.x = temp;

    d.x += o.x;
    d.y += o.y;
    return d;
}


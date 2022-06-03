#include <stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define pi (2*acos(0.0))

struct point
{
	double x,y,z;
};


point vect_add(point p1, point p2)
{
    p1.x = p1.x + p2.x;
    p1.y = p1.y + p2.y;
    p1.z = p1.z + p2.z;
    return p1;
}

point vect_scale(point p1, double val)
{
    p1.x = p1.x * val;
    p1.y = p1.y * val;
    p1.z = p1.z * val;
    return p1;
}

point unit_vector(point p1)
{
    double mod = 1/sqrt(p1.x*p1.x + p1.y*p1.y + p1.z*p1.z);
    return vect_scale(p1, mod);
}

point move_along_vect(point p, point vect, double units)
{
    point dir = vect_add(vect, vect_scale(p, -1)); //direction of move
    dir = vect_scale(unit_vector(dir), units); //units of movement
    p = vect_add(p, dir);
    return p;
}

point move_along_unit_vect(point p, point uvect, double units)
{
    point dir = vect_scale(uvect, units); //units of movement
    p = vect_add(p, dir);
    return p;
}

point cross_product(point a, point b)
{
    point p;
    p.x = a.y*b.z - a.z*b.y;
    p.y = a.z*b.x - a.x*b.z;
    p.z = a.x*b.y - a.y*b.x;
    return p;
}


void axis_rotation(point *l, point *u, point *r, double degrees)
{
    double rad = degrees * pi / 180.0;
    *l = vect_add(vect_scale(*l, cos(rad)), vect_scale(*u, sin(rad)));
    *u = cross_product(*r, *l);
}

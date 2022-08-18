#include <bits/stdc++.h>
using namespace std;
#define INF 1e10
#define ZERO 1e-8
#define pi (2*acos(0.0))

// vector class ==============================================
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

    //add vectors
    Vector3D operator+(const Vector3D &vect) const
    {
        double x_ = this->x + vect.x;
        double y_ = this->y + vect.y;
        double z_ = this->z + vect.z;
        return Vector3D(x_, y_, z_);
    }

    //add constant
    Vector3D operator+(double val) const
    {
        double x_ = this->x + val;
        double y_ = this->y + val;
        double z_ = this->z + val;
        return Vector3D(x_, y_, z_);
    }

    //sub vectors
    Vector3D operator-(const Vector3D &vect) const
    {
        double x_ = this->x - vect.x;
        double y_ = this->y - vect.y;
        double z_ = this->z - vect.z;
        return Vector3D(x_, y_, z_);
    }

    //sub constant
    Vector3D operator-(double val) const
    {
        double x_ = this->x - val;
        double y_ = this->y - val;
        double z_ = this->z - val;
        return Vector3D(x_, y_, z_);
    }

    //negate vectors
    Vector3D operator-() const
    {
        double x_ = -this->x;
        double y_ = -this->y;
        double z_ = -this->z;
        return Vector3D(x_, y_, z_);
    }

    //mul constant
    Vector3D operator*(double val) const
    {
        double x_ = this->x * val;
        double y_ = this->y * val;
        double z_ = this->z * val;
        return Vector3D(x_, y_, z_);
    }

    //divide constant
    Vector3D operator/(double val) const
    {
        double x_ = this->x / val;
        double y_ = this->y / val;
        double z_ = this->z / val;
        return Vector3D(x_, y_, z_);
    }

    //dot product
    double operator^(const Vector3D &vect) const
    {
        double x_ = this->x * vect.x;
        x_ += this->y * vect.y;
        x_ += this->z * vect.z;
        return x_;
    }

    //cross product
    Vector3D cross(Vector3D vect)
    {
        double x_  = this->y * vect.z - this->z * vect.y;
        double y_  = this->z * vect.x - this->x * vect.z;
        double z_  = this->x * vect.y - this->y * vect.x;
        return Vector3D(x_, y_, z_);
    }

    //vector length
    double get_length()
    {
        double x_ = this->x * this->x;
        double y_ = this->y * this->y;
        double z_ = this->z * this->z;
        return sqrt(x_+y_+z_);
    }

    //normalize a vector
    void normalize()
    {
        double val = get_length();
        if(val < ZERO) {
            cout << "vector len too small!\n";
            return;
        }
        this->x = this->x / val;
        this->y = this->y / val;
        this->z = this->z / val;
    }

};


//utils from first offline =====================
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

pair<double, double> solve_bi_linear(double a, double b, double e, double c, double d, double f)
{
    //we solve the linear system using CRAMER'S RULE
    //ax+by=e
    //cx+dy=f
    double determinant = a*d - b*c;
    if(determinant != 0) {
        double x = (e*d - b*f)/determinant;
        double y = (a*f - e*c)/determinant;
        return make_pair(x, y);
    } else {
        return make_pair(10, 10);
    }
}

double calc_determinant(double mat[3][3])
{
    double ans;
    ans = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2])
          - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
          + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    return ans;
}

// linear equations using cramer's rule
Vector3D solve_tri_linears(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l)
{
    double coeff[3][4] = {
        { a, b, c, d },
        { e, f, g, h },
        { i, j, k, l },
    };

    double det[3][3] = {
        { coeff[0][0], coeff[0][1], coeff[0][2] },
        { coeff[1][0], coeff[1][1], coeff[1][2] },
        { coeff[2][0], coeff[2][1], coeff[2][2] },
    };
    double d1[3][3] = {
        { coeff[0][3], coeff[0][1], coeff[0][2] },
        { coeff[1][3], coeff[1][1], coeff[1][2] },
        { coeff[2][3], coeff[2][1], coeff[2][2] },
    };
    double d2[3][3] = {
        { coeff[0][0], coeff[0][3], coeff[0][2] },
        { coeff[1][0], coeff[1][3], coeff[1][2] },
        { coeff[2][0], coeff[2][3], coeff[2][2] },
    };
    double d3[3][3] = {
        { coeff[0][0], coeff[0][1], coeff[0][3] },
        { coeff[1][0], coeff[1][1], coeff[1][3] },
        { coeff[2][0], coeff[2][1], coeff[2][3] },
    };

    double D = calc_determinant(det);
    double D1 = calc_determinant(d1);
    double D2 = calc_determinant(d2);
    double D3 = calc_determinant(d3);

    // if sol exists
    if (D != 0) {
        return Vector3D(D1/D, D2/D, D3/D);
    }

    //return invalid k1, k2, tmin
    return Vector3D(10, 10, -1.0);
}

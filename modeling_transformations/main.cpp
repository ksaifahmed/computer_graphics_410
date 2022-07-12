#include <bits/stdc++.h>
using namespace std;

struct Point
{
	double x,y,z;
};


//================== MATRIX UTILS ===========================
double **getMat(int row, int col)
{
    double **mat = new double*[row];
    for(int i=0; i<row; i++)
        mat[i] = new double[col];

    for(int i=0; i<row; i++)
        for(int j=0; j<col; j++)
            mat[i][j] = 0.0;

    return mat;
}

double **mat_mul(double **matA, double **matB, int a_rows, int b_col, int b_rows)
{
    double **ans = getMat(a_rows, b_col);
    for(int i=0; i<a_rows; i++)
        for(int j=0; j<b_col; j++)
            for(int k=0; k<b_rows; k++)
                ans[i][j] += matA[i][k] * matB[k][j];

    return ans;
}

double **point_to_mat(Point p1, Point p2, Point p3, int n, int m)
{
    double **ans = getMat(n, m);
    ans[0][0] = p1.x;
    ans[0][1] = p2.x;
    ans[0][2] = p3.x;

    ans[1][0] = p1.y;
    ans[1][1] = p2.y;
    ans[1][2] = p3.y;

    ans[2][0] = p1.z;
    ans[2][1] = p2.z;
    ans[2][2] = p3.z;
    return ans;
}


//================== POINT STRUCT UTILS ===========================
Point get_point(double x, double y, double z)
{
    Point p1;
    p1.x = x;
    p1.y = y;
    p1.z = z;
    return p1;
}

Point vect_add(Point p1, Point p2)
{
    p1.x = p1.x + p2.x;
    p1.y = p1.y + p2.y;
    p1.z = p1.z + p2.z;
    return p1;
}

Point vect_add(Point p1, Point p2, Point p3)
{
    p1.x = p1.x + p2.x + p3.x;
    p1.y = p1.y + p2.y + p3.y;
    p1.z = p1.z + p2.z + p3.z;
    return p1;
}

Point vect_scale(Point p1, double val)
{
    p1.x = p1.x * val;
    p1.y = p1.y * val;
    p1.z = p1.z * val;
    return p1;
}

Point unit_vector(Point p1)
{
    double mod = 1/sqrt(p1.x*p1.x + p1.y*p1.y + p1.z*p1.z);
    return vect_scale(p1, mod);
}

double dot_product(Point a, Point b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}


Point cross_product(Point a, Point b)
{
    Point p;
    p.x = a.y*b.z - a.z*b.y;
    p.y = a.z*b.x - a.x*b.z;
    p.z = a.x*b.y - a.y*b.x;
    return p;
}


//================== TRIANGLE CLASS ===========================
class Triangle
{
    double **coord_arr;
    public:
        Triangle();
        ~Triangle();
        Triangle(Point, Point, Point);
        void free_mem();
        void print_triangle();
        void set_coord(double **);
        double **get_coord() { return coord_arr; };
};

void Triangle::free_mem()
{
    for(int i=0; i<4; i++)
        delete[] coord_arr[i];
    delete[] coord_arr;
}

Triangle::~Triangle()
{
    free_mem();
}

Triangle::Triangle()
{
    coord_arr = getMat(4, 3);
}

Triangle::Triangle(Point p1, Point p2, Point p3)
{
    coord_arr = point_to_mat(p1, p2, p3, 4, 3);
    for(int i=0; i<3; i++)
        coord_arr[3][i] = 1.0;
}

void Triangle::set_coord(double **coord)
{
    free_mem();
    coord_arr = coord;
}

void Triangle::print_triangle()
{
    for(int i=0; i<4; i++) {
        for(int j=0; j<3; j++)
            cout << coord_arr[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}


//================== LINEAR TRANSFORM UTILS ===========================
double **get_translate_mat(double tx, double ty, double tz)
{
    double **trans_mat = getMat(4, 4);
    trans_mat[0][3] = tx;
    trans_mat[1][3] = ty;
    trans_mat[2][3] = tz;
    for(int i=0; i<4; i++)
        trans_mat[i][i] = 1.0;

    return trans_mat;
}

double **get_scale_mat(double sx, double sy, double sz)
{
    double **trans_mat = getMat(4, 4);
    trans_mat[0][0] = sx;
    trans_mat[1][1] = sy;
    trans_mat[2][2] = sz;
    trans_mat[3][3] = 1.0;

    return trans_mat;
    //triangle->set_coord(mat_mul(trans_mat, triangle->get_coord(), 4, 3, 4));
}

double **get_rotate_mat(Triangle *triangle, double angle, Point axis)
{
    Point i = get_point(1, 0, 0);
    Point j = get_point(0, 1, 0);
    Point k = get_point(0, 0, 1);

    double c = cos(angle), s = sin(angle);
    Point col1 = vect_add(
        vect_scale(i, c),
        vect_scale(axis, (1-c)*dot_product(axis, i)),
        vect_scale(cross_product(axis, i), s)
    );
    Point col2 = vect_add(
        vect_scale(j, c),
        vect_scale(axis, (1-c)*dot_product(axis, j)),
        vect_scale(cross_product(axis, j), s)
    );
    Point col3 = vect_add(
        vect_scale(k, c),
        vect_scale(axis, (1-c)*dot_product(axis, k)),
        vect_scale(cross_product(axis, k), s)
    );

    double **trans_mat = point_to_mat(col1, col2, col3, 4, 4);
    trans_mat[3][3] = 1.0;
    return trans_mat;
}


int main()
{
    Point p1 = get_point(1, 2, 3);
    Point p2 = get_point(5, 6, 7);
    Point p3 = get_point(8, 9, 3);

    Triangle triangle(p1, p2, p3);
    triangle.print_triangle();


    return 0;
}

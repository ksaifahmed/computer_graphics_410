#include <bits/stdc++.h>
using namespace std;

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

double **mat_mul(double **trian, double **trans)
{
    double **ans = getMat(4, 3);
    for(int i=0; i<4; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<4; k++)
                ans[i][j] += trans[i][k] * trian[k][j];

    //freeing mem
    for(int i=0; i<4; i++){
        delete[] trian[i];
    }
    delete[] trian;

    return ans;
}


class Point
{
    double x, y, z;
    public:
        Point();
        Point(double, double, double);
        double getX(){ return x; }
        double getY() { return y; }
        double getZ() { return z; }
};

Point::Point(){}
Point::Point(double x, double y, double z)
{
    this -> x = x;
    this -> y = y;
    this -> z = z;
}

class Triangle
{
    double **coord_arr;
    public:
        Triangle();
        Triangle(Point, Point, Point);
        void print_triangle();
        void translate(double, double, double);
        void scale(double, double, double);
};


Triangle::Triangle()
{
    coord_arr = getMat(4, 3);
}
Triangle::Triangle(Point p1, Point p2, Point p3)
{
    coord_arr = getMat(4, 3);

    coord_arr[0][0] = p1.getX();
    coord_arr[0][1] = p2.getX();
    coord_arr[0][2] = p3.getX();

    coord_arr[1][0] = p1.getY();
    coord_arr[1][1] = p2.getY();
    coord_arr[1][2] = p3.getY();

    coord_arr[2][0] = p1.getZ();
    coord_arr[2][1] = p2.getZ();
    coord_arr[2][2] = p3.getZ();

    for(int i=0; i<3; i++)
        coord_arr[3][i] = 1.0;
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

void Triangle::translate(double tx, double ty, double tz)
{
    double **trans_mat = getMat(4, 4);
    trans_mat[0][3] = tx;
    trans_mat[1][3] = ty;
    trans_mat[2][3] = tz;
    for(int i=0; i<4; i++)
        trans_mat[i][i] = 1.0;

    coord_arr = mat_mul(coord_arr, trans_mat);
}

void Triangle::scale(double sx, double sy, double sz)
{
    double **trans_mat = getMat(4, 4);
    trans_mat[0][0] = sx;
    trans_mat[1][1] = sy;
    trans_mat[2][2] = sz;
    trans_mat[3][3] = 1.0;
    coord_arr = mat_mul(coord_arr, trans_mat);
}


int main()
{
    Point p1(1, 2, 3);
    Point p2(5, 6, 7);
    Point p3(8, 9, 3);

    Triangle triangle(p1, p2, p3);
    triangle.print_triangle();

    triangle.translate(2, 6, 7);
    triangle.print_triangle();

    triangle.scale(2, 3, 5);
    triangle.print_triangle();

    return 0;
}
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


Point normalize(Point a)
{
    double div = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    a.x /= div;
    a.y /= div;
    a.z /= div;
    return a;
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
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++)
            cout << coord_arr[j][i] << " ";
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
}

double **get_rotate_mat(double angle, double x, double y, double z)
{
    Point axis = normalize(get_point(x, y, z));
    Point i = get_point(1, 0, 0);
    Point j = get_point(0, 1, 0);
    Point k = get_point(0, 0, 1);

    angle = (acos(0)/90.0) * angle;
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



//==================TRANSFORM STACK UTILS ===========================
class TStack
{
    int size;
    bool push_init;
    vector<double **> trans_stack;
    public:
        TStack();
        void pop();
        double **top();
        void push(double **);
        void enable_push(){ push_init = true; }

};

TStack::TStack()
{
    size = 1; push_init = false;
    double **id_mat = getMat(4, 4);
    for(int i=0; i<4; i++) id_mat[i][i] = 1.0;
    trans_stack.push_back(id_mat);
}

void TStack::push(double **trans_mat)
{
    if(size > 0 && push_init){
        trans_stack.push_back(trans_mat);
        size++;
    }
}

void TStack::pop()
{
    if(size > 1){
        trans_stack.pop_back();
        size--;
    }
    if(size == 1) push_init = false;
}

double **TStack::top()
{
    return trans_stack.back();
}


//==================VIEW AND PROJECTION UTILS===========================
double **get_view_rot(Point eye, Point look, Point up)
{
    Point l, u, r;
    l = vect_add(look, vect_scale(eye, -1));
    l = normalize(l);
    r = cross_product(l, up);
    r = normalize(r);
    u = cross_product(r, l);

    l = vect_scale(l, -1);
    double **temp = point_to_mat(r, u, l, 4, 4);
    double **R = getMat(4, 4);
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            R[j][i] = temp[i][j];
    return R;
}

double **get_view_trans(Point eye)
{
    double **T = get_translate_mat(-eye.x, -eye.y, -eye.z);
    return T;
}

double **get_projection_mat(double fovY, double aspectRatio, double near, double far)
{
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY/2);
    double r = near * tan(fovX/2);

    double **ProjectionMat = getMat(4, 4);
    ProjectionMat[0][0] = near/r;
    ProjectionMat[1][1] = near/t;
    ProjectionMat[2][2] = -(far+near)/(far-near);
    ProjectionMat[2][3] = -(2*far*near)/(far-near);
    ProjectionMat[3][2] = -1;

    return ProjectionMat;
}


int main()
{
    std::cout << std::fixed;
    std::cout << std::setprecision(7);

    string comm;
    TStack tstack;
    vector<TStack> state;
    freopen("scene.txt", "r", stdin);

    Point look, eye, up;
    double fovY, aspectRatio, near, far;

    cin >> eye.x >> eye.y >> eye.z;
    cin >> look.x >> look.y >> look.z;
    cin >> up.x >> up.y >> up.z;
    cin >> fovY >> aspectRatio >> near >> far;

    double **R = get_view_rot(eye, look, up);
    double **T = get_view_trans(eye);
    double **ViewMat = mat_mul(R, T, 4, 4, 4);
    double **ProjectionMat = get_projection_mat(fovY, aspectRatio, near, far);

    while(true)
    {
        cin >> comm;
        if(!comm.compare("triangle"))
        {
            Point p1, p2, p3;
            cin >> p1.x >> p1.y >> p1.z;
            cin >> p2.x >> p2.y >> p2.z;
            cin >> p3.x >> p3.y >> p3.z;
            Triangle triangle(p1, p2, p3);

            double **trans_mat = tstack.top();
            triangle.set_coord(
                mat_mul(trans_mat, triangle.get_coord(), 4, 3, 4)
            );
            triangle.print_triangle();
        }

        else if(!comm.compare("translate"))
        {
            tstack.enable_push();
            double x, y, z;
            cin >> x >> y >> z;
            double **trans_mat = get_translate_mat(x, y, z);
            tstack.push(
                mat_mul(tstack.top(), trans_mat, 4, 4, 4)
            );
        }

        else if(!comm.compare("scale"))
        {
            tstack.enable_push();
            double x, y, z;
            cin >> x >> y >> z;
            double **trans_mat = get_scale_mat(x, y, z);
            tstack.push(
                mat_mul(tstack.top(), trans_mat, 4, 4, 4)
            );
        }

        else if(!comm.compare("rotate"))
        {
            tstack.enable_push();
            double deg, x, y, z;
            cin >> deg >> x >> y >> z;
            double **trans_mat = get_rotate_mat(deg, x, y, z);
            tstack.push(
                mat_mul(tstack.top(), trans_mat, 4, 4, 4)
            );
        }

        else if(!comm.compare("push"))
        {
            state.push_back(tstack);
        }

        else if(!comm.compare("pop"))
        {
            if(!state.empty())
            {
                tstack = state.back();
                state.pop_back();
            }
        }

        else if(!comm.compare("end"))
        {
            break;
        }

    }

    fclose(stdin);
    return 0;
}

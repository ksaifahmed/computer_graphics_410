#include <bits/stdc++.h>
#include "bitmap_image.hpp"
#define INF 1e8
#define ZERO 1e-8
using namespace std;


struct Point
{
    double x,y,z;
};


struct Color
{
    int r, g, b;
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

Point get_intersection(Point L1, Point L2, double y)
{
    Point p = get_point(INF, INF, INF);
    if(fabs(L1.y - L2.y) < ZERO) return p;
    if(L1.y < y && L2.y < y) return p;
    if(L1.y > y && L2.y > y) return p;

    p.y = y;
    p.x = L1.x + ((L1.x - L2.x)/(L1.y - L2.y)) * (y - L1.y);
    p.z = L1.z + ((L1.z - L2.z)/(L1.y - L2.y)) * (y - L1.y);

    return p;
}

//================== TRIANGLE CLASS ===========================
class Triangle
{
    double **coord_arr;
public:
    Color color;
    Triangle();
    ~Triangle();
    Triangle(Point, Point, Point);
    void free_mem();
    void set_color();
    double get_maxY();
    double get_minY();
    void print_triangle();
    void print_triangle(ofstream *);
    void set_coord(double **, bool);
    double **get_coord()
    {
        return coord_arr;
    };
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

void Triangle::set_coord(double **coord, bool normalize=false)
{
    free_mem();
    coord_arr = coord;

    if(normalize)
    {
        for(int i=0; i<3; i++)
        {
            if(coord_arr[3][i])
            {
                coord_arr[0][i] /= coord_arr[3][i];
                coord_arr[1][i] /= coord_arr[3][i];
                coord_arr[2][i] /= coord_arr[3][i];
                coord_arr[3][i] /= coord_arr[3][i];
            }
        }
    }
}

void Triangle::print_triangle()
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
            cout << coord_arr[j][i] << " ";
        cout << endl;
    }
    cout << endl;
}

void Triangle::print_triangle(ofstream *ofs)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
            *ofs << coord_arr[j][i] << " ";
        *ofs << endl;
    }
    *ofs << endl;
}

void Triangle::set_color()
{
    color.r = rand() % 256;
    color.b = rand() % 256;
    color.g = rand() % 256;
}

double Triangle::get_maxY()
{
    return max({coord_arr[1][0], coord_arr[1][1], coord_arr[1][2]});
}

double Triangle::get_minY()
{
    return min({coord_arr[1][0], coord_arr[1][1], coord_arr[1][2]});
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
    void enable_push()
    {
        push_init = true;
    }

};

TStack::TStack()
{
    size = 1;
    push_init = false;
    double **id_mat = getMat(4, 4);
    for(int i=0; i<4; i++) id_mat[i][i] = 1.0;
    trans_stack.push_back(id_mat);
}

void TStack::push(double **trans_mat)
{
    if(size > 0 && push_init)
    {
        trans_stack.push_back(trans_mat);
        size++;
    }
}

void TStack::pop()
{
    if(size > 1)
    {
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

    R[3][3] = 1;
    return R;
}

double **get_view_trans(Point eye)
{
    double **T = get_translate_mat(-eye.x, -eye.y, -eye.z);
    T[3][3] = 1;
    return T;
}

double **get_projection_mat(double fovY, double aspectRatio, double near, double far)
{
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY/2 * (acos(0)/90.0));
    double r = near * tan(fovX/2* (acos(0)/90.0));

    double **ProjectionMat = getMat(4, 4);
    ProjectionMat[0][0] = near/r;
    ProjectionMat[1][1] = near/t;
    ProjectionMat[2][2] = -(far+near)/(far-near);
    ProjectionMat[2][3] = -(2*far*near)/(far-near);
    ProjectionMat[3][2] = -1;

    return ProjectionMat;
}


//==================Hidden Surface Removal===========================
void triangle_to_image()
{
    int screenWidth, screenHeight;
    double minX, minY, maxX, maxY;
    double zMax, zMin;

    ifstream stage3, config;
    config.open("config.txt");

    //read configuration vars
    config >> screenWidth >> screenHeight >> minX >> minY;
    config >> zMin >> zMax;
    maxX = -minX;
    maxY = -minY;

    config.close();

    Point p1, p2, p3;
    stage3.open("stage3.txt");

    //initialize z-buffer
    double zBuffer[screenWidth][screenHeight];
    for(int i=0; i<screenWidth; i++)
        for(int j=0; j<screenHeight; j++)
            zBuffer[i][j] = zMax;

    //initialize image object
    bitmap_image image(screenWidth, screenHeight);
    for(int i=0; i<screenWidth; i++)
        for(int j=0; j<screenHeight; j++)
            image.set_pixel(i, j, 0, 0, 0);

    //necessary vars for scanning
    double dx = (maxX - minX) / screenWidth;
    double dy = (maxY - minY) / screenHeight;

    double topY = maxY - dy / 2;
    double bottomY = minY + dy / 2;

    double rightX = maxX - dx / 2;
    double leftX = minX + dx / 2;

    double slope, zp;
    double largestY, smallestY, ys;
    int topPixel, bottomPixel, leftLim, rightLim;

    //begin procedure for each triangle
    while(stage3 >> p1.x)
    {
        stage3 >> p1.y >> p1.z;
        stage3 >> p2.x >> p2.y >> p2.z;
        stage3 >> p3.x >> p3.y >> p3.z;

        Triangle t(p1, p2, p3);
        //t.print_triangle();
        t.set_color();

        //top and bottom scan-line pixels
        largestY = min(topY, t.get_maxY());
        smallestY = max(bottomY, t.get_minY());
        topPixel = floor( (topY-largestY) / dy);
        bottomPixel = floor( (topY-smallestY) / dy);

        ys = topY- topPixel*dy;
        for(int i=topPixel; i<=bottomPixel; i++)
        {
            //get intersections with sides of triangle
            Point a = get_intersection(p1, p2, ys);
            Point b = get_intersection(p1, p3, ys);
            Point c = get_intersection(p2, p3, ys);
            ys -= dy;
            if(a.x==INF && b.x==INF && c.x==INF) continue;

            //determine left and right intersections
            Point leftInter, rightInter;
            if(a.x == INF){
                leftInter = b.x < c.x ? b : c;
                rightInter = b.x > c.x ? b : c;
            }
            else if(b.x == INF){
                leftInter = a.x < c.x ? a : c;
                rightInter = a.x > c.x ? a : c;
            }
            else{
                leftInter = b.x < a.x ? b : a;
                rightInter = b.x > a.x ? b : a;
            }

            //depth calculation of leftPoint
            zp = leftInter.z;
            slope = (rightInter.z-leftInter.z)/(rightInter.x-leftInter.x);

            //box range check
            if (leftInter.x < leftX) {
               zp += (leftX - leftInter.x) * slope;
               leftInter.x = leftX;
            }
            if (rightInter.x > rightX)
                rightInter.x = rightX;


            //scan left to right
            leftLim = floor((leftInter.x - leftX) / dx);
            rightLim = floor((rightInter.x - leftX) / dx);
            for (int j=leftLim; j<=rightLim; j++)
            {
                if (zp >= zMin && zBuffer[i][j] > zp)
                {
                    zBuffer[i][j] = zp;
                    image.set_pixel(j, i, t.color.r, t.color.g, t.color.b);
                }
                zp += (dx * slope);
            }

//            cout << "Points: " << ys << endl;
//            cout << leftInter.x << " " << leftInter.y << " " << leftInter.z << endl;
//            cout << rightInter.x << " " << rightInter.y << " " << rightInter.z << endl;
//            cout << endl;
        }



    }

    image.save_image("out.bmp");
    stage3.close();
}


int main()
{
    srand(time(NULL));
    cout << fixed;
    cout << setprecision(7);

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

    ofstream ofs[3];
    ofs[0].open("stage1.txt");
    ofs[1].open("stage2.txt");
    ofs[2].open("stage3.txt");
    for(int i=0; i<3; i++) ofs[i] << setprecision(7) << fixed;

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

            //transform
            double **trans_mat = tstack.top();
            triangle.set_coord(
                mat_mul(trans_mat, triangle.get_coord(), 4, 3, 4),
                true
            );
            triangle.print_triangle(&ofs[0]);

            //view
            triangle.set_coord(
                mat_mul(ViewMat, triangle.get_coord(), 4, 3, 4),
                true
            );
            triangle.print_triangle(&ofs[1]);

            //projection
            triangle.set_coord(
                mat_mul(ProjectionMat, triangle.get_coord(), 4, 3, 4),
                true
            );
            triangle.print_triangle(&ofs[2]);
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

    for(int i=0; i<3; i++) ofs[i].close();
    fclose(stdin);





    //triangle to bitmap image
    //with hidden surface removal
    triangle_to_image();




    return 0;
}

















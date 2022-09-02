#ifndef INCLUDE_LINE_HPP
#define INCLUDE_LINE_HPP


#include <bits/stdc++.h>
#include "1705110_bitmap_image.hpp"
using namespace std;

int W, H, N;

class Color {
public:
    int r, g, b;
    Color(int r, int g, int b) : r(r), g(g), b(b) {};
    Color() : r(0), g(0), b(0) {};
};

class Line {
public:
    int x0, y0, x1, y1;
    Color color;
    Line(int x0, int y0, int x1, int y1) {
        this->x0 = x0;
        this->y0 = y0;
        this->x1 = x1;
        this->y1 = y1;
    }
    Line() {
        this->x0 = 0;
        this->y0 = 0;
        this->x1 = 10;
        this->y1 = 10;
    }

    friend ostream &operator<<(ostream &out, const Line &line)
    {
        out << "Line is: (" << line.x0 << "," << line.y0 << ") -";
        out << "- (" << line.x1 << "," << line.y1 << ")" << endl;
        return out;
    }

    friend istream &operator>>(istream &ins, Line &line)
    {
        ins >> line.x0 >> line.y0 >> line.x1 >> line.y1;
        ins >> line.color.r >> line.color.g >> line.color.b;
        return ins;
    }
};

vector<Line> lines;

bool isInsideImageBounds(int x, int y)
{
    if(x > 0 && x < W-1 && y > 0 && y < H-1)
        return true;
    return false;
}

void colorImage(bitmap_image &image, int x, int y, Color color, bool isSwap=false)
{
    if(isSwap) swap(x, y); //reflection on y=x line
    if(!isInsideImageBounds(x, y))
        return;
    image.set_pixel(x, H-y, color.r, color.g, color.b);
}

bool sameSign(double a, double b)
{
    return (a < 0.0 && b < 0.0) || (a > 0.0 && b > 0.0);
}

#endif // INCLUDE_LINE_HPP
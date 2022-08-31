#include <bits/stdc++.h>
#include "1705110_bitmap_image.hpp"
using namespace std;

int W, H, N;

class Color {
public:
    int r, g, b;
    Color(int r, int g, int b) : r(r), g(g), b(b) {};
};

class Line {
public:
    int x0, y0, x1, y1;
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
        return ins;
    }
};

vector<Line> lines;

bool isInsideBounds(int x, int y)
{
    if(x > 0 && x < W-1 && y > 0 && y < H-1)
        return true;
    return false;
}

void colorImage(bitmap_image &image, int x, int y, Color color, bool isSwap=false) 
{
    if(isSwap) swap(x, y); //reflection on y=x line
    if(!isInsideBounds(x, y)) 
        return;
    image.set_pixel(x, H-y, color.r, color.g, color.b);
}

void MidpointLine(Line line, bitmap_image &image, bool isLargeGradient, bool isGradientNegative, Color color)
{
    if(isLargeGradient) {//reflection on y=x line
        swap(line.x1, line.y1);
        swap(line.x0, line.y0);
    }
        
    int dx = line.x1 - line.x0;
    int dy = line.y1 - line.y0;
    int d = 2 * dy - dx;
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - dx);

    int x = line.x0;
    int y = line.y0;

    while (x < line.x1) {
        if (d <= 0) {
            d += incrE;
            x++;
        } else {
            d += incrNE;
            x++;
            y++;
        }
        isGradientNegative
            ? isLargeGradient 
                ? colorImage(image, x, -y, color, isLargeGradient) 
                : colorImage(image, -x, y, color, isLargeGradient)
            : colorImage(image, x, y, color, isLargeGradient);
    }    
}

void scanLineMidPoint(Line line, bitmap_image &image)
{
    double m = (double)(line.y1 - line.y0) / (double)(line.x1 - line.x0);
    Color color(255, 255, 255);
    if(m < 0) {
        line.x0 = -line.x0;
        line.x1 = -line.x1;
        swap(line.x0, line.x1);
        swap(line.y0, line.y1);
        color.r = 0;
        MidpointLine(line, image, abs(m) > 1, m < 0, color);
    }
    color.b = 0;
    MidpointLine(line, image, abs(m) > 1, m < 0, color);
}
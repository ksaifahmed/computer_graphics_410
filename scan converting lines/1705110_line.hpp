#include <bits/stdc++.h>
#include "1705110_bitmap_image.hpp"
using namespace std;

int W, H, N;
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
    if(x >= 0 && x < W && y >= 0 && y < H)
        return true;
    return false;
}

void colorImage(bitmap_image &image, int x, int y, int r, int g, int b) 
{
    if(!isInsideBounds(x, y)) 
        return;
    image.set_pixel(x, H-y, r, g, b);
}

void MidpointLine(Line line, bitmap_image &image)
{
    int dx = line.x1 - line.x0;
    int dy = line.y1 - line.y0;
    int d = 2 * dy - dx;
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - dx);

    int x = line.x0;
    int y = line.y0;

    colorImage(image, x, y, 255, 255, 255);
    while (x < line.x1) {
        cout << "(" << x << "," << y << ")" << endl;
        if (d <= 0) {
            d += incrE;
            x++;
        } else {
            d += incrNE;
            x++;
            y++;
        }
        colorImage(image, x, y, 255, 255, 255);
    }    
}
#ifndef INCLUDE_SCANLINES_HPP
#define INCLUDE_SCANLINES_HPP

#include <bits/stdc++.h>
#include "1705110_bitmap_image.hpp"
#include "1705110_line.hpp"
using namespace std;



// Midpoint Line Algorithm for all slopes ==================================================================
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

void scanLine_MidPoint(Line line, bitmap_image &image)
{
    double m = (double)(line.y1 - line.y0) / (double)(line.x1 - line.x0);
    Color color(255, 255, 255);
    if(m < 0) {
        line.x0 = -line.x0;
        line.x1 = -line.x1;
        swap(line.x0, line.x1);
        swap(line.y0, line.y1);
        //color.r = 0;
        MidpointLine(line, image, abs(m) > 1, m < 0, color);
    }
    //color.b = 0;
    MidpointLine(line, image, abs(m) > 1, m < 0, color);
}
// ==================================================================================================




// Anti-Aliasing using Weighted Area Sampling =======================================================
double Filter(int distance){
    return max(0.0, 1-0.25*distance);
}

void IntensifyPixelWA(int x, int y, double distance, bitmap_image &image, Color color, bool isSwap)
{
    double intensity = Filter(round(abs(distance)));
    if(isSwap) swap(x, y); //reflection on y=x line
    if(!isInsideBounds(x, y))
        return;
    image.set_pixel(x, H-y, color.r*intensity, color.g*intensity, color.b*intensity);
}

void colorImageWA(bitmap_image &image, int x, int y, Color color, bool isSwap, int two_v_dx, double invDenom, double two_dx_invDenom)
{
    // for SMOOTH antialiasing
    IntensifyPixelWA(x, y, two_v_dx * invDenom, image, color, isSwap);
    IntensifyPixelWA(x, y + 1, two_dx_invDenom - two_v_dx * invDenom, image, color, isSwap);
    IntensifyPixelWA(x, y - 1, two_dx_invDenom + two_v_dx * invDenom, image, color, isSwap);
    IntensifyPixelWA(x, y + 2, 2*two_dx_invDenom - (two_v_dx * invDenom), image, color, isSwap);
    IntensifyPixelWA(x, y - 2, 2*two_dx_invDenom + (two_v_dx * invDenom), image, color, isSwap);
}

void WeightedAreaSamplingAntiAliased(Line line, bitmap_image &image, bool isLargeGradient, bool isGradientNegative, Color color)
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

    // for antialiasing
    int two_v_dx = 0;
    double invDenom = 1.0 / (2.0 * sqrt(dx * dx + dy * dy));
    double two_dx_invDenom = 2.0 * dx * invDenom;

    int x = line.x0;
    int y = line.y0;

    while (x < line.x1) {
        if (d < 0) {
            two_v_dx = d + dx;
            d += incrE;
            x++;
        } else {
            two_v_dx = d - dx;
            d += incrNE;
            x++;
            y++;
        }

        isGradientNegative
            ? isLargeGradient
                ? colorImageWA(image, x, -y, color, isLargeGradient, two_v_dx, invDenom, two_dx_invDenom)
                : colorImageWA(image, -x, y, color, isLargeGradient, two_v_dx, invDenom, two_dx_invDenom)
            : colorImageWA(image, x, y, color, isLargeGradient, two_v_dx, invDenom, two_dx_invDenom);
    }
}

void scanLine_WeightedAreaSamplingAntiAliased(Line line, bitmap_image &image)
{
    double m = (double)(line.y1 - line.y0) / (double)(line.x1 - line.x0);
    Color color(255, 255, 255);
    if(m < 0) {
        line.x0 = -line.x0;
        line.x1 = -line.x1;
        swap(line.x0, line.x1);
        swap(line.y0, line.y1);
        //color.r = 0;
        WeightedAreaSamplingAntiAliased(line, image, abs(m) > 1, m < 0, color);
    }
    //color.b = 0;
    WeightedAreaSamplingAntiAliased(line, image, abs(m) > 1, m < 0, color);
}
// ==================================================================================================




// Anti-Aliasing using Unweighted Area Sampling =====================================================
void IntensifyPixelUA(int x, int y, bitmap_image &image, Color color, bool isSwap)
{
    if(isSwap) swap(x, y); //reflection on y=x line
    if(!isInsideBounds(x, y))
        return;
    image.set_pixel(x, H-y, color.r, color.g, color.b);
}

void colorImageUA(bitmap_image &image, int x, int y, Color color, bool isSwap)
{
    IntensifyPixelUA(x, y, image, color, isSwap);
    IntensifyPixelUA(x, y+1, image, color, isSwap);
    IntensifyPixelUA(x, y-1, image, color, isSwap);
}

void UnweightedAreaSamplingAntiAliasedLines(Line line, bitmap_image &image, bool isLargeGradient, bool isGradientNegative, Color color)
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
        if (d < 0) {
            d += incrE;
            x++;
        } else {
            d += incrNE;
            x++;
            y++;
        }

        isGradientNegative
            ? isLargeGradient
                ? colorImageUA(image, x, -y, color, isLargeGradient)
                : colorImageUA(image, -x, y, color, isLargeGradient)
            : colorImageUA(image, x, y, color, isLargeGradient);
    }
}

void scanLine_UnweightedAreaSamplingAntiAliased(Line line, bitmap_image &image)
{
    double m = (double)(line.y1 - line.y0) / (double)(line.x1 - line.x0);
    Color color(255, 255, 255);
    if(m < 0) {
        line.x0 = -line.x0;
        line.x1 = -line.x1;
        swap(line.x0, line.x1);
        swap(line.y0, line.y1);
        //color.r = 0;
        UnweightedAreaSamplingAntiAliasedLines(line, image, abs(m) > 1, m < 0, color);
    }
    //color.b = 0;
    UnweightedAreaSamplingAntiAliasedLines(line, image, abs(m) > 1, m < 0, color);
}



#endif // SCAN_LINE_HPP_INCLUDED

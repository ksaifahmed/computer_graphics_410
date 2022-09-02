#include <bits/stdc++.h>
#include "1705110_scanlines.hpp"
using namespace std;


void capture_MidPoint() {
	bitmap_image image(W, H);
	for (int i = 0; i < W; i++)
		for (int j = 0; j < H; j++)
			image.set_pixel(i, j, 0, 0, 0);

    for (auto & line : lines){
        scanLine_MidPoint(line, image);
    }
    cout << "1_R Image is captured" << endl;
    image.save_image("1_R.bmp");
}

void capture_UnweightedAreaSampling() {
    bitmap_image image(W, H);
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
            image.set_pixel(i, j, 0, 0, 0);

    for (auto & line : lines){
        scanLine_UnweightedAreaSamplingAntiAliased(line, image);
    }
    cout << "2_RUA Image is captured" << endl;
    image.save_image("2_RUA.bmp");
}


void capture_WeightedAreaSampling() {
	bitmap_image image(W, H);
	for (int i = 0; i < W; i++)
		for (int j = 0; j < H; j++)
			image.set_pixel(i, j, 0, 0, 0);

    for (auto & line : lines){
        scanLine_WeightedAreaSamplingAntiAliased(line, image);
    }
    cout << "3_RWA Image is captured" << endl;
    image.save_image("3_RWA.bmp");
}

int main()
{
    ifstream fin("input.txt");
    if(!fin.is_open()) {
        cout << "Cannot open input.txt" << endl;
        return 0;
    }

    Line line;
    fin >> W >> H >> N;
    cout << W << " " << H << " " << N << endl;
    for (int i = 0; i < N; i++) {
        fin >> line;
        lines.push_back(line);
    }

    //check if lines are read from input
    for (auto & line : lines)
        cout << line;

    capture_MidPoint();
    capture_UnweightedAreaSampling();      
    capture_WeightedAreaSampling();

    fin.close();
    return 0;
}

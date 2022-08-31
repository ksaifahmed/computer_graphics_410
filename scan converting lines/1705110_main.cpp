#include <bits/stdc++.h>
#include "1705110_line.hpp"
using namespace std;


void capture() {
	bitmap_image image(W, H);
	for (int i = 0; i < W; i++)
		for (int j = 0; j < H; j++)
			image.set_pixel(i, j, 0, 0, 0);

    scanLine(lines[0], image);
    scanLine(lines[1], image);
    scanLine(lines[2], image);
    scanLine(lines[3], image);
    scanLine(lines[4], image);

    image.save_image("image.bmp");
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

    capture();

    fin.close();
    return 0;
}

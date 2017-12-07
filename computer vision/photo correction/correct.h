#include "edge_detect.h"

#define A4_WIDTH 420.0
#define A4_HEIGHT 594.0


vector<my_pair> calculate_corners(vector<my_pair> v) {
	vector<my_pair> corners;
	for (int i = 0; i < v.size(); i++) {
		for (int j = i + 1; j < v.size(); j++) {
			int value = abs(v[i].x - v[j].x);
			if (value < 10 || (value >= 170 && value < 190)) continue;

			double n = cosine[v[i].x] * sine[v[j].x] - sine[v[i].x] * cosine[v[j].x];
			int x = (sine[v[j].x] * v[i].y - sine[v[i].x] * v[j].y) / n + 0.5;
			int y = (cosine[v[i].x] * v[j].y - cosine[v[j].x] * v[i].y) / n + 0.5;
			corners.push_back(my_pair(x, y));
		}
	}
	return corners;
}


CImg<unsigned char> correct(CImg<unsigned char> img, vector<my_pair> c) {
	CImg<unsigned char> paper(A4_WIDTH, A4_HEIGHT, img.depth(), img.spectrum());

	for (int i = 0; i < c.size(); i++) {
		int min = i;
		for (int j = i; j < c.size(); j++) {
			min = (c[min].x*c[min].x + c[min].y*c[min].y > c[j].x*c[j].x + c[j].y*c[j].y) ? j : min;
		}
		my_pair tmp = c[i];
		c[i] = c[min];
		c[min] = tmp;
		printf("(%d,%d) ", c[i].x, c[i].y);
	}

	my_pair v1(c[1].x - c[0].x, c[1].y - c[0].y), v2(c[2].x - c[0].x, c[2].y - c[0].y);

	cimg_forXY(paper, x, y) {
		double u = (x*1.0) / A4_WIDTH;
		double v = (y*1.0) / A4_HEIGHT;

		int src_x = 0, src_y = 0;
		src_x = u*v1.x + v*v2.x + c[0].x;
		src_y = u*v1.y + v*v2.y + c[0].y;

		paper(x, y, 0) = img(src_x, src_y, 0);
		paper(x, y, 1) = img(src_x, src_y, 1);
		paper(x, y, 2) = img(src_x, src_y, 2);
	}

	return paper;
}
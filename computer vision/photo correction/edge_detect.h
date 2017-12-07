#include "canny.h"
#include <cmath>
#include <vector>
#include <cstdio>

#define MAX_ANGLE 360
#define MAX_LENGTH 800
#define N 200
#define PI 3.1415926

using namespace cimg_library;
using namespace std;

struct point {
	point(int i, int j) {
		x = i;
		y = j;
	}

	int x;
	int y;
};

int acc[MAX_ANGLE][MAX_LENGTH] = { { 0 } };
vector<point> points[MAX_ANGLE][MAX_LENGTH];
double cosine[MAX_ANGLE];
double sine[MAX_ANGLE];

bool redundant(int a, int l, vector<point> v) {
	for (int i = 0; i < v.size(); i++) {
		int value = abs(v[i].x - a);
		if (value < 10 || (value >= 170 && value < 190)) {
			if (abs(v[i].y - l) < 30) return true;
		}
	}
	return false;
}

void print_corners(vector<point> v) {
	for (int i = 0; i < v.size(); i++) {
		for (int j = i + 1; j < v.size(); j++) {
			int value = abs(v[i].x - v[j].x);
			if (value < 10 || (value >= 170 && value < 190)) continue;
			
			double n = cosine[v[i].x] * sine[v[j].x] - sine[v[i].x] * cosine[v[j].x];
			int x = (sine[v[j].x] * v[i].y - sine[v[i].x] * v[j].y) / n;
			int y = (cosine[v[i].x] * v[j].y - cosine[v[j].x] * v[i].y) / n;
			printf("(%d,%d)\n", x, y);
		}
	}
}

CImg<unsigned char> hough(CImg<unsigned char> img) {
	CImg<unsigned char> h_img(img.width(), img.height(), 1, 1);
	cimg_forXY(h_img, x, y) {
		h_img(x, y, 0) = 0;
	}
	
	int max_len = sqrt(img.width()*img.width() + img.height()*img.height())+0.5;
	
	
	for (int i = 0; i < MAX_ANGLE; i++) {
		cosine[i] = cos(i * 180 / PI);
		sine[i] = sin(i * 180 / PI);
	}

	cimg_forXY(img, x, y) {
		if (img(x, y, 0) == 255) {
			for (int i = 0; i < MAX_ANGLE; i++) {
				int p = x*cosine[i] + y*sine[i];
				acc[i][p]++;
				points[i][p].push_back(point(x, y));
			}
		}
	}

	vector<point> lines;
	for (int i = 0; i < MAX_ANGLE; i++) {
		for (int j = 0; j < max_len; j++) {
			if (acc[i][j] > N) {
				if (lines.size() < 4 && !redundant(i, j, lines)) {
					lines.push_back(point(i, j));
					printf("%d = xcos(%d)+ysin(%d)\n", j, i, i);
				}
				for (int k = 0; k < points[i][j].size(); k++) {
					point p = points[i][j].at(k);
					h_img(p.x, p.y) = 255;
				}
			}
		}
	}

	cimg_forXY(h_img, x, y) {
		if (h_img(x, y, 0) == 255) {
			int count = 0;
			for (int i = 0; i < lines.size(); i++) {
				int value = abs(cosine[lines[i].x] * x + sine[lines[i].x] * y - lines[i].y);
				if (value <= 0.05) count++;
			}
			if (count > 1) printf("(%d,%d)\n", x, y);
		}
	}

	//print_corners(lines);

	return h_img;
}

CImg<unsigned char> edge_detect(CImg<unsigned char> src_img) {
	src_img.display("origin image");

	CImg<unsigned char> g_img = toGrayScale(src_img);
	g_img.display("gray scale");

	g_img.blur(1);

	CImg<unsigned char> angles;
	CImg<unsigned char> s_img = sobel(g_img, angles);
	CImg<unsigned char> t_img = threshold(s_img, 140, 140);
	CImg<unsigned char> n_img = nonMaxSupp(t_img, angles);

	n_img.display("approximate edge");

	CImg<unsigned char> h_img = hough(n_img);
	h_img.display("accurate edge");

	return h_img;
}
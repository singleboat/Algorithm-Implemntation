#include "CImg.h"
#include <vector>

#define PI 3.1415926

using namespace std;
using namespace cimg_library;

CImg<unsigned char> toGrayScale(CImg<unsigned char> src_img) {
	CImg<unsigned char> gray_img(src_img.width(), src_img.height(), 1, 1);

	cimg_forXY(gray_img, x, y) {
		double new_value = src_img(x, y, 0)*0.2126 + src_img(x, y, 1)*0.7125 + src_img(x, y, 2)*0.0772;
		gray_img(x, y) = new_value;
	}

	return gray_img;
}

vector<vector<double> > createFilter(int row, int column, double sigma) {
	vector<vector<double>> filter;

	for (int i = 0; i < row; i++)
	{
		vector<double> col;
		for (int j = 0; j < column; j++)
		{
			col.push_back(-1);
		}
		filter.push_back(col);
	}

	float coordSum = 0;
	float constant = 2.0 * sigma * sigma;

	// Sum is for normalization
	float sum = 0.0;

	for (int x = -row / 2; x <= row / 2; x++)
	{
		for (int y = -column / 2; y <= column / 2; y++)
		{
			coordSum = (x*x + y*y);
			filter[x + row / 2][y + column / 2] = (exp(-(coordSum) / constant)) / (PI * constant);
			sum += filter[x + row / 2][y + column / 2];
		}
	}

	// Normalize the Filter
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			filter[i][j] /= sum;

	return filter;
}


CImg<unsigned char> useFilter(CImg<unsigned char> img, vector<vector<double> > filter) {
	int size = (int)filter.size() / 2;

	CImg<unsigned char> filter_img(img.width() - 2 * size, img.height() - 2 * size, 1, 1);
	//Mat filteredImg = Mat(img.rows - 2 * size,img_in.cols - 2 * size,CV_8UC1);
	for (int i = size; i < img.width() - size; i++)
	{
		for (int j = size; j < img.height() - size; j++)
		{
			double sum = 0;

			for (int x = 0; x < filter.size(); x++)
				for (int y = 0; y < filter.size(); y++)
				{
					sum += filter[x][y] * (double)(img(i + x - size, j + y - size));
				}

			filter_img(i - size, j - size) = sum;
		}

	}
	return filter_img;
}


CImg<unsigned char> sobel(CImg<unsigned char> g_filter_img, CImg<unsigned char>& angles) {
	//Sobel X Filter
	double x1[] = { -1.0, 0, 1.0 };
	double x2[] = { -2.0, 0, 2.0 };
	double x3[] = { -1.0, 0, 1.0 };

	vector<vector<double> > xFilter(3);
	xFilter[0].assign(x1, x1 + 3);
	xFilter[1].assign(x2, x2 + 3);
	xFilter[2].assign(x3, x3 + 3);

	//Sobel Y Filter
	double y1[] = { 1.0, 2.0, 1.0 };
	double y2[] = { 0, 0, 0 };
	double y3[] = { -1.0, -2.0, -1.0 };

	vector<vector<double> > yFilter(3);
	yFilter[0].assign(y1, y1 + 3);
	yFilter[1].assign(y2, y2 + 3);
	yFilter[2].assign(y3, y3 + 3);

	//Limit Size
	int size = (int)xFilter.size() / 2;

	CImg<unsigned char> filter_img(g_filter_img.width() - 2 * size, g_filter_img.height() - 2 * size, 1, 1);

	CImg<unsigned char> angle(g_filter_img.width() - 2 * size, g_filter_img.height() - 2 * size, 1, 1);
	angles = angle;


	for (int i = size; i < g_filter_img.width() - size; i++)
	{
		for (int j = size; j < g_filter_img.height() - size; j++)
		{
			double sumx = 0;
			double sumy = 0;

			for (int x = 0; x < xFilter.size(); x++)
				for (int y = 0; y < xFilter.size(); y++)
				{
					sumx += xFilter[x][y] * (double)(g_filter_img(i + x - size, j + y - size)); //Sobel_X Filter Value
					sumy += yFilter[x][y] * (double)(g_filter_img(i + x - size, j + y - size)); //Sobel_Y Filter Value
				}
			double sumxsq = sumx*sumx;
			double sumysq = sumy*sumy;

			double sq2 = sqrt(sumxsq + sumysq);

			if (sq2 > 255) //Unsigned Char Fix
				sq2 = 255;
			filter_img(i - size, j - size) = sq2;

			if (sumx == 0) //Arctan Fix
				angles(i - size, j - size) = 90;
			else
				angles(i - size, j - size) = atan(sumy / sumx) * 180 / PI;
		}
	}

	return filter_img;
}


CImg<unsigned char> nonMaxSupp(CImg<unsigned char> s_filter_img, CImg<unsigned char> angles) {
	CImg<unsigned char> nonMaxSupped(s_filter_img.width() - 2, s_filter_img.height() - 2, 1, 1);
	//Mat nonMaxSupped = Mat(sFiltered.rows - 2,sFiltered.cols - 2,CV_8UC1);
	for (int i = 1; i< s_filter_img.width() - 1; i++) {
		for (int j = 1; j<s_filter_img.height() - 1; j++) {
			float Tangent = angles(i, j);

			nonMaxSupped(i - 1, j - 1) = s_filter_img(i, j);
			//Horizontal Edge
			if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
			{
				if ((s_filter_img(i, j) < s_filter_img(i, j + 1)) || s_filter_img(i, j) < s_filter_img(i, j - 1))
					nonMaxSupped(i - 1, j - 1) = 0;
			}
			//Vertical Edge
			if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
			{
				if ((s_filter_img(i, j) < s_filter_img(i + 1, j)) || s_filter_img(i, j) < s_filter_img(i - 1, j))
					nonMaxSupped(i - 1, j - 1) = 0;
			}

			//-45 Degree Edge
			if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
			{
				if ((s_filter_img(i, j) < s_filter_img(i - 1, j + 1)) || s_filter_img(i, j) < s_filter_img(i + 1, j - 1))
					nonMaxSupped(i - 1, j - 1) = 0;
			}

			//45 Degree Edge
			if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
			{
				if ((s_filter_img(i, j) < s_filter_img(i + 1, j + 1)) || s_filter_img(i, j) < s_filter_img(i - 1, j - 1))
					nonMaxSupped(i - 1, j - 1) = 0;
			}
		}
	}
	return nonMaxSupped;
}

CImg<unsigned char> threshold(CImg<unsigned char> img, int low, int high) {
	if (low > 255)
		low = 255;
	if (high > 255)
		high = 255;

	CImg<unsigned char> EdgeMat(img.width(), img.height(), 1, img.spectrum());
	//Mat EdgeMat = Mat(imgin.rows,imgin.cols,imgin.type());

	for (int i = 0; i<img.width(); i++)
	{
		for (int j = 0; j<img.height(); j++)
		{
			EdgeMat(i, j) = img(i, j);
			if (EdgeMat(i, j) > high)
				EdgeMat(i, j) = 255;
			else if (EdgeMat(i, j) < low)
				EdgeMat(i, j) = 0;
			else
			{
				bool anyHigh = false;
				bool anyBetween = false;
				for (int x = i - 1; x < i + 2; x++)
				{
					for (int y = j - 1; y<j + 2; y++)
					{
						if (x <= 0 || y <= 0 || EdgeMat.width() || y > EdgeMat.height()) //Out of bounds
							continue;
						else
						{
							if (EdgeMat(x, y) > high)
							{
								EdgeMat(i, j) = 255;
								anyHigh = true;
								break;
							}
							else if (EdgeMat(x, y) <= high && EdgeMat(x, y) >= low)
								anyBetween = true;
						}
					}
					if (anyHigh)
						break;
				}
				if (!anyHigh && anyBetween)
					for (int x = i - 2; x < i + 3; x++)
					{
						for (int y = j - 1; y<j + 3; y++)
						{
							if (x < 0 || y < 0 || x > EdgeMat.width() || y > EdgeMat.height()) //Out of bounds
								continue;
							else
							{
								if (EdgeMat(x, y) > high)
								{
									EdgeMat(i, j) = 255;
									anyHigh = true;
									break;
								}
							}
						}
						if (anyHigh)
							break;
					}
				if (!anyHigh)
					EdgeMat(i, j) = 0;
			}
		}
	}
	return EdgeMat;
}
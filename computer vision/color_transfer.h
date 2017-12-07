#include "CImg.h"

#include <cmath>
#include <iostream>

using namespace cimg_library;

float* cal_m(CImg<float> img) {
	float* m = new float[3];
	float n = img.width() * img.height();

	m[0] = m[1] = m[2] = 0;

	cimg_forXY(img, x, y) {
		m[0] += img(x, y, 0) / n;
		m[1] += img(x, y, 1) / n;
		m[2] += img(x, y, 2) / n;
	}

	return m;
}

float* cal_sd(CImg<float> img, float* m) {
	float* sd = new float[3];
	float n = img.width() * img.height();

	sd[0] = sd[1] = sd[2] = 0;

	cimg_forXY(img, x, y) {
		sd[0] += (img(x, y, 0) - m[0])*(img(x, y, 0) - m[0]) / n;
		sd[1] += (img(x, y, 1) - m[1])*(img(x, y, 1) - m[1]) / n;
		sd[2] += (img(x, y, 2) - m[2])*(img(x, y, 2) - m[2]) / n;
	}

	sd[0] = sqrt(sd[0]);
	sd[1] = sqrt(sd[1]);
	sd[2] = sqrt(sd[2]);

	return sd;
}

CImg<unsigned char> color_transfer(CImg<unsigned char> src_img, CImg<unsigned char> tgt_img) {
	CImg<float> src_img_lab = src_img.get_RGBtoLab();
	CImg<float> tgt_img_lab = tgt_img.get_RGBtoLab();

	float* src_m = cal_m(src_img_lab);
	float* src_sd = cal_sd(src_img_lab, src_m);
	float* tgt_m = cal_m(tgt_img_lab);
	float* tgt_sd = cal_sd(tgt_img_lab, tgt_m);

	std::cout << src_m[0] << " " << src_m[1] << " " << src_m[2] << std::endl;
	std::cout << src_sd[0] << " " << src_sd[1] << " " << src_sd[2] << std::endl;

	CImg<float> result_img(src_img_lab.width(), src_img_lab.height(), src_img_lab.depth(), src_img_lab.spectrum());

	cimg_forXY(result_img, x, y) {
		result_img(x, y, 0) = (src_img_lab(x, y, 0) - src_m[0])*(tgt_sd[0] / src_sd[0]) + tgt_m[0];
		result_img(x, y, 1) = (src_img_lab(x, y, 1) - src_m[1])*(tgt_sd[1] / src_sd[1]) + tgt_m[1];
		result_img(x, y, 2) = (src_img_lab(x, y, 2) - src_m[2])*(tgt_sd[2] / src_sd[2]) + tgt_m[2];
	}

	delete[] src_m;
	delete[] src_sd;
	delete[] tgt_m;
	delete[] tgt_sd;

	CImg<unsigned char> fin_img = result_img.get_LabtoRGB();
	fin_img.display("color transfer");
	return fin_img;
}
#include "CImg.h"

using namespace cimg_library;

CImg<unsigned char> histogram_equalize(CImg<unsigned char> src_img, int spectrums) {
	CImg<unsigned char> new_img(src_img.width(), src_img.height(), src_img.depth(), spectrums);

	int count[3][256] = { 0 };
	int max[3] = { 0 };
	int n = 0;

	cimg_forXY(src_img, x, y) {
		for (int i = 0; i < spectrums; i++) {
			if (src_img(x, y, i) > 255) src_img(x, y, i) = 255;
			if (src_img(x, y, i) < 0) src_img(x, y, i) = 0;
			count[i][src_img(x, y, i)]++;
			max[i] = (max[i] >= src_img(x, y, i)) ? max[i] : src_img(x, y, i);
			n++;
			new_img(x, y, i) = src_img(x, y, i);
		}
	}

	int sum[3] = { 0 };
	int p_new[3][256] = { 0 };

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < spectrums; j++) {
			sum[j] += count[j][i];
			p_new[j][i] = max[j] * sum[j] / n;
		}
	}

	cimg_forXY(src_img, x, y) {
		for (int i = 0; i < spectrums; i++) new_img(x, y, i) = p_new[i][src_img(x, y, i)];
	}

	new_img.display("");
	return new_img;
}

CImg<unsigned char> to_gray_scale(CImg<unsigned char> src_img) {
	CImg<unsigned char> gray_img(src_img.width(), src_img.height(), 1, 1);

	cimg_forXY(gray_img, x, y) {
		double new_value = src_img(x, y, 0)*0.2126 + src_img(x, y, 1)*0.7125 + src_img(x, y, 2)*0.0772;
		gray_img(x, y) = new_value;
	}

	gray_img.display("gray scale");
	return gray_img;
}
#pragma once
#include <opencv2\core.hpp>
#include "sift\imgfeatures.h"


#define K 50000
#define INLIER_THR 5


using namespace cv;


struct feature_match {
	feature_match(int a, int b) {
		id1 = a;
		id2 = b;
	}

	int id1;
	int id2;
};


Mat stitch(IplImage **img, int size);
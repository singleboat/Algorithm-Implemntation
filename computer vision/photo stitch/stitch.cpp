#include "stitch.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include "sift\sift.h"
#include "sift\imgfeatures.h"


using namespace std;


vector<feature_match> find_match(feature *f1, feature *f2, int n1, int n2) {
	vector<feature_match> matches;
	double dist, d_fst, d_scd;
	double tmp;
	int id;
	int i, j;

	//2NN，暴力匹配
	for (i = 0; i < n1; i++) {
		id = 0;
		d_fst = descr_dist_sq(f1 + i, f2);
		d_scd = descr_dist_sq(f1 + i, f2 + 1);

		if (d_fst > d_scd) {
			tmp = d_fst;
			d_fst = d_scd;
			d_scd = tmp;
			id = 1;
		}

		for (j = 2; j < n2; j++) {
			dist = descr_dist_sq(f1 + i, f2 + j);
			if (dist < d_fst) {
				d_scd = d_fst;
				d_fst = dist;
				id = j;
			}
			else if (dist < d_scd) {
				d_scd = dist;
			}
		}

		if (d_fst / d_scd < 0.8) matches.push_back(feature_match(i, id));
	}

	return matches;
}


bool exist(int x, int *arr, int n) {
	for (int i = 0; i < n; i++) {
		if (x == arr[i]) return true;
	}
	return false;
}


Mat ransac(vector<feature_match> matches, feature *f1, feature *f2) {
	Mat homo;
	Mat h;
	feature *feat1;
	feature *feat2;
	int inlier_amount = 0;
	int inlier_max = 0;
	int ran[4] = { 0 };
	int i, j;
	vector<Point2f> src_points;
	vector<Point2f> dst_points;
	vector<Point2f> trans_points;
	Point2f src[4];
	Point2f dst[4];

	for (i = 0; i < matches.size(); i++) {
		feat1 = f1 + matches[i].id1;
		feat2 = f2 + matches[i].id2;
		src_points.push_back(Point2f(feat2->x, feat2->y));
		dst_points.push_back(Point2f(feat1->x, feat1->y));
	}

	srand(time(NULL));
	for (i = 0; i < K; i++) {
		//初始化选取的4组匹配点
		for (j = 0; j < 4; j++) ran[j] = 0;
		for (j = 0; j < 4; j++) {
			ran[j] = rand() % matches.size();
			while (exist(ran[j], ran, j)) ran[j] = rand() % matches.size();
		}

		src[0] = Point2f((f2 + matches[ran[0]].id2)->x, (f2 + matches[ran[0]].id2)->y);
		src[1] = Point2f((f2 + matches[ran[1]].id2)->x, (f2 + matches[ran[1]].id2)->y);
		src[2] = Point2f((f2 + matches[ran[2]].id2)->x, (f2 + matches[ran[2]].id2)->y);
		src[3] = Point2f((f2 + matches[ran[3]].id2)->x, (f2 + matches[ran[3]].id2)->y);

		dst[0] = Point2f((f1 + matches[ran[0]].id1)->x, (f1 + matches[ran[0]].id1)->y);
		dst[1] = Point2f((f1 + matches[ran[1]].id1)->x, (f1 + matches[ran[1]].id1)->y);
		dst[2] = Point2f((f1 + matches[ran[2]].id1)->x, (f1 + matches[ran[2]].id1)->y);
		dst[3] = Point2f((f1 + matches[ran[3]].id1)->x, (f1 + matches[ran[3]].id1)->y);
		
		h = getPerspectiveTransform(src, dst);
		perspectiveTransform(src_points, trans_points, h);

		inlier_amount = 0;
		for (j = 0; j < dst_points.size(); j++) {
			if (fabs(dst_points[j].x - trans_points[j].x) + fabs(dst_points[j].y - trans_points[j].y) < INLIER_THR) {
				inlier_amount++;
			}
		}

		//保留最好的变换矩阵H
		if (inlier_amount > inlier_max) {
			inlier_max = inlier_amount;
			homo = h;
		}
	}

	cout << homo << endl;
	printf("total:%d, inlier: %d\n", matches.size(), inlier_max);

	return homo;
}


Mat blend(Mat img1, Mat img2, Mat h) {
	Mat pano;
	int r = (img1.rows > img2.rows) ? img1.rows : img2.rows;
	warpPerspective(img2, pano, h, Size(img1.cols + img2.cols, r));
	img1.copyTo(Mat(pano, Rect(0, 0, img1.cols, img1.rows)));
	return pano;
}


Mat remove_black(Mat img) {
	int x = 0;

	for (int i = 0; i < img.rows; i++) {
		for (int j = img.cols - 1; j >= 0; j--) {
			if (!(img.at<Vec3b>(i, j)[0] == 0 && img.at<Vec3b>(i, j)[1] == 0 && img.at<Vec3b>(i, j)[2] == 0)) {
				x = (x > j) ? x : j;
				break;
			}
		}
	}


	Mat new_img(img, Rect(0, 0, x, img.rows));
	return new_img;
}


Mat stitch(IplImage **imgs, int size) {
	Mat pano = cvarrToMat(imgs[0]);

	for (int i = 1; i < size; i++) {
		IplImage acc(pano);
		Mat add = cvarrToMat(imgs[i]);

		feature *pano_feat;
		int pano_sift_n = sift_features(&acc, &pano_feat);
		feature *add_feat;
		int add_sift_n = sift_features(imgs[i], &add_feat);

		vector<feature_match> matches = find_match(pano_feat, add_feat, pano_sift_n, add_sift_n);
		Mat h = ransac(matches, pano_feat, add_feat);
		pano = remove_black(blend(pano, add, h));

		imshow("pano", pano);
		waitKey(10000);
	}
	
	return pano;
}

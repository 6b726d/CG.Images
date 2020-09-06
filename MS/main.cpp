#include "segmentation.hpp"
#include "morphology.hpp"

int main() {
	Mat img_in = imread("imgs/sample1.png", IMREAD_GRAYSCALE);
	int rows = img_in.rows;
	int cols = img_in.cols;

	uint totalElements = img_in.total()*img_in.channels();
	Mat mat_img = img_in.reshape(1, totalElements);
	if (!img_in.isContinuous()) {
		mat_img = mat_img.clone();
	}

	vector<uchar> in(mat_img.data, mat_img.data + mat_img.total());
	vector<uchar> out(mat_img.data, mat_img.data + mat_img.total());

	cout << "Rows: " << rows << endl;
	cout << "Cols: " << cols << endl;
	cout << in.size() << endl; // r*c

	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img_in);

	//Morphology Begin
	vector<uchar> inC(mat_img.data, mat_img.data + mat_img.total());
	img_coord c1(1, 0);
	morph_struct s1(c1, { 255, 255, 255 }, 3, 1);
	img_coord c2(1, 1);
	morph_struct s2(c2, { 0,255,0, 255,255,255, 0,255,0 }, 3, 3);
	img_coord c3(0, 1);
	morph_struct s3(c3, { 255,255,255 }, 1, 3);
	img_coord c4(2, 2);
	morph_struct s4(c4, { 0,0,255,0,0, 0,255,255,255,0, 255,255,255,255,255,
	0,255,255,255,0, 0,0,255,0,0 }, 5, 5);
	img_coord c5(1, 0);
	morph_struct s5(c5, { 255,255, 0,255 }, 2, 2);
	//morphology morph;
	//morph.erosion(out, in, rows, cols, s2);
	//morph.dilatation(out, in, rows, cols, s2);
	//morph.opening(out, in, rows, cols, s2);
	//morph.closing(out, in, rows, cols, s2);
	//morph.HitOrMiss(out, in, inC, rows, cols, s2, s3);
	//Morphlogy End

	//Segmentation Begin
	segmentation otsu;
	otsu.threshold(out, in, rows, cols);
	//Segmentation End


	Mat img_inN = Mat(img_in.rows, img_in.cols, img_in.type(), in.data());
	Mat img_inC = Mat(img_in.rows, img_in.cols, img_in.type(), inC.data());

	Mat img_out = Mat(img_in.rows, img_in.cols, img_in.type(), out.data());

	namedWindow("inN", WINDOW_AUTOSIZE);
	imshow("inN", img_inN);

	namedWindow("inC", WINDOW_AUTOSIZE);
	imshow("inC", img_inC);

	namedWindow("output", WINDOW_AUTOSIZE);
	imshow("output", img_out);

	waitKey(0);
	return 0;
}
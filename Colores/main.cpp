#include <iostream>				// cin, cout
#include <opencv2/opencv.hpp>	// image (read, write)
#include <string>
#include <cmath>

#define PI 3.14159265

//normal(rgb) => opencv(bgr)

using namespace std;
using namespace cv;

void RGB_CMY(const Mat& input, Mat& output, int rows, int cols) {
	float r = 0, g = 0, b = 0, c = 0, m = 0, y = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			b = input.at<Vec3b>(i, j)[0];
			g = input.at<Vec3b>(i, j)[1];
			r = input.at<Vec3b>(i, j)[2];

			//bgr=>ymc
			output.at<Vec3b>(i, j)[0] = 255.0-b;
			output.at<Vec3b>(i, j)[1] = 255.0-g;
			output.at<Vec3b>(i, j)[2] = 255.0-r;
		}
	}
}

void RGB_HSV(const Mat& input, Mat& output, int rows, int cols) {
	float r = 0, g = 0, b = 0, h = 0, s = 0, v = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			b = input.at<Vec3b>(i, j)[0];
			g = input.at<Vec3b>(i, j)[1];
			r = input.at<Vec3b>(i, j)[2];

			int max_val = 0;
			int min_val = 0;
			int delta = max_val - min_val;
			max_val = max(b, max(g, r));
			min_val = min(b, min(g, r));

			if (delta > 0) {
				if (max_val == r) {
					h = 60 * fmod(((g - b) / delta), 6);
				}
				else if (max_val == g) {
					h = 60 * (((b - r) / delta) + 2);
				}
				else if (max_val == b) {
					h = 60 * (((r - g) / delta) + 4);
				}

				if (max_val > 0) {
					s = delta / max_val;
				}
				else {
					s = 0;
				}

				v = max_val;
			}
			else {
				h = 0;
				s = 0;
				v = max_val;
			}

			if (h < 0) {
				h = 360 + h;
			}

			//bgr=>ish
			output.at<Vec3b>(i, j)[0] = v;
			output.at<Vec3b>(i, j)[1] = s;
			output.at<Vec3b>(i, j)[2] = h;
		}
	}
}

void RGB_HSI(const Mat& input, Mat& output, int rows, int cols) {
	float r=0, g=0, b=0, h=0, s=0, i=0;
	float nr = 0, ng = 0, nb = 0;
	float n = 0;
	float a = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			b = input.at<Vec3b>(i, j)[0];
			g = input.at<Vec3b>(i, j)[1];
			r = input.at<Vec3b>(i, j)[2];

			i = (b + g + r) / (3*255.0);
			n = sqrt((r*r) + (g*g) + (b*b));
			nr = r / n;
			ng = g / n;
			nb = b / n;

			if ((r + g + b) == 765) {
				s = 0;
				h = 0;
			}
			else {
				int min_val = min(nr, min(ng, nb));
				s = 1 - 3 * (min_val / (nr + ng + nb));
				if (s < 0.5) {
					s = 0;
				}
				else {
					s = 1;
				}
			}

			if (s != 0) {
				a = 0.5 * ((nr - ng) + (nr - nb)) / sqrt(((nr - ng) * (nr - ng)) + (nr - nb)*(ng - nb));
				a = acos(a);
				if (b <= g) {
					h = a;
				}
				else {
					h = 2 * PI - a;
				}
			}

			i = i * 255;
			s = s * 100;
			h = (h * 180) / PI;

			if ((r == 0) && (g == 0) && (b == 0)) {
				h = 0;
				s = 0;
				i = 0;
			}

			//bgr=>ish
			output.at<Vec3b>(i, j)[0] = i*255;
			output.at<Vec3b>(i, j)[1] = s*100;
			output.at<Vec3b>(i, j)[2] = (h * 180) / PI;
		}
	}
}

void RGB_YCbCr(const Mat& input, Mat& output, int rows, int cols) {
	float r = 0, g = 0, b = 0, y = 0, cb = 0, cr = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			b = input.at<Vec3b>(i, j)[0];
			g = input.at<Vec3b>(i, j)[1];
			r = input.at<Vec3b>(i, j)[2];

			y = (0.299*r) + (0.587*g) + (0.114*b);
			cb = 128 + (-0.169*r) + (-0.331*g) + (0.5*b);
			cr = 128 + (0.5*r) + (-0.419*g) + (-0.081*b);

			//bgr=>ymc
			output.at<Vec3b>(i, j)[0] = cr;
			output.at<Vec3b>(i, j)[1] = cb;
			output.at<Vec3b>(i, j)[2] = y;
		}
	}
}

int main() {
	Mat src = imread("lenna.jpg",1);
	if (src.empty()) {
		cerr << "Error: Loading image" << endl;
	}

	int rows = src.rows;
	int cols = src.cols;

	Mat mod(src.rows, src.cols, src.type());

	string type = "";
	int op = 0;
	cout << "Insert Option" << endl;
	cout << "1.CMY 2.HSV 3.HSI" << endl;
	cout << "4.YMQ 5.YMI 6.YCbCr" << endl;
	cin >> op;
	if (op == 1) {
		type = "CMY";
		RGB_CMY(src, mod, rows, cols);
	}
	else if (op == 2) {
		type = "HSV";
		RGB_HSV(src, mod, rows, cols);
	}
	else if (op == 3) {
		type = "HSI";
		RGB_HSI(src, mod, rows, cols);
	}
	else if (op == 4) {
		type = "YMQ";
		//Not found YMQ Color
		//RGB_YMQ(src, mod, rows, cols);
		cerr << "YMQ color not found" << endl;
	}
	else if (op == 5) {
		type = "YMI";
		//Not found YMI Color
		//RGB_YMI(src, mod, rows, cols);
		cerr << "YMI color not found" << endl;
	}
	else if (op == 6) {
		type = "YCbCr";
		RGB_YCbCr(src, mod, rows, cols);
	}
	else {
		cerr << "Invalid Option" << endl;
	}

	cout << "END" << endl;

	string window_name = type + " Image";
	namedWindow("RGB Image", WINDOW_AUTOSIZE);
	namedWindow(window_name, WINDOW_AUTOSIZE);

	imshow("RGB Image", src);
	imshow(window_name, mod);

	waitKey(0);
	return 0;
}

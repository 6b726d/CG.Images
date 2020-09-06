#include <iostream>				// cin, cout
#include <algorithm>			// sort
#include <opencv2/opencv.hpp>	// image (read, write)

using namespace std;
using namespace cv;

//IMREAD_GRAYSCALE
//IMREAD_COLOR (BGR)

void Media(vector<uchar> &out, vector<uchar> in, int rows, int cols, int n) {
	int s = (n - 1) / 2;	// n = (2*s)+1
	int cOffset = 0;		// CurrentOffset
	int pixVal = 0;			// PixelValue
	int offset = 0;			// Offset
	int pixels = 0;			// Pixels
	int cr = 0;				// CurrentRow
	int cc = 0;				// CurrentColumn
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((i < rows) && (j < cols)) {
				pixVal = 0;
				pixels = 0;
				for (int r = -s; r < s; r++) {
					for (int c = -s; c < s; c++) {
						cr = i + r;
						cc = j + c;
						if ((cr > -1) && (cr < rows) && (cc > -1) && (cc < cols)) {
							cOffset = cr * rows + cc;
							pixVal += in[cOffset];
							pixels++;
						}
					}
				}
				offset = i * rows + j;
				out[offset] = (uchar) (pixVal / pixels);
			}
		}
	}
}

void Mediana(vector<uchar> &out, vector<uchar> in, int rows, int cols, int n) {
	int s = (n - 1) / 2;	// n = (2*s)+1
	int cOffset = 0;		// CurrentOffset
	int offset = 0;			// Offset
	int cr = 0;				// CurrentRow
	int cc = 0;				// CurrentColumn
	int me = 0;				// Media
	vector<uchar> v(n*n);	// Values
	int vr = 0;				// VectorRow
	int vc = 0;				// VectorColumn
	int p = 0;				// Pixels
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((i < rows) && (j < cols)) {
				p = 0;
				for (int r = -s; r < s; r++) {
					for (int c = -s; c < s; c++) {
						cr = i + r;
						cc = j + c;
						vr = r + s;
						vc = c + s;
						if ((cr > -1) && (cr < rows) && (cc > -1) && (cc < cols)) {
							cOffset = cr * rows + cc;
							v[vr*n + vc] = in[cOffset];
							p++;
						}
					}
				}
				me = 0;
				sort(v.begin(),v.end());
				if ((p%2) == 0) {
					me = (v[(p/2)-1] + v[p/2]) / 2;
				}
				else {
					me = v[(p-1)/2];
				}
				offset = i * rows + j;
				out[offset] = (uchar)(me);
			}
		}
	}
}

void Max(vector<uchar> &out, vector<uchar> in, int rows, int cols, int n) {
	int s = (n - 1) / 2;	// n = (2*s)+1
	int cOffset = 0;		// CurrentOffset
	int offset = 0;			// Offset
	int cr = 0;				// CurrentRow
	int cc = 0;				// CurrentColumn
	int vMax = 0;			// MaxValue
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((i < rows) && (j < cols)) {
				vMax = -1;
				for (int r = -s; r < s; r++) {
					for (int c = -s; c < s; c++) {
						cr = i + r;
						cc = j + c;
						if ((cr > -1) && (cr < rows) && (cc > -1) && (cc < cols)) {
							cOffset = cr * rows + cc;
							if (in[cOffset] > vMax) {
								vMax = in[cOffset];
							}
						}
					}
				}
				offset = i * rows + j;
				out[offset] = (uchar)(vMax);
			}
		}
	}
}

void Min(vector<uchar> &out, vector<uchar> in, int rows, int cols, int n) {
	int s = (n - 1) / 2;	// n = (2*s)+1
	int cOffset = 0;		// CurrentOffset
	int offset = 0;			// Offset
	int cr = 0;				// CurrentRow
	int cc = 0;				// CurrentColumn
	int vMin = 0;			// MaxValue
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((i < rows) && (j < cols)) {
				vMin = 256;
				for (int r = -s; r < s; r++) {
					for (int c = -s; c < s; c++) {
						cr = i + r;
						cc = j + c;
						if ((cr > -1) && (cr < rows) && (cc > -1) && (cc < cols)) {
							cOffset = cr * rows + cc;
							if (in[cOffset] < vMin) {
								vMin = in[cOffset];
							}
						}
					}
				}
				offset = i * rows + j;
				out[offset] = (uchar)(vMin);
			}
		}
	}
}

void Conv(vector<uchar> &out, vector<uchar> in, int rows, int cols, vector<int> mtx, int n) {
	int s = (n - 1) / 2;	// n = (2*s)+1
	int cOffset = 0;		// CurrentOffset
	int pixVal = 0;			// PixelValue
	int offset = 0;			// Offset
	int cr = 0;				// CurrentRow
	int cc = 0;				// CurrentColumn
	int mr = 0;				// MatrixRow
	int mc = 0;				// MatrixColumn
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((i < rows) && (j < cols)) {
				pixVal = 0;
				for (int r = -s; r < s; r++) {
					for (int c = -s; c < s; c++) {
						cr = i + r;
						cc = j + c;
						mr = r + s;
						mc = c + s;
						if ((cr > -1) && (cr < rows) && (cc > -1) && (cc < cols)) {
							cOffset = cr * rows + cc;
							pixVal += (mtx[mr*n + mc] * (in[cOffset]));
						}
					}
				}
				offset = i * rows + j;
				out[offset] = (uchar)(pixVal);
			}
		}
	}
}

void ConvXY(vector<uchar> &out, vector<uchar> inX, vector<uchar> inY, int rows, int cols) {
	int offset = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			offset = i * rows + j;
			out[offset] = inX[offset] + inY[offset];
		}
	}
}

int main()
{
	Mat img_in = imread("lenaS.jpg", IMREAD_GRAYSCALE);
	int rows = img_in.rows;
	int cols = img_in.cols;
	
	uint totalElements = img_in.total()*img_in.channels();
	Mat mat_img = img_in.reshape(1, totalElements);
	if (!img_in.isContinuous()) {
		mat_img = mat_img.clone();
	}

	vector<uchar> in(mat_img.data, mat_img.data + mat_img.total());
	vector<uchar> inX(mat_img.data, mat_img.data + mat_img.total());
	vector<uchar> inY(mat_img.data, mat_img.data + mat_img.total());
	vector<uchar> out(mat_img.data, mat_img.data + mat_img.total());

	cout << "Rows: " << rows << endl;
	cout << "Cols: " << cols << endl;
	cout << in.size() << endl; // r*c

	Media(inX, in, rows, cols, 5);
	vector<int> mtx = { 1,1,1,1,-8,1,1,1,1 };
	Conv(out, inX, rows, cols, mtx, 3);


	/*
	int op = 0;
	cout << "Select Option" << endl;
	cout << "1.Media 2.Mediana 3.Max 4.Min" << endl;
	cout << "5.Gauss273 6.Gauss115" << endl;
	cout << "7.Laplace-4 8.Laplace-8 9.Laplace4 10.Laplace8" << endl;
	cout << "11.Roberts 12.Sobel 13.Prewitt" << endl;
	cin >> op;
	if (op == 1) {
		Media(out, in, rows, cols, 3);
	}
	else if (op == 2) {
		Mediana(out, in, rows, cols, 3);
	}
	else if (op == 3) {
		Max(out, in, rows, cols, 3);
	}
	else if (op == 4) {
		Min(out, in, rows, cols, 3);
	}
	else if (op == 5) {
		vector<int> mtx = { 1,4,7,4,1,4,16,26,16,4,7,25,41,26,7,
		4,16,26,16,4,1,4,7,4,1 };
		Conv(out, in, rows, cols, mtx, 5);
	}
	else if (op == 6) {
		vector<int> mtx = { 2,4,5,4,2,4,9,12,9,4,5,12,15,12,5,
		4,9,12,9,4,2,4,5,4,2 };
		Conv(out, in, rows, cols, mtx, 5);
	}
	else if (op == 7) {
		vector<int> mtx = { 0,1,0,1,-4,1,0,1,0 };
		Conv(out, in, rows, cols, mtx, 3);
	}
	else if (op == 8) {
		vector<int> mtx = { 1,1,1,1,-8,1,1,1,1 };
		Conv(out, in, rows, cols, mtx, 3);
	}
	else if (op == 9) {
		vector<int> mtx = { 0,-1,0,-1,4,-1,0,-1,0 };
		Conv(out, in, rows, cols, mtx, 3);
	}
	else if (op == 10) {
		vector<int> mtx = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
		Conv(out, in, rows, cols, mtx, 3);
	}
	else if (op == 11) {
		vector<int> mtx_x = { -1,0,0,0,1,0,0,0,0 };
		vector<int> mtx_y = { 0,-1,0,1,0,0,0,0,0 };
		Conv(inX, in, rows, cols, mtx_x, 3);
		Conv(inY, in, rows, cols, mtx_y, 3);
		ConvXY(out, inX, inY, rows, cols);
	}
	else if (op == 12) {
		vector<int> mtx_x = { -1,-2,-1,0,0,0,1,2,1 };
		vector<int> mtx_y = { -1,0,1,-2,0,2,-1,0,1 };
		Conv(inX, in, rows, cols, mtx_x, 3);
		Conv(inY, in, rows, cols, mtx_y, 3);
		ConvXY(out, inX, inY, rows, cols);
	}
	else if (op == 13) {
		vector<int> mtx_x = { -1,0,1,-1,0,1,-1,0,1 };
		vector<int> mtx_y = { -1,-1,-1,0,0,0,1,1,1 };
		Conv(inX, in, rows, cols, mtx_x, 3);
		Conv(inY, in, rows, cols, mtx_y, 3);
		ConvXY(out, inX, inY, rows, cols);
	}
	else {
		cout << "Invalid Operation" << endl;
	}
	*/
	Mat img_out = Mat(img_in.rows, img_in.cols, img_in.type(), out.data());


	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img_in);

	namedWindow("output", WINDOW_AUTOSIZE);
	imshow("output", img_out);

	waitKey(0);
	return 0;
}

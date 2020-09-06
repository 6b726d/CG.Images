#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

struct img_coord {
	int r;
	int c;
	img_coord();
	img_coord(int _r, int _c);
};

struct morph_struct {
	img_coord origin;
	vector<int> matrix;
	int rows;
	int cols;
	morph_struct(img_coord _origin, vector<int> _matrix, int _rows, int _cols);
};

class morphology {
public:
	void clear(vector<uchar>& out, int rows, int cols);
	void complement(vector<uchar>& out, vector<uchar>& in, int rows, int cols);
	void intersection(vector<uchar>& out, vector<uchar>& in1, vector<uchar>& in2, int rows, int cols);
	void erosion(vector<uchar>& out, vector<uchar>& in, int rows, int cols, morph_struct s);
	void dilatation(vector<uchar>& out, vector<uchar>& in, int rows, int cols, morph_struct s);
	void opening(vector<uchar>& out, vector<uchar>& in, int rows, int cols, morph_struct s);
	void closing(vector<uchar>& out, vector<uchar>& in, int rows, int cols, morph_struct s);
	void HitOrMiss(vector<uchar>& out, vector<uchar>& in1, vector<uchar>& in2, int rows, int cols, morph_struct s1, morph_struct s2);
};
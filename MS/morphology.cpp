#include "morphology.hpp"

img_coord::img_coord() {
	r = 0;
	c = 0;
}

img_coord::img_coord(int _r, int _c) {
	r = _r;
	c = _c;
}

morph_struct::morph_struct(img_coord _origin, vector<int> _matrix, int _rows, int _cols) {
	origin = _origin;
	matrix = _matrix;
	rows = _rows;
	cols = _cols;
}

void morphology::clear(vector<uchar>& out, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			out[i*cols + j] = 0;
		}
	}
}

void morphology::complement(vector<uchar>& out, vector<uchar>& in, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			out[i*cols + j] = 255 - in[i*cols + j];
		}
	}
}

void morphology::intersection(vector<uchar>& out, vector<uchar>& in1, vector<uchar>& in2, int rows, int cols) {
	clear(out, rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			//if ((in1[i*cols + j] == 0) && (in2[i*cols + j] == 0))
			if (in1[i*cols + j] == in2[i*cols + j]) {
				out[i*cols + j] = 255;
			}
		}
	}
}

void morphology::erosion(vector<uchar>& out, vector<uchar>& in, int rows, int cols, morph_struct s) {
	clear(out, rows, cols);
	bool inclusion = true;	//inclusion
	int ci = 0;				//current i
	int cj = 0;				//current j
	int ni = 0;				//new i
	int nj = 0;				//new j
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			inclusion = true;
			for (int r = 0; r < s.rows; r++) {
				for (int c = 0; c < s.cols; c++) {
					ci = i + r;
					cj = j + c;
					if ((ci > -1) && (ci < rows) && (cj > -1) && (cj < cols)) {
						if (s.matrix[r*s.cols + c] == 255) {
						//if (in[ci*cols + cj] == 0) {
							if (in[ci*cols + cj] != s.matrix[r*s.cols + c]) {
								inclusion = false;
							}
						}
					}
					else {
						inclusion = false;
					}
				}
			}
			if (inclusion == true) {
				ni = i + s.origin.r;
				nj = j + s.origin.c;
				if ((ni > -1) && (ni < rows) && (nj > -1) && (nj < cols)) {
					out[ni*cols + nj] = 255;
				}
			}
		}
	}
}

void morphology::dilatation(vector<uchar>& out, vector<uchar>& in, int rows, int cols, morph_struct s) {
	clear(out, rows, cols);
	bool inclusion = false;	//inclusion
	int ci = 0;				//current i
	int cj = 0;				//current j
	int ni = 0;				//new i
	int nj = 0;				//new j
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			inclusion = false;
			if (in[i*cols + j] == 255) {
				inclusion = true;
			}
			if (inclusion == true) {
				ni = i - s.origin.r;
				nj = j - s.origin.c;
				for (int r = 0; r < s.rows; r++) {
					for (int c = 0; c < s.cols; c++) {
						ci = ni + r;
						cj = nj + c;
						if ((ci > -1) && (ci < rows) && (cj > -1) && (cj < cols)) {
							if (s.matrix[r*s.cols + c] == 255) {
								out[ci*cols + cj] = 255;
							}
						}
					}
				}
			}
		}
	}
}

void morphology::opening(vector<uchar>& out, vector<uchar>& in, int rows, int cols, morph_struct s) {
	erosion(out, in, rows, cols, s);
	in = out;
	dilatation(out, in, rows, cols, s);
}

void morphology::closing(vector<uchar>& out, vector<uchar>& in, int rows, int cols, morph_struct s) {
	dilatation(out, in, rows, cols, s);
	in = out;
	erosion(out, in, rows, cols, s);
}

void morphology::HitOrMiss(vector<uchar>& out, vector<uchar>& in1, vector<uchar>& in2, int rows, int cols, morph_struct s1, morph_struct s2) {
	complement(in2, in1, rows, cols);
	erosion(out, in1, rows, cols, s1);
	in1 = out;
	erosion(out, in2, rows, cols, s2);
	in2 = out;
	intersection(out, in1, in2, rows, cols);
}

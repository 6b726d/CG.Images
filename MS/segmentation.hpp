#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class segmentation {
private:
	float histogram[256];
	float probability[256];
public:
	segmentation();
	void threshold(vector<uchar>& out, vector<uchar>& in, int rows, int cols);
};

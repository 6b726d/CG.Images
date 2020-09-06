#include "segmentation.hpp"

segmentation::segmentation() {
	for (int pos = 0; pos < 256; pos++) {
		histogram[pos] = 0;
		probability[pos] = 0;
	}
}

void segmentation::threshold(vector<uchar>& out, vector<uchar>& in, int rows, int cols) {
	float p = 0;
	float q = 0;
	float q_prev = 0;
	float q_next = 0;
	float mean1 = 0;
	float mean2 = 0;
	float m1_next = 0;
	float m2_next = 0;
	float mean = 0;
	float variance = 0;
	float max_variance = 0;
	float sum = 0;
	float v_threshold = 0;
	int pix_val = 0;

	//Histrogram
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			pix_val = in[i*rows + j];
			sum += pix_val;
			histogram[pix_val]++;
		}
	}

	//Probability
	for (int i = 0; i < 256; i++) {
		probability[i] = histogram[i] / (rows*cols);
	}

	//Threshold (Otsu)
	p = probability[0];
	q = p;
	mean = sum / (rows*cols);

	q_prev = q;
	for (int i = 0; i < 256; i++) {
		q_next = q_prev + probability[i + 1];
		m1_next = (q_prev*mean1 + (i + 1)*probability[i + 1]) / q_next;
		m2_next = (mean - q_next * m1_next) / (1 - q_next);
		variance = q_prev * (1 - q_prev)*((mean1 - mean2)*(mean1 - mean2));
		if (variance > max_variance) {
			max_variance = variance;
			v_threshold = i;
		}
		q_prev = q_next;
		mean1 = m1_next;
		mean2 = m2_next;
		if (q_next == 0) {
			mean1 = 0;
		}
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			pix_val = in[i*rows + j];
			if (pix_val < v_threshold) {
				pix_val = 0;
				out[i*rows + j] = pix_val;
			}
			else {
				pix_val = 255;
				out[i*rows + j] = pix_val;
			}
		}
	}
}

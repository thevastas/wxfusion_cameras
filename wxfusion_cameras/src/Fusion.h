#pragma once
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <stdio.h>
class Fusion
{
private:
	int m_fused_columns;
	int m_fused_rows;
	int m_nirx1;
	int m_nirx2;
	int m_niry1;
	int m_niry2;
	int m_lwirx1;
	int m_lwirx2;
	int m_lwiry1;
	int m_lwiry2;
	cv::Mat m_nir_roi;
	cv::Mat m_lwir_roi;
	cv::Mat m_nir_poi;
	cv::Mat m_lwir_poi;
public:
	int m_offsety;
	int m_offsetx;
	double m_ratio;
	bool m_palette;
	cv::Mat m_fused_img;
	cv::Mat fuse_offset(cv::Mat nir_img, cv::Mat lwir_img);
	void init(cv::Mat nir_img, cv::Mat lwir_img, int offsetx, int offsety, double ratio, bool palette);
};


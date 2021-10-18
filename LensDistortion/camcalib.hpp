#pragma once

#include "io.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/hal/interface.h>


void convertToGray(std::vector<cv::Mat>& src, std::vector<cv::Mat>& dest, const std::string& outFolder);
cv::Mat applyEdgeDetection(cv::Mat& src, int dx, int dy);
cv::Mat applySobel(cv::Mat& src, int dx, int dy);
cv::Mat applyScharr(cv::Mat& src, int dx, int dy);
void calibrateCamera(std::vector<cv::Mat>& src, cv::Mat outParams[], int cbCornerRows, int cbCornerCols, const std::string& outFolder);
void refineCamera(int imgRows, int imgCols, cv::Mat camMatrix, cv::Mat distCoeffs, int typeUndistort, cv::Mat& image, std::string outFolder);

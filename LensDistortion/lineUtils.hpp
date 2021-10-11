#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <stack>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#include <algorithm>
#include <chrono>
#include <thread>

void calculateNeighbors(const cv::Point origin, std::vector<cv::Point>& directions);

enum class Direction
{
	horizontal,
	vertical
};

std::vector<cv::Point> getLine(const cv::Mat& img, cv::Point origin, float dirChangeThreshold);

std::vector<std::tuple<int,int,cv::Point>> findPossibleLineLocation(const cv::Mat& img, int length, const std::string outputName);

void extractLines(const cv::Mat& img , std::vector<std::tuple<int, int, cv::Point>> candidateAreas);

void paintLines(const cv::Mat& img, std::vector<std::vector<cv::Point>> lines);

void applyThinning(const cv::Mat& img);
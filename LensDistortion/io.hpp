#pragma once

#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>


bool getFile(const std::string& dir_path, cv::Mat& outputImage);
void getFiles(const std::string& dir_path, std::vector<cv::Mat>& outArray);

void handleOutFolder(const std::string& dir_path);



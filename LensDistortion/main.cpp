// LensDistortion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "io.hpp"
#include "camcalib.hpp"
#include "lineUtils.hpp"

using namespace std;

namespace fs = std::filesystem;


int main(int argc, char** argv)
{
    if (argc != 5)
    {
        string usage = "Usage: lensdistortion [calib input dir] [calib output dir] [threshold] [image]";
        std::cout << usage;
        return EXIT_FAILURE;
    }

    const string inDirPath = argv[1];       //calibration images
    const string outDirPath = argv[2];      //output calibration images
    const double threshold = atof(argv[3]); //threshold
    const string imagePath = argv[4];       //image to undistort

    //get image to undistort
    cv::Mat image;
    bool success = getFile(imagePath, image);


    //make sure output folder exists
    handleOutFolder(outDirPath);

    //get loaded images
    vector<cv::Mat> checkboards;
    getFiles(inDirPath, checkboards);

    //convert to grayscale
    std::vector<cv::Mat> grayCheckboards;
    convertToGray(checkboards, grayCheckboards, outDirPath);

    //make binary images
    vector<cv::Mat> binaryCheckboards;
    for (int i{ 0 }; i < grayCheckboards.size(); i++)
    {
        ostringstream blurredOutput;
        blurredOutput << outDirPath << "\\blurred\\blurred" << i << ".png";

        std::cout << "Creating blurred image nr: " << i << endl;
        cv::Mat blurredImg;
        //cv::GaussianBlur(src, blurredImg, cv::Size(5,5), 0, cv::BORDER_DEFAULT );
        cv::bilateralFilter(grayCheckboards[i], blurredImg, 8, 75, 75);
        cv::imwrite(blurredOutput.str(), blurredImg);

        std::cout << "Creating binary image nr: " << i << " with threshold:" << threshold << endl;

        ostringstream binaryOutput;
        binaryOutput << outDirPath << "\\binary\\binary" << i << ".png";
        cv::Mat tImage;
        cv::threshold(blurredImg, tImage, threshold, 255, cv::THRESH_BINARY);
        blurredImg.release();

        binaryCheckboards.push_back(tImage);
        cv::imwrite(binaryOutput.str(), tImage);
        tImage.release();
    }

    // [0] CameraMatrix
    // [1] Distance Coefficients
    // [2] Rotation Vector
    // [3] Translation Vector
    cv::Mat calibParams[4];

    calibrateCamera(binaryCheckboards, calibParams, 6, 8, outDirPath);

    std::cout << "Camera Matrix: " << calibParams[0] << endl;
    std::cout << "Distance Coefficients: " << calibParams[1] << endl;
    std::cout << "Rotation Vector: " << calibParams[2] << endl;
    std::cout << "Translation Vector: " << calibParams[3] << endl;

    refineCamera(image.rows, image.cols, calibParams[0], calibParams[1], 1, image, outDirPath);
    //refineCamera(image.rows, image.cols, calibParams[0], calibParams[1], 2, image);

    cout << "Calibration complete!" << endl;

    return EXIT_SUCCESS;
}



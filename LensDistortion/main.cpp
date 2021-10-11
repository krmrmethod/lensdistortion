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
        string usage = "Usage: lensdistortion [calib input dir] [calib output dir] [threshold] [src image]";
        std::cout << usage;
        return EXIT_FAILURE;
    }

    vector<cv::Mat> edgeImgs;
    cv::Mat edgeX;
    cv::Mat edgeY;
    bool noSkip = false;

    //getFile("Z:\\sandbox\\LensDistortion\\output\\edges\\edge_atry.png", edgeTry);
    //getFile("Z:\\sandbox\\LensDistortion\\output\\edges\\edges{0}.png", edgeTry);
    //getFiles("Z:\\sandbox\\LensDistortion\\output\\edges", edgeImgs);
    //getFiles("Z:\\sandbox\\LensDistortion\\output\\sobel\\test", edgeImgs);
    getFile("Z:\\sandbox\\LensDistortion\\output\\sobel\\test\\sobel2{0x}.png", edgeX);
    getFile("Z:\\sandbox\\LensDistortion\\output\\sobel\\test\\sobel2{0y}.png", edgeY);

    if (noSkip)
    {

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
        convertToGray(checkboards, grayCheckboards);

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


        std::cout << "Generating edge images: " << endl;


        //vector<cv::Mat> edgeImgs;
        for (int i = 0; i < binaryCheckboards.size(); i++)
        {
            ostringstream edgeOutput;

            cv::Mat edgeImg(applyEdgeDetection(binaryCheckboards[i], 75, 95));
            //edgeImgs.push_back(edgeImg);

            edgeOutput << outDirPath << "\\edges\\edges{" << i << "}.png";
            cout << "Canny algorithm executed on image " << i << endl;
            edgeImgs.push_back(edgeImg);
            cv::imwrite(edgeOutput.str(), edgeImg);
            edgeOutput.str("");
            edgeOutput.clear();


            //edgeOutput << outDirPath << "\\sobel\\sobel{" << i << "x" << "}.png";
            //edgeImg = cv::Mat(applySobel(binaryCheckboards[i], 1, 0));
            //cout << "Sobel algorithm executed on image " << i << " axis: x" << endl;
            //edgeImgs.push_back(edgeImg);
            //cv::imwrite(edgeOutput.str(), edgeImg);
            //edgeOutput.str("");
            //edgeOutput.clear();

            //edgeOutput << outDirPath << "\\scharr\\scharr{" << i << "x" << "}.png";
            //edgeImg = cv::Mat(applyScharr(binaryCheckboards[i], 1, 0));
            //cout << "Scharr algorithm executed on image " << i << " axis: x" << endl;
            //cv::imwrite(edgeOutput.str(), edgeImg);
            //edgeOutput.str("");
            //edgeOutput.clear();

            //edgeOutput << outDirPath << "\\sobel\\sobel-2nd{" << i << "x" << "}.png";
            //edgeImg = cv::Mat(applySobel(binaryCheckboards[i], 2, 0));
            //cout << "Sobel algorithm executed on image " << i << " axis: x" << endl;
            //cv::imwrite(edgeOutput.str(), edgeImg);
            //edgeOutput.str("");
            //edgeOutput.clear();

            //edgeOutput << outDirPath << "\\sobel\\sobel-3rd{" << i << "x" << "}.png";
            //edgeImg = cv::Mat(applySobel(binaryCheckboards[i], 3, 0));
            //cout << "Sobel algorithm executed on image " << i << " axis: x" << endl;
            //cv::imwrite(edgeOutput.str(), edgeImg);
            //edgeOutput.str("");
            //edgeOutput.clear();

            //edgeOutput << outDirPath << "\\sobel\\sobel{" << i << "y" << "}.png";
            //edgeImg = cv::Mat(applySobel(binaryCheckboards[i], 0, 1));
            //cout << "Sobel algorithm executed on image " << i << " axis: y" << endl;
            //cv::imwrite(edgeOutput.str(), edgeImg);
            //edgeOutput.str("");
            //edgeOutput.clear();

            //edgeOutput << outDirPath << "\\scharr\\scharr{" << i << "y" << "}.png";
            //edgeImg = cv::Mat(applyScharr(binaryCheckboards[i], 0, 1));
            //cout << "Scharr algorithm executed on image " << i << " axis: x" << endl;
            //cv::imwrite(edgeOutput.str(), edgeImg);
            //edgeOutput.str("");
            //edgeOutput.clear();

            //edgeOutput << outDirPath << "\\sobel\\sobel-2nd{" << i << "y" << "}.png";
            //edgeImg = cv::Mat(applySobel(binaryCheckboards[i], 0, 2));
            //cout << "Sobel algorithm executed on image " << i << " axis: y" << endl;
            //cv::imwrite(edgeOutput.str(), edgeImg);
            //edgeOutput.str("");
            //edgeOutput.clear();

            //edgeOutput << outDirPath << "\\sobel\\sobel-3rd{" << i << "y" << "}.png";
            //edgeImg = cv::Mat(applySobel(binaryCheckboards[i], 0, 3));
            //cout << "Sobel algorithm executed on image " << i << " axis: y" << endl;
            //cv::imwrite(edgeOutput.str(), edgeImg);

            edgeImg.release();
        }

    }

    //findPossibleLineLocation(edgeY, 50);

    //cv::rotate(edgeX, edgeX, cv::ROTATE_90_CLOCKWISE);


    //cv::imwrite("Z:\\sandbox\\LensDistortion\\output\\sobel\\test\\sobel_rotated{0x}.png", edgeX);

    vector<tuple<int, int, cv::Point>> candidateAreas;

    //findPossibleLineLocation(edgeX, 50, "edgeX_candidates.png");
    candidateAreas = findPossibleLineLocation(edgeY, 50, "edgeY_candidates.png");


    //for (cv::Mat img : edgeImgs)
    //{
    //    findPossibleLineLocation(img, 100);
    //}


    // [0] CameraMatrix
    // [1] Distance Coefficients
    // [2] Rotation Vector
    // [3] Translation Vector
    //cv::Mat calibParams[4];

    //calibrateCamera(binaryCheckboards, calibParams, 6, 8);

    //std::cout << "Camera Matrix: " << calibParams[0] << endl;
    //std::cout << "Distance Coefficients: " << calibParams[1] << endl;
    //std::cout << "Rotation Vector: " << calibParams[2] << endl;
    //std::cout << "Translation Vector: " << calibParams[3] << endl;

    //refineCamera(image.rows, image.cols, calibParams[0], calibParams[1], 1, image);
    //refineCamera(image.rows, image.cols, calibParams[0], calibParams[1], 2, image);


    return EXIT_SUCCESS;
}



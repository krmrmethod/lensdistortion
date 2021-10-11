#include "camcalib.hpp"

using namespace std;

cv::Mat applyEdgeDetection(cv::Mat& src, int low_t, int high_t)
{
	cv::Mat edges;
	cv::Canny(src, edges, low_t, high_t);

	return edges;
}

cv::Mat applySobel(cv::Mat& src, int dx, int dy)
{
	cv::Mat edges;
	cv::Sobel(src, edges, CV_8U, dx, dy, 3, 1.0, 0.5);

	return edges;
}

cv::Mat applyScharr(cv::Mat& src, int dx, int dy)
{
	cv::Mat edges;
	cv::Scharr(src, edges, CV_8U, dx, dy);

	return edges;
}

void convertToGray(vector<cv::Mat>& src, vector<cv::Mat>& dest) 
{
	for (int i{ 0 }; i < src.size(); i++)
	{
		cv::Mat frame;
		cout << "Processing image nr: " << i << endl;

		cv::cvtColor(src[i], frame, cv::COLOR_BGR2GRAY);

		dest.push_back(frame);

		ostringstream grayoutput;
		grayoutput << "z:\\sandbox\\lensdistortion\\output\\grayscale\\grayscale(" << i << ").png";
		cv::imwrite(grayoutput.str(), frame);

		cout << "Succesful grayscale for image nr: " << i << endl;
		frame.release();
	}
}


void calibrateCamera(vector<cv::Mat>& src, cv::Mat outParams[], int cbCornerRows, int cbCornerCols)
{
	cout << "Starting calibration" << endl;
	
	vector<vector<cv::Point3f>> worldPoints;
	vector<vector<cv::Point2f>> imgPoints;

	vector<cv::Point3f> worldPoint;
	for (int i{ 0 }; i < cbCornerCols; i++)
	{
		for (int j{ 0 }; j < cbCornerRows; j++)
		{
			worldPoint.push_back(cv::Point3f(j, i, 0));
		}
	}

	cv::Mat frame;
	for (int i{ 0 }; i < src.size(); i++)
	{
		frame = src[i];
		vector<cv::Point2f> corner_pts;

		bool success = cv::findChessboardCorners(frame, cv::Size(cbCornerRows, cbCornerCols), corner_pts,
			cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

		if (!success)
		{
			cout << "Checkboard not recognized" << endl;
		}
		else
		{
			cout << "Found checkboard in image nr: " << i << endl;

			cv::TermCriteria criteria(cv::TermCriteria::Type::EPS | cv::TermCriteria::Type::MAX_ITER, 30, 0.001);

			cv::cornerSubPix(frame, corner_pts, cv::Size(11, 11), cv::Size(-1, -1), criteria);

			cout << "Enhancing corner positions in image nr: " << i << endl;

			cv::Mat colorCB;
			cv::cvtColor(frame, colorCB, cv::COLOR_GRAY2BGR);

			cv::drawChessboardCorners(colorCB, cv::Size(cbCornerRows, cbCornerCols), corner_pts, success);

			cout << "Drawing on top of checkboard in image nr: " << i << endl;

			worldPoints.push_back(worldPoint);
			imgPoints.push_back(corner_pts);

			ostringstream output;
			output << "z:\\sandbox\\lensdistortion\\output\\chessboard\\chessboard(" << i << ").png";

			cout << "Writing image nr: " << i << endl;
			cv::imwrite(output.str(), colorCB);
			colorCB.release();
		}
	}

	cout << "Finished processing checkboard images" << endl;

	cv::calibrateCamera(worldPoints, imgPoints, cv::Size(frame.rows, frame.cols),
		outParams[0], outParams[1], outParams[2], outParams[3]);

	cout << "Finalized calibration." << endl;
}

void refineCamera(int imgRows, int imgCols, cv::Mat camMatrix, cv::Mat distCoeffs, int typeUndistort, cv::Mat& image)
{
	cv::Mat dst, map1, map2, newCamMatrix;
	cv::Size imageSize(cv::Size(imgCols, imgRows));

	newCamMatrix = cv::getOptimalNewCameraMatrix(camMatrix, distCoeffs, imageSize, 1, imageSize, 0);

	if (typeUndistort == 1)
	{
		cv::undistort(image, dst, newCamMatrix, distCoeffs, newCamMatrix);
	}
	else if (typeUndistort == 2)
	{
		cv::initUndistortRectifyMap(camMatrix, distCoeffs, cv::Mat(),
			cv::getOptimalNewCameraMatrix(camMatrix, distCoeffs, imageSize, 1, imageSize, 0), imageSize, CV_8U, map1, map2);

		cv::remap(image, dst, map1, map2, cv::INTER_LINEAR);
	}
	else
	{
		throw invalid_argument("Invalid type of undistort");
		return;
	}

	ostringstream outputFile;
	outputFile << "z:\\sandbox\\lensdistortion\\output\\result\\calibrated-type("<< typeUndistort <<").png";

	cv::imwrite(outputFile.str(), dst);

}



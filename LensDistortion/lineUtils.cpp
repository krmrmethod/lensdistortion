#include "lineUtils.hpp"

using namespace std;

double getAngle(cv::Point p1, cv::Point p2)
{
	return abs(atan2(p1.y - p2.y, p1.x - p2.x) * 180 / M_PI);
}

bool checkAngleInsideCone(double angle)
{
	return (((angle >= 0 && angle <= 45) || (angle >= 315 && angle <= 359)) || (angle >= 135 && angle <= 225));

	//if (dir == Direction::horizontal)
	//{
	//	return (((angle >= 0 && angle <= 45) || (angle >= 315 && angle <= 359)) || (angle >= 135 && angle <= 225));
	//}
	//else
	//{
	//	return (angle >= 45 && angle <= 135) || (angle >= 225 && angle <= 315);
	//}
}

void calculateNeighbors(const cv::Point origin, std::vector<cv::Point>& directions)
{
	directions = {	cv::Point(origin.x - 1, origin.y),			//left
					cv::Point(origin.x - 1, origin.y - 1),		//top-left
					cv::Point(origin.x - 1, origin.y + 1),		//bottom-left
					cv::Point(origin.x, origin.y - 1),			//top
					cv::Point(origin.x, origin.y + 1),			//bottom
					cv::Point(origin.x + 1, origin.y - 1),		//top-right
					cv::Point(origin.x + 1, origin.y + 1),		//bottom-right
					cv::Point(origin.x + 1, origin.y)			//right
				};

}

tuple<int, int> calculateCandidateArea(const cv::Point& candidatePos, const int offset, const int maxValue)
{
	return tuple<int, int>{ candidatePos.y - offset > 0 ? candidatePos.y - offset : 0, candidatePos.y + offset < maxValue ? candidatePos.y + offset : maxValue };
}

vector<tuple<int,int,cv::Point>> findPossibleLineLocation(const cv::Mat& img, int length, const std::string outputName)
{
	vector<cv::Vec2i> possibleLines;
	vector<tuple<int, int, cv::Point>> candidateAreas;
	vector<vector<cv::Point>> linesToPaint;

	cv::Mat debugLines(img.rows, img.cols, CV_8U, cv::Scalar{0,0,0});

	//cv::cvtColor(debugLines, debugLines, cv::COLOR_GRAY2BGR);
	float angleThreshold = 15;

	//int inner = dir == Direction::vertical ? img.rows : img.cols;
	//int outer = dir == Direction::vertical ? img.cols : img.rows;

	for (int y = 1; y < img.rows; y++)
	{
		for (int x = 1; x < img.cols; x++)
		{
			if (img.at<uchar>(y,x) == UCHAR_MAX)
			{
				//cout << "Found white pixel at (x,y): (" << x << "," << y << ")" << endl;
				vector<cv::Point> line = getLine(img, cv::Point(x, y), 4);

				if (line.size() > length)
				{
					//cout << "Found line with length : " << line.size() << endl;
					double angle1 = getAngle(line[0], line[1]);
					double angle2 = getAngle(line[0], line.back());
					double deltaAngle = abs(angle1 - angle2);
					linesToPaint.push_back(line);

					if (deltaAngle < angleThreshold)
					{
						int v1 = line.back().x - line.front().x;
						int v2 = line.back().y - line.front().y;
						possibleLines.push_back(cv::Vec2i(v1, v2));					
						cv::line(debugLines, line.front(), line.back(), cv::Scalar{255,255,0}, 2);

						//cout << "Candidate found! Vector: (" << v1 << "," << v2 << ")" << endl;
						tuple<int, int> offsets{ calculateCandidateArea(line.at(line.size() / 2), 125, img.cols-1) };
						tuple<int, int, cv::Point> candidate{ get<0>(offsets), get<1>(offsets), line.at(line.size() / 2) };
						candidateAreas.push_back(candidate);
						cout << "Candidate area calculated with offsets: " << get<0>(offsets) << " " << get<1>(offsets) << endl;

						std::this_thread::sleep_for(std::chrono::milliseconds(5));

						y = get<1>(candidate);
						x = 0;

						//if (dir == Direction::horizontal)
						//{
						//	y = get<1>(candidate);
						//	x = 0;
						//}
						//else
						//{
						//	x = get<0>(candidate);
						//	y = 0;
						//}
					}
					else
					{
						cout << "Angle too step: " << deltaAngle << " (a1,a2): " << angle1 << " " << angle2 << endl;
						std::this_thread::sleep_for(std::chrono::milliseconds(5));
					}
				}
			}
		}// end for-loop cols
	}// end for-loop rows
	
	cv::Mat resultCandidates;

	img.copyTo(resultCandidates);

	cv::cvtColor(resultCandidates, resultCandidates, cv::COLOR_GRAY2BGR);


	for (std::tuple<int,int, cv::Point> area : candidateAreas)
	{
		//two red lines for begin and end of candidate area
		cv::line(resultCandidates, cv::Point(0, get<0>(area)), cv::Point(img.cols, get<0>(area)), cv::Scalar(0, 255, 255));
		cv::line(resultCandidates, cv::Point(0, get<1>(area)), cv::Point(img.cols, get<1>(area)), cv::Scalar(0, 255, 255));

		//line for the middle pixel of the line
		cv::line(resultCandidates, cv::Point(0, get<2>(area).y), cv::Point(img.cols, get<2>(area).y), cv::Scalar(0, 0, 255));

		//if (dir == Direction::horizontal) 
		//{
		//	cv::line(resultCandidates, cv::Point(0, get<0>(area)), cv::Point(img.cols, get<0>(area)), cv::Scalar(0, 255, 255));
		//	cv::line(resultCandidates, cv::Point(0, get<1>(area)), cv::Point(img.cols, get<1>(area)), cv::Scalar(0, 255, 255));
		//	cv::line(resultCandidates, cv::Point(0, get<2>(area).y), cv::Point(img.cols, get<2>(area).y), cv::Scalar(0, 0, 255));
		//}
		//else
		//{
		//	cv::line(resultCandidates, cv::Point(get<0>(area), 0), cv::Point(get<0>(area), img.rows), cv::Scalar(0, 255, 255));
		//	cv::line(resultCandidates, cv::Point(get<1>(area), 0), cv::Point(get<1>(area), img.rows), cv::Scalar(0, 255, 255));
		//	cv::line(resultCandidates, cv::Point(get<2>(area).x, 0), cv::Point(get<2>(area).x, img.rows), cv::Scalar(0, 0, 255));
		//}
	}

	cout << "Writing image for candidates found!" << std::endl;
	cv::imwrite("Z:\\sandbox\\LensDistortion\\output\\candidateAreas\\" + outputName, resultCandidates);
	cout << "Writing lines-found-image" << endl;
	cv::imwrite("Z:\\sandbox\\LensDistortion\\output\\candidateAreas\\linesFound_" + outputName, debugLines);

	paintLines(img, linesToPaint);

	return candidateAreas;

}

vector<cv::Point> getLine(const cv::Mat& img, cv::Point origin, float dirChangeThreshold)
{
	vector<cv::Point> foundPixels{origin};
	vector<cv::Point> neighboors;
		
	calculateNeighbors(origin, neighboors);
	
	do
	{
 		bool newPosFound = false;
		bool outsideBounds = neighboors.back().x >= img.cols || neighboors.back().y >= img.rows || neighboors.back().x <= 0 || neighboors.back().y <= 0;
		bool pixelNotInList = find(foundPixels.begin(), foundPixels.end(), neighboors.back()) == foundPixels.end();

		
		//check if next neighboor in queue is a white pixel and if neighboor doesn't exist already inside the list of found pixels
		if (!outsideBounds && pixelNotInList && img.at<uchar>(neighboors.back().y, neighboors.back().x) == 255)
		{
			//new current position is the neighboor found
			cv::Point pos = neighboors.back();
			foundPixels.push_back(pos);
			neighboors.clear();
			calculateNeighbors(pos, neighboors);
			newPosFound = true;
		}
		
		//if neighboor isn't white then pop
		if (!newPosFound)
		{
			neighboors.pop_back();
		}

	} while (!neighboors.empty());

	return foundPixels;
}

//void extractLines(const cv::Mat& img, vector<tuple<int, int, cv::Point>> candidateAreas)
//{
//	for (tuple<int, int, cv::Point> candidate : candidateAreas)
//	{
//		for(int y = )
//	}
//}

void paintLines(const cv::Mat& img, std::vector<std::vector<cv::Point>> lines)
{
	cv::Mat debugLines(img.rows, img.cols, CV_8U, cv::Scalar{ 0,0,0 });

	for (vector<cv::Point> line : lines)
	{
		for (cv::Point point : line)
		{
			debugLines.at<uchar>(point) = 255;
		}
	}

	cout << "Writing image with all found lines" << endl;
	cv::imwrite("Z:\\sandbox\\LensDistortion\\output\\candidateAreas\\paintedLines.png", debugLines);
}

void applyThinning(const cv::Mat& img)
{
	
}
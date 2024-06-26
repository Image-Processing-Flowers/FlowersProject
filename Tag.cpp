#include "stdafx.h"
#include "Tag.hpp"
#include "common.h"
#include <cmath> // For std::sqrt
#include <limits> // For std::numeric_limits

using namespace std;

int Tag::getRandomTag() {
	return rand() % 5;
}

bool sortByVal(const std::pair<string, int>& a, const std::pair<string, int>& b) {
	return a.second > b.second;
}

GeometricCharacteristics** Tag::applyFilter(vector<string> train, map<string, int> flowersMap) {
	GeometricCharacteristics** characteristics = new GeometricCharacteristics * [5];
	for (int i = 0; i < 5; ++i) {
		characteristics[i] = new GeometricCharacteristics{ 0, 0, 0, 0, 0, 0 };
	}
	int count[5] = { 0, 0, 0, 0, 0 };

	for (const auto& imagePath : train) {
		int tag = flowersMap[imagePath];
		Mat gray = imread(imagePath, 0);

		// Apply Laplacian filter
		Mat laplacian;
		Laplacian(gray, laplacian, CV_16S, 3);
		convertScaleAbs(laplacian, laplacian);

		cv::Mat binary;
		cv::threshold(laplacian, binary, 128, 255, cv::THRESH_BINARY);

		// Find contours
		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(binary, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

		// Analyze contours
		for (size_t i = 0; i < contours.size(); ++i) {
			double area = cv::contourArea(contours[i]);
			characteristics[tag]->averageArea += area;

			double perimeter = cv::arcLength(contours[i], true);
			characteristics[tag]->averagePerimeter += perimeter;

			cv::Rect boundingBox = cv::boundingRect(contours[i]);
			characteristics[tag]->averageBoundingBoxWidth += boundingBox.width;
			characteristics[tag]->averageBoundingBoxHeight += boundingBox.height;

			double aspectRatio = static_cast<double>(boundingBox.width) / boundingBox.height;
			characteristics[tag]->averageAspectRatio += aspectRatio;

			cv::Point2f center;
			float radius;
			cv::minEnclosingCircle(contours[i], center, radius);
			characteristics[tag]->averageEnclosingCircleRadius += radius;

			++count[tag];
		}
	}

	for (int i = 0; i < 5; ++i) {
		if (count[i] > 0) {
			characteristics[i]->averageArea /= count[i];
			characteristics[i]->averagePerimeter /= count[i];
			characteristics[i]->averageBoundingBoxWidth /= count[i];
			characteristics[i]->averageBoundingBoxHeight /= count[i];
			characteristics[i]->averageAspectRatio /= count[i];
			characteristics[i]->averageEnclosingCircleRadius /= count[i];
		}
	}

	return characteristics;
}

GeometricCharacteristics Tag::calculateGeometricCharacteristics(const string& imagePath) {
	GeometricCharacteristics characteristics{ 0, 0, 0, 0, 0, 0 };

	Mat gray = imread(imagePath, 0);
	if (gray.empty()) {
		cerr << "Could not read the image: " << imagePath << endl;
		return characteristics;
	}

	// Apply Laplacian filter
	Mat laplacian;
	Laplacian(gray, laplacian, CV_16S, 3);
	convertScaleAbs(laplacian, laplacian);

	cv::Mat binary;
	cv::threshold(laplacian, binary, 128, 255, cv::THRESH_BINARY);

	// Find contours
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binary, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	double totalArea = 0, totalPerimeter = 0, totalBoundingBoxWidth = 0, totalBoundingBoxHeight = 0, totalAspectRatio = 0, totalEnclosingCircleRadius = 0;
	size_t numContours = contours.size();

	// Analyze contours
	for (const auto& contour : contours) {
		double area = cv::contourArea(contour);
		totalArea += area;

		double perimeter = cv::arcLength(contour, true);
		totalPerimeter += perimeter;

		cv::Rect boundingBox = cv::boundingRect(contour);
		totalBoundingBoxWidth += boundingBox.width;
		totalBoundingBoxHeight += boundingBox.height;

		double aspectRatio = static_cast<double>(boundingBox.width) / boundingBox.height;
		totalAspectRatio += aspectRatio;

		cv::Point2f center;
		float radius;
		cv::minEnclosingCircle(contour, center, radius);
		totalEnclosingCircleRadius += radius;
	}

	if (numContours > 0) {
		characteristics.averageArea = totalArea / numContours;
		characteristics.averagePerimeter = totalPerimeter / numContours;
		characteristics.averageBoundingBoxWidth = totalBoundingBoxWidth / numContours;
		characteristics.averageBoundingBoxHeight = totalBoundingBoxHeight / numContours;
		characteristics.averageAspectRatio = totalAspectRatio / numContours;
		characteristics.averageEnclosingCircleRadius = totalEnclosingCircleRadius / numContours;
	}

	return characteristics;
}

int Tag::getGeometricTag(const string& imagePath, GeometricCharacteristics** characteristics) {
	GeometricCharacteristics imageCharacteristics = calculateGeometricCharacteristics(imagePath);

	float minDistance = (std::numeric_limits<float>::max)();
	int closestTag = -1;

	for (int i = 0; i < 5; ++i) {
		float distance = 0.0;
		distance += pow(characteristics[i]->averageArea - imageCharacteristics.averageArea, 2);
		distance += pow(characteristics[i]->averagePerimeter - imageCharacteristics.averagePerimeter, 2);
		distance += pow(characteristics[i]->averageBoundingBoxWidth - imageCharacteristics.averageBoundingBoxWidth, 2);
		distance += pow(characteristics[i]->averageBoundingBoxHeight - imageCharacteristics.averageBoundingBoxHeight, 2);
		distance += pow(characteristics[i]->averageAspectRatio - imageCharacteristics.averageAspectRatio, 2);
		distance += pow(characteristics[i]->averageEnclosingCircleRadius - imageCharacteristics.averageEnclosingCircleRadius, 2);

		distance = sqrt(distance);

		if (distance < minDistance) {
			minDistance = distance;
			closestTag = i;
		}
	}

	return closestTag;
}

void Tag::assignVariableRangeValues(vector<string> paths, map<String, int> flowersMap, map<int, map<string, float>>& colorsByLabel) {

	//map for collecting the number of pixels by color by label
	vector<string> colors = { "RED", "WHITE", "PINK", "YELLOW", "GREEN", "ORANGE" };
	map<String, int> colorsFreq;

	for (const auto& path : paths) {

		colorsFreq.clear();
		assignColorsForImageByHSV(path, colorsFreq);

		for (const auto& color : colorsFreq) {
			colorsByLabel[flowersMap[path]][color.first] += color.second;
		}

	}
	for (int i = 0; i < 5; i++) {

		for (const auto& color : colors) {
			//compute the average number of pixels with an exact color
			colorsByLabel[i][color] /= 500.0;
		}
	}
	cout << "Variable range values assigned" << endl;
}

void Tag::assignColorsForImageByHSV(String imagePath, map<String, int>& colFreq) {

	Mat image = imread(imagePath);
	if (image.empty()) {
		cout << "Could not read the image" << endl;
		return;
	}

	// Convert BGR to HSV format
	Mat hsvImage;
	cvtColor(image, hsvImage, COLOR_BGR2HSV);

	// Access the matrix
	// Fast method for continuous memory allocation
	if (hsvImage.isContinuous()) {
		cv::Vec3b* pixels = hsvImage.ptr<cv::Vec3b>(); // Pointer to the first pixel
		for (size_t i = 0; i < hsvImage.total(); i++) {
			cv::Vec3b& pixel = pixels[i];
			int H = pixel[0];
			int S = pixel[1];
			int V = pixel[2];

			// Define color ranges in HSV
			//RED (split into two ranges because of the Hue wrapping at 180)
			if ((H >= 0 && H <= 10 || H >= 170 && H <= 179) && S >= 100 && V >= 100) {
				colFreq["RED"] += 1;
			}

			//WHITE
			if (S <= 25 && V >= 200) {
				colFreq["WHITE"] += 1;
			}

			//YELLOW
			if (H >= 20 && H <= 30 && S >= 100 && V >= 100) {
				colFreq["YELLOW"] += 1;
			}

			//PINK
			if (H >= 150 && H <= 169 && S >= 100 && V >= 100) {
				colFreq["PINK"] += 1;
			}

			//ORANGE
			if (H >= 11 && H <= 20 && S >= 100 && V >= 100) {
				colFreq["ORANGE"] += 1;
			}

			//GREEN
			if (H >= 50 && H <= 90 && S >= 100 && V >= 100) {
				colFreq["GREEN"] += 1;
			}
		}
	}
}

void Tag::assignColorsForImageByRGB(String imagePath, map<String, int>& colFreq) {

	Mat image = imread(imagePath);
	if (image.empty()) {
		cout << "Could not read the image" << endl;
		return;
	}

	// Convert to RGB format
	Mat data;
	image.convertTo(data, CV_8UC3);

	// Access the matrix 
	// Fast method for continuous memory allocation
	if (data.isContinuous()) {
		cv::Vec3b* pixels = data.ptr<cv::Vec3b>(); // Pointer to the first pixel
		for (size_t i = 0; i < data.total(); i++) {
			cv::Vec3b& pixel = pixels[i];

			//BGR
			//RED
			if (pixel[0] >= 0 && pixel[0] <= 100 && pixel[1] >= 0 && pixel[1] <= 100 && pixel[2] >= 200 && pixel[2] <= 255) {
				colFreq["RED"] += 1;
			}

			//WHITE
			if (pixel[0] >= 200 && pixel[0] <= 255 && pixel[1] >= 200 && pixel[1] <= 255 && pixel[2] >= 200 && pixel[2] <= 255) {
				colFreq["WHITE"] += 1;
			}

			//YELLOW
			if (pixel[0] >= 0 && pixel[0] <= 100 && pixel[1] >= 200 && pixel[1] <= 255 && pixel[2] >= 200 && pixel[2] <= 255) {
				colFreq["YELLOW"] += 1;
			}

			//PINK
			if (pixel[0] >= 100 && pixel[0] <= 180 && pixel[1] >= 100 && pixel[1] <= 180 && pixel[2] >= 200 && pixel[2] <= 255) {
				colFreq["PINK"] += 1;
			}

			//ORANGE
			if (pixel[0] >= 0 && pixel[0] <= 100 && pixel[1] >= 100 && pixel[1] <= 165 && pixel[2] >= 200 && pixel[2] <= 255) {
				colFreq["ORANGE"] += 1;
			}

			//GREEN
			if (pixel[0] >= 0 && pixel[0] <= 100 && pixel[1] >= 200 && pixel[1] <= 255 && pixel[2] >= 0 && pixel[2] <= 100) {
				colFreq["GREEN"] += 1;
			}
		}
	}
}

int Tag::getColorRGBTag(String imagePath) {

	map<String, int> colorFreq;

	//find the number of appearences of a colors in a photo
	assignColorsForImageByHSV(imagePath, colorFreq);

	//numbers of most predominant colors
	int K = 3;

	// Copying the map to a vector of pairs
	std::vector<std::pair<cv::String, int>> freqVec(colorFreq
		.begin(), colorFreq.end());

	// Sort the vector by the frequency (second element of the pair), in descending order
	std::sort(freqVec.begin(), freqVec.end(), sortByVal);


	int limit = min(K, static_cast<int>(freqVec.size()));
	array<std::string, 70> topThreeColors;

	bool foundYellow = false;
	bool foundOrange = false;
	bool foundRed = false;
	bool foundGreen = false;
	bool foundPink = false;
	bool foundWhite = false;

	for (int i = 0; i < limit && i < K; i++) {

		// Add the color name to the list
		topThreeColors[i] = (freqVec[i].first);
		if (freqVec[i].first == "YELLOW") {
			foundYellow = true;
		}

		if (freqVec[i].first == "ORANGE") {
			foundOrange = true;
		}

		if (freqVec[i].first == "RED") {
			foundRed = true;
		}

		if (freqVec[i].first == "GREEN") {
			foundGreen = true;
		}

		if (freqVec[i].first == "PINK") {
			foundPink = true;
		}

		if (freqVec[i].first == "WHITE") {
			foundWhite = true;
		}
	}

	//sunflower
	if (foundOrange == true && foundYellow == true)
		return 3;

	//orchid
	if (foundWhite == true && foundPink == true)
		return 2;

	//tulip
	if (foundWhite == true && (foundOrange == true || foundRed == true || foundPink == true))
		return 4;
	//lily
	if (topThreeColors[0] == "WHITE")
		return 0;

	//lotus
	return 1;
}

int Tag::getColorHSVTag(String imagePath, map<int, map<string, float>>& colorsByLabel) {

	std::map<std::string, int> colorFreq;
	assignColorsForImageByHSV(imagePath, colorFreq);

	float minDistance = (std::numeric_limits<float>::max)();
	int closestTag = -1;

	for (const auto& labelPair : colorsByLabel) {

		int label = labelPair.first;
		const auto& tagColorMap = labelPair.second;

		float distance = 0.0;

		for (const auto& tagColorPair : tagColorMap) {

			string color = tagColorPair.first;
			float tagColorValue = tagColorPair.second;
			int imageColorValue = colorFreq[color];

			distance += pow(tagColorValue - imageColorValue, 2);
		}

		distance = sqrt(distance);

		if (distance < minDistance) {
			minDistance = distance;
			closestTag = label;
		}
	}

	return closestTag;
}


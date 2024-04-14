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

void Tag::assignVariableRangeValues(map<String, int> flowersMap, map<int, map<string, float>> &colorsByLabel) {
	//map for collecting the number of pixels by color by label
	//!!!!!!TODO pt SERJU: inlocuiste TOATA LOGICA RGB de aici cu HSV
	//ADICA = SERJU CREAZA O FUNCTIE NOUA assignColorsForImageByRGB DAR PENTRU HSV si o apelezi aici
	map<String, int> colorsFreq;
	for (const auto& pair : flowersMap) {

		const string& imagePath = pair.first;
		int label = pair.second;

		//aici schimbi cand ai cu assignColorsForImageByHSV
		assignColorsForImageByRGB(imagePath, colorsFreq);
		for (const auto& color : colorsFreq) {
			colorsByLabel[label][color.first] += color.second;
		}
	}
	for (int i = 0; i < 5; i++) {

		for (const auto& color : colorsFreq) {
			//compute the average number of pixels with an exact color
			colorsByLabel[i][color.first] /= 500.0;
		}
	}
	cout << "Variable range values assigned" << endl;
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
	assignColorsForImageByRGB(imagePath, colorFreq);

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

int Tag::getColorRGBTag2(String imagePath, map<int, map<string, float>>& colorsByLabel) {

	std::map<std::string, int> colorFreq;
	assignColorsForImageByRGB(imagePath, colorFreq);

	float minDistance = (std::numeric_limits<float>::max)();
	int closestTag = -1;

	for (const auto& labelPair : colorsByLabel) {
		int label = labelPair.first;
		const auto& tagColorMap = labelPair.second;

		float distance = 0.0;
		for (const auto& tagColorPair : tagColorMap) {
			std::string color = tagColorPair.first;
			float tagColorValue = tagColorPair.second;
			int imageColorValue = colorFreq.count(color) > 0 ? colorFreq[color] : 0;

			distance += std::pow(tagColorValue - imageColorValue, 2);
		}

		distance = std::sqrt(distance);
		std::cout << "Label: " << label << " Distance: " << distance << std::endl;  // Debug output

		if (distance < minDistance) {
			minDistance = distance;
			closestTag = label;
		}
	}

	return closestTag;
}
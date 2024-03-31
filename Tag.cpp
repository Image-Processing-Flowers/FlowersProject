#include "stdafx.h"
#include "Tag.hpp"
#include "common.h"

using namespace std;

int Tag::getRandomTag() {
	return rand() % 5;
}

bool sortByVal(const std::pair<string, int>& a, const std::pair<string, int>& b) {
	return a.second > b.second;
}

int Tag::getColorV1Tag(String imagePath) {
	Mat image = imread(imagePath);
	if (image.empty()) {
		cout << "Could not read the image" << endl;
		return 1;
	}

	// Convert to RGB format
	Mat data;
	image.convertTo(data, CV_8UC3);

	// Declare colorsFrequency map with Vec3bCompare as the comparator
	std::map<string, int> colFreq;

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

	//numbers of most predominant colors
	int K = 3;

	// Copying the map to a vector of pairs
	std::vector<std::pair<cv::String, int>> freqVec(colFreq.begin(), colFreq.end());

	// Sort the vector by the frequency (second element of the pair), in descending order
	std::sort(freqVec.begin(), freqVec.end(), sortByVal);

	// Printing the sorted values
	/*for (int i = 0; i < K; i++) {
		std::cout << freqVec[i].first<< "Frequency: " << freqVec[i].second << std::endl;
	}*/
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

	//lily
	if (topThreeColors[0] == "WHITE")
		return 0;

	//sunflower
	if (foundOrange == true && foundYellow == true)
		return 3;

	//orchid
	if (foundWhite == true && foundPink == true)
		return 2;

	//tulip
	if (foundWhite == true &&(foundOrange == true || foundRed == true || foundPink == true))
		return 4;

	//lotus
	return 1;
}
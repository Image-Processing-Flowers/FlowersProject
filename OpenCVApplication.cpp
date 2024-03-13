// OpenCVApplication.cpp : Defines the entry point for the console application.
//

// De modificat:
// Functia sa returneze lista cu poze, functii noi: functie de separat cele 2 seturi train si test;
// functie care sa genereze etichetele, functie care sa stabileasca path-ul in functie de cine ruleaza codul.
// Test la fiecare functie. Functia care creaza lista de imagini trebuie sortata.

#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <string>
#include <iostream>

using namespace std;

#define path "E:\\Scoala\\Facultate\\An 3\\PI\\flower_images";

void openImagesBatch()
{
	std::string folderPath = path;

	// Array of flower folder names
	// 0 - Lilly, 1 - Lotus, 2 - Orchid, 3 - Sunflower, 4 - Tulip
	std::string flowerFolders[] = {"Lilly", "Lotus", "Orchid", "Sunflower", "Tulip"};
	std::vector<cv::String> train;
	std::vector<cv::String> test;
	map<String, int> flowersMap;
	int i = 0;
	for (const auto& flowerFolder : flowerFolders)
	{
		std::string currentFolderPath = folderPath + "\\" + flowerFolder;

		// Use OpenCV's glob function to find files matching the pattern
		std::vector<cv::String> imageFiles;
		cv::glob(currentFolderPath + "\\*.jpg", imageFiles, false);

		// Iterate through the found files
		int counter = 0;
		for (const auto& imagePath : imageFiles)
		{
			flowersMap.insert(pair<String, int>(imagePath, i));
			cv::Mat src = cv::imread(imagePath);
			if (counter == 0)
				train.push_back(imagePath);
			else 
				test.push_back(imagePath);
			counter = 1 - counter;
		}
		i++;
	}
	for (const auto& imagePath : train)
		cout << "Train: " << imagePath << endl;
	for (const auto& imagePath : test)
		cout << "Test: " << imagePath << endl;
	map<string, int>::iterator it = flowersMap.begin();
	while (it != flowersMap.end()) {
		cout << "Key: " << it->first
			<< ", Value: " << it->second << endl;
		++it;
	}


}

int main() 
{
	openImagesBatch();
	return 0;
}


//#include <array>
//#include <iostream>
//#ifdef _WIN32
//#include <windows.h>
//#else
//#include <unistd.h>
//#endif

//std::string GetComputerName() {
//	std::array<char, MAX_COMPUTERNAME_LENGTH + 1> computerName{};
//	DWORD size = computerName.size();
//	if (GetComputerNameA(computerName.data(), &size)) {
//		return std::string(computerName.data(), size);
//	}
//	return "Unknown";
//}
//int main() {
//	cout << GetComputerName();
//}
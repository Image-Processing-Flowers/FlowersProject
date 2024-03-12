// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <string>
#include <iostream>

using namespace std;

#define path "C:\\Users\\DELL\\Desktop\\skull\\3\\PI\\project\\flower_images";

vector<String> openImagesBatch(map<String, int>& flowersMap)
{
	//this method opens the image batch, tags the types and returns all the path to all flower photos
	std::string folderPath = path;

	// Array of flower folder names
	// 0 - Lilly, 1 - Lotus, 2 - Orchid, 3 - Sunflower, 4 - Tulip
	std::string flowerFolders[] = { "Lilly", "Lotus", "Orchid", "Sunflower", "Tulip" };
	vector<String> imagePaths;
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
			imagePaths.push_back(imagePath);
			/*cv::Mat src = cv::imread(imagePath);
			if (counter == 0)
				train.push_back(imagePath);
			else
				test.push_back(imagePath);
			if (!src.data)
			{
				std::cerr << "Error loading image: " << imagePath << std::endl;
				continue;
			}
			counter = 1 - counter;*/
		}
		i++;
	}
	sort(imagePaths.begin(), imagePaths.end());
	return imagePaths;
}
void assignTrainTest(vector<String>& train, vector<String>& test) {

}

int main()
{
	map<String, int> flowersMap;
	vector<String> imagePaths;
	imagePaths = openImagesBatch(flowersMap);
	for (const auto& elem : imagePaths)
	{
		cout << elem << endl;
	}
	for (const auto& elem : flowersMap)
	{
		cout << elem.first << " " << elem.second << endl;
	}
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
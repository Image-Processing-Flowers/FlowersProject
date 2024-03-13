// OpenCVApplication.cpp : Defines the entry point for the console application.
//
//
#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <string>
#include <iostream>
#include "stdafx.h"
#include <array>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

String path = "Not initialized!";

String GetComputerName() {

	array<char, MAX_COMPUTERNAME_LENGTH + 1> computerName{};
	DWORD size = computerName.size();

	if (GetComputerNameA(computerName.data(), &size)) {
		return String(computerName.data(), size);
	}
	return "Unknown";
}

void AssignPath() {

	String pcName = GetComputerName();

	//Vlad
	if (pcName == "DESKTOP-0RHH391") {
		path = "C:\\Users\\DELL\\Desktop\\skull\\3\\PI\\project\\flower_images";
	}
	//Ivan

	//Istrate

	//Serju

}

void OpenImagesBatch(vector<String>& imagePaths, map<String, int>& flowersMap) {
	//this method opens the image batch, tags the types and returns all the path to all flower photos
	String folderPath = path;

	// Array of flower folder names
	// 0 - Lilly, 1 - Lotus, 2 - Orchid, 3 - Sunflower, 4 - Tulip
	String flowerFolders[] = { "Lilly", "Lotus", "Orchid", "Sunflower", "Tulip" };
	int i = 0;

	for (const auto& flowerFolder : flowerFolders)
	{
		String currentFolderPath = folderPath + "\\" + flowerFolder;

		vector<String> imageFiles;
		cv::glob(currentFolderPath + "\\*.jpg", imageFiles, false);

		for (const auto& imagePath : imageFiles)
		{
			flowersMap.insert(pair<String, int>(imagePath, i));
			imagePaths.push_back(imagePath);
		}
		i++;
	}

	sort(imagePaths.begin(), imagePaths.end());
}
void AssignTrainTest(vector<String> imagePaths, vector<String>& train, vector<String>& test) {

	bool toggle = true;
	for (const auto& elem : imagePaths) {

		if (toggle == true) {
			train.push_back(elem);
		}
		else
			test.push_back(elem);
		toggle = !toggle;
	}
}

int main()
{
	AssignPath();

	map<String, int> flowersMap;
	vector<String> imagePaths;
	vector<String> test;
	vector<String> train;

	OpenImagesBatch(imagePaths, flowersMap);
	AssignTrainTest(imagePaths, train, test);


	//results printing
	for (const auto& elem : imagePaths)
	{
		cout << elem << endl;
	}
	for (const auto& elem : flowersMap)
	{
		cout << elem.first << " " << elem.second << endl;
	}
	for (const auto& elem : train) {
		cout << elem << "    ->train" << endl;
	}
	for (const auto& elem : test) {
		cout << elem << "    ->test" << endl;
	}
	cout << imagePaths.size() << endl;

	return 0;
}
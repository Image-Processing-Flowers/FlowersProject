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
#include "Tag.hpp"
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
	if (pcName == "DESKTOP-F7R1VR7") {
		path = "C:\\An 3 Sem 2\\PI\\flower_images";
	}

	//Istrate
	if (pcName == "ANDREI-PC") {
		path = "E:\\Scoala\\Facultate\\An 3\\PI\\flower_images";
	}

	//Serju
	if (pcName == "DESKTOP-MML3IRG") {
		path = "C:\\Users\\Sergiu\\Documents\\Faculta Stuff\\Procesarea Imaginilor\\flower_images";
	}

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

		vector<String> auxPaths;
		vector<String> imageFiles;
		cv::glob(currentFolderPath + "\\*.jpg", imageFiles, false);

		for (const auto& imagePath : imageFiles)
		{
			flowersMap.insert(pair<String, int>(imagePath, i));
			auxPaths.push_back(imagePath);
		}

		i++;
		sort(auxPaths.begin(), auxPaths.end());
		imagePaths.insert(imagePaths.end(), auxPaths.begin(), auxPaths.end());

	}


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

	cout << "No. elements assigned to test: " << test.size() << endl;
	cout << "No. elements assigned to train: " << train.size() << endl;
}

void AreAllFilesOpened(const vector<String>& allFiles) {

	cout << "Files opened: " << allFiles.size() << endl;
	if (allFiles.size() == 5000)
	{
		cout << "The test was passed successfully!" << endl;
	}
	else
	{
		cout << "The test failed!" << endl;
	}
	return;
}

void generateTestTags(vector<String>& test, map<String, int>& testMap) {

	if (test.empty()) {
		cout << "WARNING: Elements is empty. Firstly, assign elements to test." << endl;
	}

	for (const auto& elem : test) {

		int randomTag = Tag::getRandomTag();
		testMap.insert(pair<String, int>(elem, randomTag));
	}

	cout << "Test tags have been generated." << endl;
}

void tagsCorrectRangeTest(map<String, int> testMap) {

	if (testMap.empty()) {
		cout << "ERROR: Test tags aren't generated." << endl;
	}

	bool inRange = true;
	for (const auto& elem : testMap) {
		if (elem.second < 0 || elem.second > 4) {
			inRange = false;
			cout << "Element {" << elem.first << "}\n\tnot in range [0, 4], with wrong tag {" << elem.second << "}." << endl;
		}

	}
	if (inRange) {
		cout << "All elements have correct tags," << endl;
	}
}

void calculateAccuracy(vector<String> test, map<String, int> flowersMap, map<String, int> testMap, float& accuracy) {
	
	if (test.empty()) {
		cout << "WARNING: Elemtents for test are not assigned." << endl;
	}
	if (testMap.empty()) {
		cout << "WARNING: Test tag are not generated." << endl;
	}

	int checks = 0;
	for (const auto& elem : test) {
		if (flowersMap[elem] == testMap[elem]) {
			checks++;
		}
	}

	accuracy = checks / (float)test.size() * 100.0f;

}

int main()
{
	vector<String> possibleOptions = {
		"Assign Test/Train",
		"[TEST] Check number of files opened",
		"Generate random tags for test",
		"[TEST] Tags in correct range",
		"Calculate accuracy",
		"Exit"
	};
	int optionChosed = -1;

	vector<String> imagePaths;
	vector<String> test;
	vector<String> train;

	map<String, int> flowersMap;
	map<String, int> testMap;

	float accuracy;

	AssignPath();
	OpenImagesBatch(imagePaths, flowersMap);

	//logic for executing the chosen option
	//user interface
	while (1) {

		cout << "LIST OF OPTIONS:" << endl << endl;
		for (int i = 0; i < possibleOptions.size(); i++) {
			cout << i << " - " << possibleOptions[i] << endl;
		}
		cout << "\n<#> Choose from one of the options: ";

		cin >> optionChosed;
		cout << " ------------------ " << endl;

		switch (optionChosed) {
		case 0:
			AssignTrainTest(imagePaths, train, test);
			break;

		case 1:
			AreAllFilesOpened(imagePaths);
			break;

		case 2:
			generateTestTags(test, testMap);
			testMap[test[0]] = 7;
			break;

		case 3:
			tagsCorrectRangeTest(testMap);
			break;
		case 4:
			calculateAccuracy(test, flowersMap, testMap, accuracy);
			cout << "Accuracy: " << accuracy << " % " << endl;
			break;
		case 5:
			return 0;

		default:
			cout << "Invalid option" << endl;
			break;
		}

		cout << " ------------------\n " << endl;
		system("pause");
		system("cls");
	}

	return 0;
}
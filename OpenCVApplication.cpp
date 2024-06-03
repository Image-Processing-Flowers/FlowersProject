// OpenCVApplication.cpp : Defines the entry point for the console application.
//
//
#define _HAS_STD_BYTE 0
#include <cstddef> 
#include "stdafx.h"
#include "common.h"
#include <filesystem>
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

namespace fs = filesystem;
using namespace std;

String path = "Not initialized!";

	// Array of flower folder names
	// 0 - Lily, 1 - Lotus, 2 - Orchid, 3 - Sunflower, 4 - Tulip
String flowerFolders[] = { "Lilly", "Lotus", "Orchid", "Sunflower", "Tulip" };

String GetComputerName() {

	array<char, MAX_COMPUTERNAME_LENGTH + 1> computerName{};
	DWORD size = computerName.size();

	if (GetComputerNameA(computerName.data(), &size)) {
		return String(computerName.data(), size);
	}
	return "Unknown";
}

void assignPath() {

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

void openImagesBatch(vector<String>& imagePaths, map<String, int>& flowersMap) {
	//this method opens the image batch, tags the types and returns all the path to all flower photos
	String folderPath = path;

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

void assignTrainTest(vector<String> imagePaths, vector<String>& train, vector<String>& test) {

	bool toggle = true;
	if (train.size() == 0 && test.size() == 0)
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

void areAllFilesOpened(const vector<String>& allFiles) {

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

// In the 'testMap' we generate a random number for every element in test
void generateRandomTestTags(vector<String>& test, map<String, int>& testMap) {
	//empty testmap
	testMap.clear();
	// in case 'test' have no assigned elements
	if (test.empty()) {
		cout << "WARNING: Elements is empty. Firstly, assign elements to test." << endl;
		return;
	}

	// generate tags
	for (const auto& elem : test) {

		int randomTag = Tag::getRandomTag();
		testMap.insert(pair<String, int>(elem, randomTag));
	}

	cout << "Test tags have been generated." << endl;
}

void generateColorV1TestTags(vector<String>& test, map<String, int>& testMap) {
	//empty testmap
	testMap.clear();
	// in case 'test' have no assigned elements
	if (test.empty()) {
		cout << "WARNING: Elements is empty. Firstly, assign elements to test." << endl;
		return;
	}

	// generate tags
	for (const auto& path : test) {
		int colorTag = Tag::getColorRGBTag(path);
		testMap.insert(pair<String, int>(path, colorTag));
	}

	cout << "Test tags have been generated." << endl;
}

void generateColorV2TestTags(vector<String>& test, map<String, int>& testMap, map<int, map<String, float>>& colorsByLabel) {
	//empty testmap
	testMap.clear();
	// in case 'test' have no assigned elements
	if (test.empty()) {
		cout << "WARNING: Elements is empty. Firstly, assign elements to test." << endl;
		return;
	}

	// generate tags
	for (const auto& path : test) {
		int colorTag = Tag::getColorHSVTag(path, colorsByLabel);
		testMap.insert(pair<String, int>(path, colorTag));
	}

	cout << "Test tags have been generated." << endl;
}

// Verify if the value(tag) from the 'testMap' is in range 0 to 4
void tagsCorrectRangeTest(map<String, int> testMap) {

	// in case 'testMap' is not populated
	if (testMap.empty()) {
		cout << "ERROR: Test tags aren't generated." << endl;
	}

	// display every element with the unwanted tag number
	bool allInRange = true;
	for (const auto& elem : testMap) {
		if (elem.second < 0 || elem.second > 4) {
			allInRange = false;
			cout << "Element {" << elem.first << "}\n\tnot in range [0, 4], with wrong tag {" << elem.second << "}." << endl;
		}

	}

	// in case every tag is in the range
	if (allInRange) {
		cout << "All elements have correct tags." << endl;
	}
}

void emptyFolder(const fs::path& dirPath) {
	try {
		// Check if the path exists and is a directory
		if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
			// Iterate through each item in the directory
			for (const auto& entry : fs::directory_iterator(dirPath)) {
				// Remove files and directories recursively
				fs::remove_all(entry.path());
			}
			cout << "Folder has been emptied successfully." << endl;
		}
		else {
			cout << "The specified path does not exist or is not a directory." << endl;
		}
	}
	catch (const fs::filesystem_error& e) {
		cerr << "Error: " << e.what() << endl;
	}
}

// Calculate the accuracy and save the wrong labeled photos
void calculateAccuracy(vector<String> test, map<String, int> flowersMap, map<String, int> testMap, float& accuracy) {
	// in case 'test' have no assigned elements
	if (test.empty()) {
		cout << "WARNING: Elemtents for 'test' are not assigned." << endl;
	}

	// in case 'testMap' is not populated
	if (testMap.empty()) {
		cout << "WARNING: Test tag are not generated." << endl;
	}

	// increment the 'checks' if the value from 'testMap' match the value from 'flowersMap' for every element from 'test'
	int checks = 0;
	string wrong_path = path + "\\WrongImages";
	string correct_path = path + "\\CorrectImages";

	// Create directories if they don't exist
	if (!fs::exists(wrong_path)) {
		fs::create_directories(wrong_path);
	}
	if (!fs::exists(correct_path)) {
		fs::create_directories(correct_path);
	}

	for (const auto& elem : test) {
		if (flowersMap[elem] == testMap[elem]) {
			checks++;

			// Save the photos that were labeled correctly
			Mat image = cv::imread(elem);

			fs::path originalPath(elem);
			string filename = originalPath.filename().string();

			// Check if the image has been loaded properly
			if (image.empty()) {
				cout << "Could not read the image: " << elem << endl;
				continue;
			}

			bool isSavedCorrect = cv::imwrite(correct_path + "\\" + filename, image);

			if (!isSavedCorrect) {
				cout << "Failed to save the image to: " << correct_path + "\\" + filename << endl;
				continue;
			}
		}
		else {
			// Save the photos that were NOT labeled correctly
			Mat image = cv::imread(elem);

			fs::path originalPath(elem);
			string filename = originalPath.filename().string();

			// Check if the image has been loaded properly
			if (image.empty()) {
				cout << "Could not read the image: " << elem << endl;
				continue;
			}

			// Save the image to the specified path
			bool isSavedWrong = cv::imwrite(wrong_path + "\\" + filename, image);

			if (!isSavedWrong) {
				cout << "Failed to save the image to: " << wrong_path + "\\" + filename << endl;
				continue;
			}
		}
	}

	// Divide the number of 'checks' by the total number of 'test';
	// multiplied by 100 so the value can be read in percentage
	accuracy = checks / (float)test.size() * 100.0f;
}

void assignRangeValues(vector<string> train, map<String, int> flowersMap, map<int, map<String, float>>& colorsByLabel) {
	Tag::assignVariableRangeValues(train, flowersMap, colorsByLabel);
}

void printRangeValues(map<int, map<String, float>>& colorsByLabel) {
	map<int, string> flowerNames = {
		{0, "Lily"},
		{1, "Lotus"},
		{2, "Orchid"},
		{3, "Sunflower"},
		{4, "Tulip"}
	};

	for (const auto& labelPair : colorsByLabel) {
		int label = labelPair.first;
		const auto& colorsMap = labelPair.second;

		// Using flower names instead of tag numbers
		cout << "Flower " << flowerNames[label] << ":\n";
		for (const auto& colorPair : colorsMap) {
			cout << "  Color " << colorPair.first << ": " << colorPair.second << " average pixels\n";
		}
	}
}

void printPredictionMatrix(map<String, int> predictionMap, map<String, int> trueFlowerMap, vector<String> test) {

	String flowerFolders[] = { "Lilly", "Lotus", "Orchid", "Sunflower", "Tulip" };
	int mat[5][5] = { 0 };
	auto itPred = predictionMap.begin();
	for (const auto& elem : test) {
		mat[itPred->second][trueFlowerMap[elem]]++;
		++itPred;
	}
	// Set a fixed width for 4-digit numbers with additional padding
	int width = 12; // Adjust this width as needed for aesthetics

	// Top border
	cout << " +";
	for (int j = 0; j < 6; ++j) {
		cout << setfill('-') << setw(width + 1) << "+";
	}
	cout << endl;
	//Box with the info about rows and columns
	cout << " |";
	cout << setfill(' ') << setw(width) << "Predict\\True" << "|";
	//Row with true flower tags
	for (int i = 0; i < 5; i++) {
		cout << setfill(' ') << setw(width) << flowerFolders[i] << "|";
	}
	cout << endl;
	//Bottom border of first row
	cout << " +";
	for (int j = 0; j < 6; ++j) {
		cout << setfill('-') << setw(width + 1) << "+";
	}
	cout << endl;

	// Print the matrix with uniform column widths and borders
	for (int i = 0; i < 5; ++i) {
		cout << " |";
		//Column with predicted flower tags
		cout << setfill(' ') << setw(width) << flowerFolders[i] << "|";
		for (int j = 0; j < 5; ++j) {
			//Matrix values
			cout << setfill(' ') << setw(width) << mat[i][j] << "|";
		}
		cout << endl;

		// Print row border
		cout << " +";
		for (int j = 0; j < 6; ++j) {
			cout << setfill('-') << setw(width + 1) << "+";
		}
		cout << endl;
	}

}

void demoCorrectWrongRGB() {
	String correct = path + "\\CorrectImages\\1eecf7a4c5.jpg";
	String wrong = path + "\\WrongImages\\00f36a3c40.jpg";
	map<int, map<string, float>> colorsByLabel;

	// Information on image labeled correct
	Mat correctImg = imread(correct);
	if (correctImg.empty()) {
		cout << "ERROR: Correct image not found! Ensure the path and file name are correct." << endl;
		return;
	}
	resize(correctImg, correctImg, Size(400, 400));
	imshow("Correct RGB", correctImg);

	int correctTag = Tag::getColorRGBTag(correct);
	cout << "\nPredicted: " << flowerFolders[correctTag] << " -> CORRECT (Sunflower)\n";
	cout << "Correct image color frequencies:" << endl;

	map<string, int> correctColorFreq;
	Tag::assignColorsForImageByHSV(correct, correctColorFreq);
	for (const auto& pair : correctColorFreq) {
		cout << pair.first << ": " << pair.second << endl;
	}

	// Information on image labeled wrong
	Mat wrongImg = imread(wrong);
	if (wrongImg.empty()) {
		cout << "ERROR: Wrong image not found! Ensure the path and file name are correct." << endl;
		return;
	}
	resize(wrongImg, wrongImg, Size(400, 400));
	imshow("Wrong RGB", wrongImg);

	int wrongTag = Tag::getColorRGBTag(wrong);
	cout << "\nPredicted: " << flowerFolders[wrongTag] << " -> WRONG (Lily) \n";
	cout << "Wrong image color frequencies:" << endl;

	map<string, int> wrongColorFreq;
	Tag::assignColorsForImageByHSV(wrong, wrongColorFreq);
	for (const auto& pair : wrongColorFreq) {
		cout << pair.first << ": " << pair.second << endl;
	}

	waitKey(0);
}

void demoCorrectWrongGeometric(GeometricCharacteristics** characteristics) {
	String correct = path + "\\CorrectImages\\1a81e77515.jpg";
	String wrong = path + "\\WrongImages\\0dfaf0300b.jpg";

	// Information on image labeled correct
	Mat correctImg = imread(correct);
	if (correctImg.empty()) {
		cout << "ERROR: Correct image not found! Ensure the path and file name are correct." << endl;
		return;
	}
	resize(correctImg, correctImg, Size(400, 400));
	imshow("Correct Geometric", correctImg);

	int correctTag = Tag::getGeometricTag(correct, characteristics);
	cout << "\nPredicted: " << "Sunflower" << " -> CORRECT (Sunflower)\n";
	cout << "Correct image geometric characteristics:" << endl;

	GeometricCharacteristics correctChar = Tag::calculateGeometricCharacteristics(correct);
	cout << "  Average Area: " << correctChar.averageArea << endl;
	cout << "  Average Perimeter: " << correctChar.averagePerimeter << endl;
	cout << "  Average Bounding Box Width: " << correctChar.averageBoundingBoxWidth << endl;
	cout << "  Average Bounding Box Height: " << correctChar.averageBoundingBoxHeight << endl;
	cout << "  Average Aspect Ratio: " << correctChar.averageAspectRatio << endl;
	cout << "  Average Enclosing Circle Radius: " << correctChar.averageEnclosingCircleRadius << endl;

	// Information on image labeled wrong
	Mat wrongImg = imread(wrong);
	if (wrongImg.empty()) {
		cout << "ERROR: Wrong image not found! Ensure the path and file name are correct." << endl;
		return;
	}
	resize(wrongImg, wrongImg, Size(400, 400));
	imshow("Wrong Geometric", wrongImg);

	int wrongTag = Tag::getGeometricTag(wrong, characteristics);
	cout << "\nPredicted: " << flowerFolders[wrongTag] << " -> WRONG (Lotus)\n";
	cout << "Wrong image geometric characteristics:" << endl;

	GeometricCharacteristics wrongChar = Tag::calculateGeometricCharacteristics(wrong);
	cout << "  Average Area: " << wrongChar.averageArea << endl;
	cout << "  Average Perimeter: " << wrongChar.averagePerimeter << endl;
	cout << "  Average Bounding Box Width: " << wrongChar.averageBoundingBoxWidth << endl;
	cout << "  Average Bounding Box Height: " << wrongChar.averageBoundingBoxHeight << endl;
	cout << "  Average Aspect Ratio: " << wrongChar.averageAspectRatio << endl;
	cout << "  Average Enclosing Circle Radius: " << wrongChar.averageEnclosingCircleRadius << endl;

	waitKey(0);
}




int main()
{
	srand(time(0)); // init for random
	vector<String> possibleOptions = {
		"Assign Test/Train",
		"[TEST] Check number of files opened",
		"Generate random tags for test",
		"Generate color tags for test",
		"[TEST] Tags in correct range",
		"Calculate accuracy",
		"Print prediction matrix",
		"Variable range values",
		"Print range values",
		"Generate color tags for test v2",
		"Apply filter and calculate geometric characteristics",
		"Generate geometric tags for test",
		"Demo Correct/Wrong RGB",
		"Demo Correct/Wrong Geometric",
		"Exit"
	};
	int optionChosed = -1;

	vector<String> imagePaths;
	vector<string> test;
	vector<String> train;

	map<String, int> flowersMap;
	map<String, int> testMap;

	map<int, map<String, float>> colorsByLabel;

	float accuracy;

	assignPath();
	openImagesBatch(imagePaths, flowersMap);
	string wrongPath = path + "\\WrongImages";
	emptyFolder(wrongPath);

	GeometricCharacteristics** characteristics = nullptr;

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
			assignTrainTest(imagePaths, train, test);
			break;
		case 1:
			areAllFilesOpened(imagePaths);
			break;
		case 2:
			generateRandomTestTags(test, testMap);
			break;
		case 3:
			generateColorV1TestTags(test, testMap);
			break;
		case 4:
			tagsCorrectRangeTest(testMap);
			break;
		case 5:
			calculateAccuracy(test, flowersMap, testMap, accuracy);
			cout << "Accuracy: " << accuracy << " % " << endl;
			break;
		case 6:
			printPredictionMatrix(testMap, flowersMap, test);
			break;
		case 7:
			assignRangeValues(train, flowersMap, colorsByLabel);
			break;
		case 8:
			printRangeValues(colorsByLabel);
			break;
		case 9:
			generateColorV2TestTags(test, testMap, colorsByLabel);
			break;
		case 10:
			characteristics = Tag::applyFilter(train, flowersMap);
			for (int i = 0; i < 5; ++i) {
				cout << "Tag " << i << " Geometric Characteristics:" << endl;
				cout << "  Average Area: " << characteristics[i]->averageArea << endl;
				cout << "  Average Perimeter: " << characteristics[i]->averagePerimeter << endl;
				cout << "  Average Bounding Box Width: " << characteristics[i]->averageBoundingBoxWidth << endl;
				cout << "  Average Bounding Box Height: " << characteristics[i]->averageBoundingBoxHeight << endl;
				cout << "  Average Aspect Ratio: " << characteristics[i]->averageAspectRatio << endl;
				cout << "  Average Enclosing Circle Radius: " << characteristics[i]->averageEnclosingCircleRadius << endl;
			}
			break;
		case 11:
			if (characteristics == nullptr) {
				cout << "You need to calculate the geometric characteristics first (option 10)." << endl;
			}
			else {
				for (const auto& imagePath : test) {
					int tag = Tag::getGeometricTag(imagePath, characteristics);
					testMap[imagePath] = tag;
				}
				cout << "Geometric tags for test set generated." << endl;
			}
			break;
		case 12:
			demoCorrectWrongRGB();
			break;
		case 13:
			demoCorrectWrongGeometric(characteristics);
			break;
		case 14:
			return 0;
		default:
			cout << "Invalid option" << endl;
			break;
		}

		cout << " ------------------\n " << endl;

#ifdef _WIN32
		system("pause"); // Pause
		system("cls");   // Clear screen
#else
		cout << "Press Enter to continue...";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.get();
		system("clear");
#endif
	}

	if (characteristics != nullptr) {
		for (int i = 0; i < 5; ++i) {
			delete characteristics[i];
		}
		delete[] characteristics;
	}

	return 0;
}
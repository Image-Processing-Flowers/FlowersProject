#ifndef Tag_hpp
#define Tag_hpp

#include <iostream>
#include <cstdlib>
#include <random>
#include <opencv2/opencv.hpp>
#include <cmath> // For std::sqrt
#include <limits> // For std::numeric_limits

typedef struct {
	double averageArea;
	double averagePerimeter;
	double averageBoundingBoxWidth;
	double averageBoundingBoxHeight;
	double averageAspectRatio;
	double averageEnclosingCircleRadius;
} GeometricCharacteristics;

using namespace std;

class Tag
{

public:

	// Returns a number number between 0 and 4
	static int getRandomTag();

	static int getColorRGBTag(string imagePath);

	static int getColorHSVTag(string imagePath, map<int, map<string, float>>& colorsByLabel);

	static void assignVariableRangeValues(vector<string> paths, map<string, int> flowersMap, map<int, map<string, float>>&colorsByLabel);

	static void assignColorsForImageByRGB(string imagePath, map<string,int>& colorFreq);

	static void assignColorsForImageByHSV(string imagePath, map<string, int>& colorFreq);

	static GeometricCharacteristics** applyFilter(vector<string> train, map<string, int> flowersMap);

	static GeometricCharacteristics calculateGeometricCharacteristics(const string &imagePath);

	static int getGeometricTag(const string& imagePath, GeometricCharacteristics** characteristics);

};

#endif


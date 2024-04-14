#ifndef Tag_hpp
#define Tag_hpp

#include <iostream>
#include <cstdlib>
#include <random>
#include <opencv2/opencv.hpp>
#include <cmath> // For std::sqrt
#include <limits> // For std::numeric_limits

using namespace std;

class Tag
{
public:

	// Returns a number number between 0 and 4
	static int getRandomTag();

	static int getColorRGBTag(string imagePath);

	static int getColorRGBTag2(string imagePath, map<int, map<string, float>>& colorsByLabel);

	static void assignVariableRangeValues(map<string, int> flowersMap, map<int, map<string, float>>&colorsByLabel);

	static void assignColorsForImageByRGB(string imagePath, map<string,int>& colorFreq);
};

#endif


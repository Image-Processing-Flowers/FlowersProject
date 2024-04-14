#ifndef Tag_hpp
#define Tag_hpp

#include <iostream>
#include <cstdlib>
#include <random>
#include <opencv2/opencv.hpp>

using namespace std;

class Tag
{
public:

	// Returns a number number between 0 and 4
	static int getRandomTag();

	static int getColorRGBTag(string imagePath);

	static void assignVariableRangeValues(map<string, int> flowersMap);

	static void assignColorsForImageByRGB(string imagePath, map<string,int>& colorFreq);
};

#endif


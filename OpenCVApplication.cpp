// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <string>

#define path "E:\\Scoala\\Facultate\\An 3\\PI\\flower_images";

void openImagesBatch()
{
	std::string folderPath = path;

	// Array of flower folder names
	std::string flowerFolders[] = { "Lilly", "Lotus", "Orchid", "Sunflower", "Tulip" };

	for (const auto& flowerFolder : flowerFolders)
	{
		std::string currentFolderPath = folderPath + "\\" + flowerFolder;

		// Use OpenCV's glob function to find files matching the pattern
		std::vector<cv::String> imageFiles;
		cv::glob(currentFolderPath + "\\*.jpg", imageFiles, false);

		// Iterate through the found files
		for (const auto& imagePath : imageFiles)
		{
			cv::Mat src = cv::imread(imagePath);

			if (!src.data)
			{
				std::cerr << "Error loading image: " << imagePath << std::endl;
				continue;
			}

			imshow("image", src);
			waitKey();
		}
	}
}

int main() 
{

	openImagesBatch();

	/*
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_FATAL);
    projectPath = _wgetcwd(0, 0);

	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative\n");
		printf(" 4 - Image negative (fast)\n");
		printf(" 5 - BGR->Gray\n");
		printf(" 6 - BGR->Gray (fast, save result to disk) \n");
		printf(" 7 - BGR->HSV\n");
		printf(" 8 - Resize image\n");
		printf(" 9 - Canny edge detection\n");
		printf(" 10 - Edges in a video sequence\n");
		printf(" 11 - Snap frame from live video\n");
		printf(" 12 - Mouse callback demo\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testNegativeImage();
				break;
			case 4:
				testNegativeImageFast();
				break;
			case 5:
				testColor2Gray();
				break;
			case 6:
				testImageOpenAndSave();
				break;
			case 7:
				testBGR2HSV();
				break;
			case 8:
				testResize();
				break;
			case 9:
				testCanny();
				break;
			case 10:
				testVideoSequence();
				break;
			case 11:
				testSnap();
				break;
			case 12:
				testMouseClick();
				break;
		}
	}
	while (op!=0);
	return 0;
	*/
}
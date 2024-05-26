
# Flower Recognition Project 
## Table of Contents

1. [Introduction](#introduction)
2. [Project Overview](#project-overview)
   1. [Handling the dataset](#1-handling-the-dataset)
   2. [Result Computation and Presentation](#2-result-computation-and-presentation)
   3. [Classification Methods](#3-classification-methods)
3. [Prerequisites](#prerequisites)
   1. [Software](#software)
   2. [Hardware](#hardware)
4. [Set up and Troubleshooting](#set-up-and-troubleshooting)
5. [User Guide and Options Description](#user-guide-and-options-description)
6. [Project Structure](#project-structure)
   1. [OpenCVApplication.cpp](#opencvapplicationcpp)
   2. [Tag.hpp](#taghpp)
   3. [Tag.cpp](#tagcpp)
7. [Conclusions](#conclusions)

## Introduction

&nbsp; &nbsp;&nbsp; &nbsp;This project aims to correctly classify the photos of the dataset: https://www.kaggle.com/datasets/kausthubkannan/5-flower-types-classification-dataset, using OpenCV for image processing. The dataset contains 5 types of flowers, each of them having 1000 photos. The included flower classes are: Sunflower, Tulip, Orchid, Lily, and Lotus.

## Project Overview

### 1. Handling the dataset
&nbsp; &nbsp;&nbsp; &nbsp; First, we needed to store the data on disk. Then we separated the main folders into subfolders depending on the flower type. After this, we sorted the photos lexicographically and split them into a training set and a test set.
### 2. Result Computation and Presentation
&nbsp; &nbsp;&nbsp; &nbsp; Depending on the methods that will be described below, the accuracy is always computed on the results compared to the test set. To be more specific and to have a much more complex analysis of the result, there is a confusion matrix for each type of flower.
### 3. Classification Methods
&nbsp; &nbsp;&nbsp; &nbsp; Each method is a standalone method and uses different classification algorithms based on different approaches such as color frequency or geometric characteristics. Any wrong classification of the test set will be saved in the WrongImages folder according to its prediction class.

## Prerequisites
&nbsp; &nbsp;&nbsp; &nbsp;All the following prerequisites were used in the development process of this project.
### Software
- **IDE**: Visual Studio 2022
- **Language**: C++ 17 Standard
- **Library**: OpenCV
### Hardware
- **Processor**: Dual-core minimum, Quad-core recommended.
- **RAM**: 4 GB minimum, 8 GB recommended
- **Disk Space**: at least 350 MB.

## Set up and Troubleshooting
&nbsp; &nbsp;&nbsp; &nbsp;The most common problem while trying to get the program to work is the double definition of a macro type. For this, we will need to add to the Project -> Properties -> C/C++ -> Preprocessor -> Preprocessor Definitions: **_HAS_STD_BYTE=0;_CRT_SECURE_NO_WARNINGS;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)**. Other than this, everything should work as intended if all the previous conditions are met.

## User Guide and Options Description
&nbsp; &nbsp;&nbsp; &nbsp;For the program to work, the user needs to add its path to the flower images folder according to its PC name. Before running the actual method, we need to run the "Assign Test/Train" option first to split the set into training set and test set. The result printing methods: "Calculate accuracy", "Print prediction matrix" will not work unless a classification method is used.
&nbsp; &nbsp;&nbsp; &nbsp; The "Generate random tags for test" is pretty straightforward, you don't need to choose any other option than this one to make it work. Instead, for the "Generate color tags for test", using RGB, and "Generate color tags for test v2", using HSV, you need to use the "Variable range values" option to assign the average characteristics of each type of flower. You can see these values by running the "Print range values" option. The methods used to generate color tags measure the average frequency of the colors: red, white, green, orange, pink, and yellow. The RGB one is based on a decision tree and the v2 one is based on KNN.
&nbsp; &nbsp;&nbsp; &nbsp;The "Generate geometric tags for test" option needs to first run option 10 to compute the average geometric characteristics. This method is based on KNN too, but before computing the contours and the geometric analysis, we first apply a Laplace filter on each image.
&nbsp; &nbsp;&nbsp; &nbsp;The options that begin with [TEST] are auxiliary methods used to test options responsible for the management of the files and the correct range prediction.

## Project Structure
&nbsp; &nbsp;&nbsp; &nbsp; The project is based on the flower photos folder and 3 editable important files:
- OpenCVApplication.cpp
- Tag.cpp
- Tag.hpp

### OpenCVApplication.cpp
&nbsp; &nbsp;&nbsp; &nbsp; Represents the file containing the main function and all the options for the user to run. It also contains all the print and file management functions.
### Tag.hpp
&nbsp; &nbsp;&nbsp; &nbsp; Tag.hpp is the header used for the methods of the Tag class, responsible for all computation methods.
### Tag.cpp  
&nbsp; &nbsp;&nbsp; &nbsp; A class used to pass and compute all the relevant results for the classification process of the flowers.

## Conclusions
&nbsp; &nbsp;&nbsp; &nbsp; The most efficient method is the "Generate color tags for test" combining RGB and a hardcoded decision tree based on the visual differences between flowers. A substantial improvement that can be added to the project is by applying multiple filters, such as the one in "Generate geometric tags for test". We can obtain improved results as tested in the option mentioned above before and after adding a filter. Although color seems to be a good differentiator at first, most of these flower types share the same colors. Their shape could be a strong differentiator but only if more filters are applied. Because of the mix of colors and the vague background, it is a source of error.

#pragma once

#ifndef FIRE_DETECTION_H
#define FIRE_DETECTION_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>

int fireColorDetection(cv::VideoCapture& video);//checks if fire color exist
int smokeColorDetection(cv::VideoCapture& video);//checks if smoke color exist
int motionDetection(cv::VideoCapture& video);//checks motion

#endif // FIRE_DETECTION_H

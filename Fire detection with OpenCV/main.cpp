#include <opencv2/opencv.hpp>
#include "fire_detection.h"
using namespace std;




int main() {
    // Open the video capture device (replace "0" with the appropriate camera index)
    //cv::VideoCapture video(0);
    string targetVideo = "data/fire4.mp4"; // for fire color and motion
    cv::VideoCapture video(targetVideo);

    /*
    int ismotionDetected = motionDetection(video);
    if (ismotionDetected == 0) {
        cout << "Video file could not opened" << endl;
    }
    else if (ismotionDetected == 1) {
        cout << "Motion is not detected" << endl;
    }
    else if (ismotionDetected == 2) {
        cv::Mat img = cv::imread("data/motion_detected_image.jpg");
        cv::imshow("Motion Detected image", img);
        cv::waitKey(0);
    }*/
    

    
    int isFireDetected = fireColorDetection(video);
    if (isFireDetected == 0) {
        std::cout << "Video file could not opened" << std::endl;
    }
    else if (isFireDetected == 1) {
        std::cout << "Fire is not detected" << std::endl;
    }
    else if (isFireDetected == 2) {
        cv::Mat img = cv::imread("data/fire_detected_image.jpg");
        cv::imshow("Fire Detected image", img);
        cv::waitKey(0);
    }  
    

    /*
    int isSmokeDetected = smokeColorDetection(video);
    if (isSmokeDetected == 0) {
        std::cout << "Video file could not opened" << std::endl;
    }
    else if (isSmokeDetected == 1) {
        std::cout << "Smoke is not detected" << std::endl;
    }
    else if (isSmokeDetected == 2) {
        cv::Mat img = cv::imread("data/smoke_detected_image.jpg");
        cv::imshow("Smoke Detected image", img);
        cv::waitKey(0);
    }*/
    

    return 0;
}





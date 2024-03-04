#include <opencv2/opencv.hpp>

#include "fire_detection.h"


int motionDetection(cv::VideoCapture& video) {

    // these will be used to send signal after 3 seconds
    time_t fireDetectedTimeInitial = time(0); // it is just a dummy variable to initialize variables
    time_t fireDetectedTime1 = fireDetectedTimeInitial; // time of fire detection first time
    time_t fireDetectedTime2 = fireDetectedTimeInitial; // time of fire detection at current time
    double fireDurationThreshold = 5; // required time for fire detected alarm
    time_t starttime = time(0); // time of code starting to work
    time_t endtime = time(0); // time of end of code
    bool flag = false;// fireDetectionTime variables reset or not
    double minMotionArea = 1500000; // min number of pixels changed
    cv::Mat previousFrame, currentFrame, motion;
    

    if (!video.isOpened()) {
        std::cout << "************************" << std::endl;
        std::cout << "Error: Could not open video file." << std::endl;
        std::cout << "************************" << std::endl;
        return 0;
    }

    starttime = time(0); // time of code starting to work

    video >> previousFrame;
    int counter = 0;
    while (true) {
        for(int i=0; i<10000;i++){}
        video >> currentFrame;
        
        if (currentFrame.empty() || previousFrame.empty()) {
            std::cerr << "End of video stream." << std::endl;
            break;
        }
        cv::resize(previousFrame, previousFrame, cv::Size(900, 400));
        cv::resize(currentFrame, currentFrame, cv::Size(900, 400));
        //cv::cvtColor(previousFrame, previousFrame, cv::COLOR_BGR2GRAY);
        //cv::cvtColor(currentFrame, currentFrame, cv::COLOR_BGR2GRAY);

        // Compute absolute difference between frames
        cv::absdiff(previousFrame, currentFrame, motion);
        
        // Apply a threshold to identify significant changes
        cv::threshold(motion, motion, 30, 255, cv::THRESH_BINARY);

        //int motionSize = cv::countNonZero(motion);
        double motionSize = cv::sum(motion)[0];
        std::cout << motionSize << std::endl;
        /* // this part will be configured according to fire size 
        if (motionSize >= minMotionArea) {
            cv::imwrite("data/motion_detected_image.jpg", motion);
            std::cout << "Fire detected! Frame saved as data/motion_detected_image.jpg\n";

            cv::destroyAllWindows();
            video.release();

            return 2;
        }*/
        // Display the frames
        cv::imshow("Current Frame", currentFrame);
        cv::imshow("Motion Detection", motion);
        cv::imshow("Previous Frame", previousFrame);
        previousFrame = currentFrame.clone();
        if (cv::waitKey(30) == 27) {
            break;
        }
    }
    std::cout << "************************" << std::endl;
    endtime = time(0); // end of code
    std::cout << "total time taken by code: " << difftime(endtime, starttime) << " seconds" << std::endl;
    std::cout << "************************" << std::endl;
    cv::destroyAllWindows();
    video.release();
    return 1;
}


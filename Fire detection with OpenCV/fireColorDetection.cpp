#include "fire_detection.h"

int fireColorDetection(cv::VideoCapture& video) {
    /*  Variables  */
    cv::Scalar lower(0, 150, 50); // fire color lower bounndry
    cv::Scalar upper(30, 255, 255); // fire color upper bounndry

    // these will be used to send signal after 3 seconds
    time_t fireDetectedTimeInitial = time(0); // it is just a dummy variable to initialize variables
    time_t fireDetectedTime1 = fireDetectedTimeInitial; // time of fire detection first time
    time_t fireDetectedTime2 = fireDetectedTimeInitial; // time of fire detection at current time
    double fireDurationThreshold = 5; // required time for fire detected alarm
    time_t starttime = time(0); // time of code starting to work
    time_t endtime = time(0); // time of end of code
    bool flag = false;// fireDetectionTime variables reset or not
    double minFireArea = 200; // min color size to assume as fire


    if (!video.isOpened()) {
        std::cout << "************************" << std::endl;
        std::cout << "Error: Could not open video file." << std::endl;
        std::cout << "************************" << std::endl;
        return 0;
    }

    starttime = time(0); // time of code starting to work

    while (true) {
        cv::Mat frame; // the video array to manipulate 
        cv::Mat output_frame; // the video array to output
        bool ret = video.read(frame);

        if (!ret) {
            std::cout << "************************" << std::endl;
            std::cout << "End of video." << std::endl;
            std::cout << "************************" << std::endl;
            break;
        }

        cv::resize(frame, output_frame, cv::Size(900, 400));
        cv::resize(frame, frame, cv::Size(900, 400));
        cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV); // convert the color space from BGR to HSV

        // create a mask in HSV with lower and upper boundries
        cv::Mat mask;
        cv::inRange(frame, lower, upper, mask);
        cv::Mat output;
        cv::bitwise_and(frame, frame, output, mask);

        // morpological operations to reduce noise and emphesize the detected region
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::dilate(mask, mask, kernel, cv::Point(-1, -1), 2);
        cv::erode(mask, mask, kernel, cv::Point(-1, -1), 1);

        // find contour in mask
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // find non zero pixels
        int size = cv::countNonZero(mask);

        // if contour exist, draw it on output_frame an frame
        if (!contours.empty()) {
            // if fire detected for first time
            if (fireDetectedTime1 == fireDetectedTimeInitial && fireDetectedTime2 == fireDetectedTimeInitial) {
                fireDetectedTime1 = time(0);
                flag = true;
            }
            else { // if it is not the first time
                fireDetectedTime2 = time(0);
                if ((fireDetectedTime2 - fireDetectedTime1) >= fireDurationThreshold) {
                    std::cout << "************************" << std::endl;
                    std::cout << "Fire Detected" << std::endl;

                    // draw contour that has size bigger than 200
                    std::vector<std::vector<cv::Point>> largeContours;
                    for (const auto& contour : contours) {
                        double area = cv::contourArea(contour);
                        if (area >= 200) {
                            largeContours.push_back(contour);
                        }
                    }
                    cv::drawContours(output_frame, largeContours, -1, cv::Scalar(0, 255, 0), 1.5);

                    cv::imwrite("data/fire_detected_image.jpg", output_frame);
                    std::cout << "Fire detected! Frame saved as data/fire_detected_image.jpg\n";
                    endtime = time(0); // end of code
                    std::cout << "total time taken by code: " << difftime(endtime, starttime) << " seconds" << std::endl;
                    std::cout << "************************" << std::endl;
                    cv::destroyAllWindows();
                    video.release();

                    return 2;
                }
                flag = true;
            }

            for (const auto& contour : contours) {
                double area = cv::contourArea(contour);

                if (area > minFireArea) {
                    // Fire detected
                    cv::drawContours(frame, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 0, 255), 2);
                    cv::drawContours(output_frame, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 0, 255), 2);
                }
            }
        }

        // if camera stop seeing fire reset the variables
        if (!flag) {
            fireDetectedTime1 = fireDetectedTimeInitial;
            fireDetectedTime2 = fireDetectedTimeInitial;
        }

        cv::imshow("output", output_frame);

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
#include <iostream>
#include <opencv2/opencv.hpp>

int smokeColorDetection(cv::VideoCapture& video) {
    /*  Variables    */
    /*
    Deccrease Value = darker  
     Decreasing Sat = more gray scale 
     H = shift the color range
    */


    //cv::Scalar lower(0, 0, 200);  // Lower bound in HSV
    //cv::Scalar upper(180, 50, 255);  // Upper bound in HSV

    cv::Scalar lower(100, 10,150);  // Lower bound of smoke color
    cv::Scalar upper(180, 50, 180);  // Upper bound of smoke color



    // these will be used to send signal after 3 seconds
    time_t smokeDetectedTimeInitial = time(0); // it is just a dummy variable to initialize variables
    time_t smokeDetectedTime1 = smokeDetectedTimeInitial; // time of smoke detection first time
    time_t smokeDetectedTime2 = smokeDetectedTimeInitial; // time of smoke detection at current time
    double smokeDurationThreshold = 5; // required time for smoke detected alarm
    time_t starttime = time(0); // time of code starting to work
    time_t endtime = time(0); // time of end of code
    bool flag = false;// smokeDetectionTime variables reset or not
    double minSmokeArea = 200; // min color size to assume as Smoke


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

        // create a mask in HSV with lower and upper boundaries
        cv::Mat mask;
        cv::inRange(frame, lower, upper, mask);
        cv::Mat output;
        cv::bitwise_and(frame, frame, output, mask);

        // morphological operations to reduce noise and emphasize the detected region
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::dilate(mask, mask, kernel, cv::Point(-1, -1), 2);
        cv::erode(mask, mask, kernel, cv::Point(-1, -1), 1);

        // find contour in mask
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // find non-zero pixels
        int size = cv::countNonZero(mask);

        // if contour exists, draw it on output_frame 
        if (!contours.empty()) {
            // if smoke detected for the first time
            if (smokeDetectedTime1 == smokeDetectedTimeInitial && smokeDetectedTime2 == smokeDetectedTimeInitial) {
                smokeDetectedTime1 = time(0);
                flag = true;
            }
            else { // if it is not the first time
                smokeDetectedTime2 = time(0);
                if ((smokeDetectedTime2 - smokeDetectedTime1) >= smokeDurationThreshold) {
                    std::cout << "************************" << std::endl;
                    std::cout << "Smoke Detected" << std::endl;

                    // draw contour that has size bigger than 200
                    std::vector<std::vector<cv::Point>> largeContours;
                    for (const auto& contour : contours) {
                        double area = cv::contourArea(contour);
                        if (area > 200) {
                            largeContours.push_back(contour);
                        }
                    }
                    cv::drawContours(output_frame, largeContours, -1, cv::Scalar(0, 0, 255), 1.5);

                    cv::imwrite("data/smoke_detected_image.jpg", output_frame);// record the image
                    std::cout << "Smoke detected! Frame saved as data/smoke_detected_image.jpg\n";
                    endtime = time(0); // time of end of code
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

                if (area > minSmokeArea) {
                    // Smoke detected
                    cv::drawContours(frame, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 0, 255), 2);
                    cv::drawContours(output_frame, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 0, 255), 2);
                }
            }
        }
        // if the camera stops seeing smoke reset the variables
        if (!flag) {
            smokeDetectedTime1 = smokeDetectedTimeInitial;
            smokeDetectedTime2 = smokeDetectedTimeInitial;
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

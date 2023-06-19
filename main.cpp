#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main() {
    cv::VideoCapture cameraFeed(2);

    if (!cameraFeed.isOpened()) {
        std::cout << "DroidCam is not working..." << std::endl;
        return -1;
    }
    // Use the MOG2 algorithm for outside motion detecting and the KNN for inside motion detecting.
    // Both aren't perfect and get false flags for shadows and light reflections.
    // Ptr<BackgroundSubtractorMOG2> pBackSub = createBackgroundSubtractorMOG2();
    Ptr<BackgroundSubtractorKNN> pBackSub = createBackgroundSubtractorKNN();
    Mat frame;
    Mat frame_gray;
    while (cameraFeed.read(frame)) {

        pBackSub->apply(frame, frame_gray);

        // Making the kernel a little bit bigger for both morphology lines makes the detector less sensitive between frames.
        morphologyEx(frame_gray, frame_gray, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        morphologyEx(frame_gray, frame_gray, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    
        vector<vector<Point>> contours;
        findContours(frame_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        vector<vector<Point>> filteredContours;
        for (const auto& contour : contours)
        {
            if (contour.empty()) continue; // Check if the contour is empty

            Rect boundingBox = boundingRect(contour); // Get the bounding box of each contour

            if (boundingBox.area() > 500) // Ignore small contours
            {
                filteredContours.push_back(contour);
                rectangle(frame, boundingBox, Scalar(0, 0, 255), 2); // Draw a red bounding box around each moving object
            }
        }

        cv::Mat video;
        cameraFeed.read(video);

        if (video.empty()) {
            std::cout << "Video not available" << std::endl;
            break;
        }
        imshow("Tracking", frame);
        cv::imshow("Camera Feed", video);

        int key = cv::waitKey(10);
        // To exit the motion tracker, press Esc on your keyboard.
        if (key == 27) {
            break;
        }
    }
    
    cameraFeed.release();
    cv::destroyAllWindows();

    return 0;
}

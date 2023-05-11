#include <opencv2/opencv.hpp>
#include <iostream>


using namespace std;
using namespace cv;

RNG rng(12345);

int main() {
    cv::VideoCapture cap(2);

    if (!cap.isOpened()) {
        std::cout << "DroidCam ded" << std::endl;
        return -1;
    }
    Ptr<BackgroundSubtractor> pBackSub = createBackgroundSubtractorMOG2();
    Mat frame;
    Mat frame_gray;
    while (cap.read(frame)) {
        

        pBackSub->apply(frame, frame_gray);
        morphologyEx(frame_gray, frame_gray, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        morphologyEx(frame_gray, frame_gray, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(11, 11)));
    
        vector<vector<Point> > contours;
        findContours(frame_gray, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours)
        {
            if (contour.empty()) continue; // Check if the contour is empty

            Rect boundingBox = boundingRect(contour); // Get the bounding box of each contour

            if (boundingBox.area() > 100) // Ignore small contours
            {
                rectangle(frame, boundingBox, Scalar(0, 0, 255), 2); // Draw a red bounding box around each moving object
            }
        }
        cv::Mat video;
        cap.read(video);

        if (video.empty()) {
            std::cout << "video not availabL" << std::endl;
            break;
        }
        imshow("Tracking", frame);
        cv::imshow("DroidCam Video", video);

        int key = cv::waitKey(10);
        if (key == 27) {
            break;
        }
    }
    
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
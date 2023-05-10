#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(2);

    if (!cap.isOpened()) {
        std::cout << "DroidCam ded" << std::endl;
        return -1;
    }

    cv::namedWindow("DroidCam Video", cv::WINDOW_NORMAL);

    while (true) {

        cv::Mat frame;
        cap.read(frame);

        if (frame.empty()) {
            std::cout << "frame not availabL" << std::endl;
            break;
        }

        cv::imshow("Tracking", frame);

        int key = cv::waitKey(10);
        if (key == 27) {
            break;
        }
    }
    
    cap.release();
    cv::destroyAllWindows();

    return 0;
}

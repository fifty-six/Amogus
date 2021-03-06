#include <iostream>

#include <stdio.h>
#include <opencv2/opencv.hpp>

using cv::Mat;
using cv::Size;

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        printf("Usage: %s INPUT OUTPUT RES\n", argv[0]);
        return -1;
    }

    auto img = cv::imread(argv[1]);

    if (!img.data) {
        std::cerr << "No data! Does the image [" << argv[1] << "] exist?" << std::endl;
        return -1;
    }

    auto res = std::atoi(argv[3]);

    // Scale it down so we don't have too many amogi
    cv::resize(img, img, cv::Size(res, static_cast<int>((img.rows / (1.0 * img.cols)) * res)));

    int codec = cv::VideoWriter::fourcc('a', 'v', 'c', '1');

    cv::VideoCapture amogus("amogus.mp4");

    if (!amogus.isOpened()) {
        std::cerr << "Unable to open amogus.mp4; Exiting." << std::endl;
        return -1;
    }

    const auto frames = amogus.get(cv::CAP_PROP_FRAME_COUNT);
    const auto vh = amogus.get(cv::CAP_PROP_FRAME_HEIGHT);
    const auto vw = amogus.get(cv::CAP_PROP_FRAME_WIDTH);

    Mat tiled(cv::Size(vw * img.cols, vh * img.rows), CV_8UC3);

    std::cout << tiled.size << std::endl;

    cv::VideoWriter vwriter
    (
        argv[2],
        codec,
        45,
        tiled.size(),
        true
    );

    if (!vwriter.isOpened()) {
        std::cout << "Unable to open video writer to file " << argv[2] << "! Exiting." << std::endl;
        return -1;
    }

    Mat frame;

    for (size_t i = 0; i < frames; i++)
    {
        if (!amogus.read(frame)) {
            std::cerr << "Unable to read frame [" << i << " / " << frames << "]! Exiting!" << std::endl;
            return -1;
        }

        Mat frame_orig = frame.clone();

        for (int j = 0; j < img.rows; j++) {
            for (int k = 0; k < img.cols; k++) {

                frame_orig.copyTo(frame);

                Mat dark_mask;
                Mat light_mask;
    
                // Mask out the dark green portion
                cv::inRange(frame, cv::Scalar(30, 0, 0), cv::Scalar(90, 255, 100), dark_mask);
    
                // Light green above the dark
                cv::inRange(frame, cv::Scalar(0, 180, 0), cv::Scalar(100, 255, 255), light_mask);

                auto pix = img.at<cv::Vec3b>(j, k);

                frame.setTo(pix * 0.5, dark_mask);
                frame.setTo(pix, light_mask);

                auto roi = tiled(cv::Rect(k * frame.cols, j * frame.rows, frame.cols, frame.rows));

                frame.copyTo(roi);
            }
        }

        vwriter.write(tiled);
    }

    vwriter.release();

    std::cout << "Wrote video!" << std::endl;

    amogus.release();

    return 0;
}

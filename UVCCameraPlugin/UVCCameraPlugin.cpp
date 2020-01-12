#include "UVCCameraPlugin.h"
#include <opencv2/opencv.hpp>
#include <cstdio>

void* getCamera()
{
    auto cap = new cv::VideoCapture(0);
    cap->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap->set(cv::CAP_PROP_FRAME_HEIGHT, 480); 

    cap->set(cv::CAP_PROP_EXPOSURE, -7);
    cap->set(cv::CAP_PROP_GAIN, 4);

    return static_cast<void*>(cap);
}

double getExposure(void* camera) 
{
    auto cap = static_cast<cv::VideoCapture*>(camera);
    return cap->get(cv::CAP_PROP_EXPOSURE);
}

double getFPS(void* camera) 
{
    auto cap = static_cast<cv::VideoCapture*>(camera);
    return cap->get(cv::CAP_PROP_FPS);
}

double getWidth(void* camera) 
{
    auto cap = static_cast<cv::VideoCapture*>(camera);
    return cap->get(cv::CAP_PROP_FRAME_WIDTH);
}

double getHeight(void* camera) 
{
    auto cap = static_cast<cv::VideoCapture*>(camera);
    return cap->get(cv::CAP_PROP_FRAME_HEIGHT);
}

void releaseCamera(void* camera)
{
    auto cap = static_cast<cv::VideoCapture*>(camera);
    delete cap;
}

void getCameraTexture(void* camera, unsigned char* data, int width, int height)
{
    auto cap = static_cast<cv::VideoCapture*>(camera);

    cv::Mat img;
    *cap >> img;

    cv::Mat resized_img(height, width, img.type());
    cv::resize(img, resized_img, resized_img.size(), cv::INTER_CUBIC);

    // RGB --> ARGB �ϊ�
    cv::Mat argb_img;
    cv::cvtColor(resized_img, argb_img, 2);
    std::vector<cv::Mat> bgra;
    cv::split(argb_img, bgra);
    std::swap(bgra[0], bgra[3]);
    std::swap(bgra[1], bgra[2]);
    std::memcpy(data, argb_img.data, argb_img.total() * argb_img.elemSize());
}

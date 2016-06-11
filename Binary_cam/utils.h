#ifndef UTILS_H
#define UTILS_H

#include "QImage"
#include "opencv2/opencv.hpp"

struct trapezium{
    cv::Point p[4];
};

QImage Mat2QImage(cv::Mat cvMat);

#endif // UTILS_H

#ifndef UTILS_H
#define UTILS_H

#include "QImage"
#include "opencv2/opencv.hpp"

struct quadrangle{
    cv::Point p[4];
};

QImage Mat2QImage(const cv::Mat &cvMat);

#endif // UTILS_H

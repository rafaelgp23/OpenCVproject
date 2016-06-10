#include "utils.h"


QImage Mat2QImage(cv::Mat cvMat){
    cv::cvtColor(cvMat, cvMat,cv::COLOR_BGR2RGB);
    return QImage((uchar*) cvMat.data, cvMat.cols, cvMat.rows, cvMat.step, QImage::Format_RGB888);
}

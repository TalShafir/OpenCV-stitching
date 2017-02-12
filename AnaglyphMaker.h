#ifndef AnaglyphMaker
#define AnaglyphMaker

#include <opencv2\imgproc.hpp>

void toAnaglyph(const cv::Mat &leftImage, const cv::Mat &rightImage, cv::Mat &result);

#endif

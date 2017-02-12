#ifndef FramesExtractor
#define FramesExtractor

#include <opencv2\imgproc.hpp>
#include <opencv2\core\cvstd.hpp>
#include <vector>

void extractFrames(const cv::String &videoName, std::vector<cv::Mat> &frames);


#endif
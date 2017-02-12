#include "FramesExtractor.h"

#include <opencv2\highgui.hpp>


void extractFrames(const cv::String &videoName, std::vector<cv::Mat> &frames)
{
	frames.resize(0);
	cv::VideoCapture cap(videoName);
	if (!cap.isOpened())
	{
		return;
	}

	int numFrames = (int)cap.get(CV_CAP_PROP_FRAME_COUNT);
	frames.reserve(numFrames);
	cv::Mat temp;

	for (int i = 0; i < numFrames; i++)
	{
		cap >> temp;
		frames.push_back(temp.clone());
	}

	temp.release();
	cap.release();
}
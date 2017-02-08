#include <stdio.h> 
#include <opencv2\opencv.hpp> 
#include <opencv2/stitching.hpp>
#include <iostream>
#include <vector>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include "opencv2/xfeatures2d.hpp"
#include <time.h>

using namespace cv;
//void on_trackbar(int, void*);
using namespace std;
const char* windowsName = "Panorama";
int index = 0;
int iters = 3;
int each_iter = 30;
int size = iters*each_iter;
//std::vector<Mat> panorama(size);

void stitchLeftRight(Mat& leftImage, Mat& rightImage, Mat& rightImageWarped, Mat& panorama);
#define OUTPUT_IMAGE "out.jpg"
void main()
{
	vector<Mat> panoramas(iters);
	Mat rightImageWarped1;
	std::vector<Mat> leftEye;

	leftEye = getFrames();
	clock_t tic = clock();

	for (int iter = 0; iter < iters; iter++)
	{
		stitchLeftRight(leftEye[iter*each_iter + 1], leftEye[iter*each_iter], rightImageWarped1, panoramas[iter]);

		for (int i = 2; i < each_iter; i++)
		{
			stitchLeftRight(leftEye[iter*each_iter + i], panoramas[iter], rightImageWarped1, panoramas[iter]);

			printf("%d\n", i);
		}
	}
	clock_t toc = clock();

	printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	//cvNamedWindow(windowsName, 1);
	for (int i = 0; i < iters;i++)
		imshow(std::to_string(i), panoramas[i]);
	//createTrackbar("MyTrackbar:", windowsName, &index, size - 2, on_trackbar);
	//on_trackbar(index, 0);
	while (true)
	{
		int c;
		c = waitKey(10);
		if ((char)c == 27) // ESC
		{
			break;
		}
	}

	//for (Mat tmp : panorama)
	//{
	//	tmp.release();

	//}

}

//void on_trackbar(int, void*)
//{
//
//	imshow(windowsName, panorama[index]);
//}

void stitchLeftRight(Mat& leftImage, Mat& rightImage, Mat& rightImageWarped, Mat& panorama)
{
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	Mat descriptors_1, descriptors_2, img_matches;
	std::vector<cv::DMatch> good_matches;
	std::vector<std::vector<DMatch>> matches;

	Ptr<FeatureDetector> brisk = BRISK::create(20, 3, 1.0f);
	//Ptr<Feature2D> brisk = KAZE::create();

	brisk->detect(leftImage, keypoints_1);
	brisk->detect(rightImage, keypoints_2);
	brisk->compute(leftImage, keypoints_1, descriptors_1);
	brisk->compute(rightImage, keypoints_2, descriptors_2);

	Ptr<DescriptorMatcher> matcher = BFMatcher::create("BruteForce-Hamming");
	matcher->knnMatch(descriptors_1, descriptors_2, matches, 2);  // Find two nearest matches

	const float ratio = 0.9;
	for (int i = 0; i < matches.size(); ++i)
	{
		if (matches[i][0].distance / matches[i][1].distance < ratio)
		{
			good_matches.push_back(matches[i][0]);
		}
	}

	// Isolate the matched keypoints in each image
	std::vector<Point2f> leftImage_matchedKPs;
	std::vector<Point2f> rightImage_matchedKPs;

	for (size_t i = 0; i < good_matches.size(); i++)
	{
		leftImage_matchedKPs.push_back(keypoints_1[good_matches[i].queryIdx].pt);
		rightImage_matchedKPs.push_back(keypoints_2[good_matches[i].trainIdx].pt);
	}

	// Find the Homography relating rightImage and leftImage
	Mat H = findHomography(Mat(rightImage_matchedKPs), Mat(leftImage_matchedKPs), RANSAC);
	// Warp rightImage to leftImage's space using the Homography just constructed
	//    Mat rightImageWarped;  // warped image has twice the width to account for overlap
	warpPerspective(rightImage, rightImageWarped, H, Size(5000, 1280), INTER_CUBIC);

	panorama = rightImageWarped.clone();
	// Overwrite leftImage on left end of final panorma image
	Mat roi(panorama, Rect(0, 0, leftImage.cols, leftImage.rows));
	leftImage.copyTo(roi);
}



std::vector<Mat> getFrames()
{
	std::vector<Mat> videoFrames;
	std::vector<Mat> leftEye;

	VideoCapture cap("lala.mov");
	if (!cap.isOpened())
		return;
	double frnb(cap.get(CV_CAP_PROP_FRAME_COUNT));
	std::cout << "frame count = " << frnb << endl;

	for (int i = 0; i < size; i++)
	{
		Mat frame;
		cap.set(CV_CAP_PROP_POS_FRAMES, i);
		bool success = cap.read(frame);
		if (!success) {
			cout << "Cannot read  frame " << endl;
			break;
		}
		//printf("%d\n", i);
		videoFrames.push_back(frame);
		Mat subImage(frame, cv::Rect(frame.cols*0.2, 0, frame.cols*0.3, frame.rows));
		leftEye.push_back(subImage);
	}
	return leftEye;
}
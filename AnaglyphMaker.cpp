#include "AnaglyphMaker.h"


//http://blogue.nbourre.profweb.ca/?p=64
// Return an anaglyph image of a right and left channel 
// Author : Nicolas Bourré 20100927 
void toAnaglyph(const cv::Mat &leftImage, const cv::Mat &rightImage, cv::Mat &result)
{
	static const int NUM_CHANNELS = 3;
	cv::Mat leftImageChannels[NUM_CHANNELS], rightImageChannels[NUM_CHANNELS]; //BGR

	split(leftImage, leftImageChannels);
	split(rightImage, rightImageChannels);

	cv::Mat channelsToMerge[NUM_CHANNELS + 1] = { rightImageChannels[0], rightImageChannels[1], leftImageChannels[2], leftImageChannels[0] };
	merge(channelsToMerge, NUM_CHANNELS + 1, result);

	for (int i = 0; i < NUM_CHANNELS; i++)
	{
		leftImageChannels[i].release();
		rightImageChannels[i].release();
	}
}





/*Original
int main()
{
Mat img1 = imread("cherry-left.jpg");
Mat img2 = imread("cherry-right.jpg");
Mat result;
toAnaglyph(img1, img2, result);
imwrite("redCyanPic.jpg", result);
imshow("Img1", img1);
imshow("Img2", img2);
imshow("Red/Cyan picture", result);
waitKey();
img1.release();
img2.release();
result.release();
//destroyAllWindows();
//IplImage* img = cvLoadImage("cherry-left.jpg");
//
//IplImage* img2 = cvLoadImage("cherry-right.jpg");

//cvNamedWindow("Image:", 1);
//cvNamedWindow("Gray Image:", 2);

//cvShowImage("Gray Image:", img2);
//cvShowImage("Image:", img);
//cvWaitKey();
//cvDestroyWindow("Image:");
//cvDestroyWindow("Gray Image:");



//cvSaveImage("redCyanPic.jpg", result);
//cvShowImage("result:", result);


return 0;
}




IplImage * toAnaglyph(IplImage * imgLeft, IplImage *imgRight)
{
IplImage *iplReturn; IplImage *l_R, * l_G, *l_B;
IplImage *r_R, * r_G, *r_B;
CvSize channelSize = cvGetSize(imgLeft);

iplReturn = cvCreateImage(channelSize, 8, 3);
l_R = cvCreateImage(channelSize, 8, 1);
l_G = cvCreateImage(channelSize, 8, 1);
l_B = cvCreateImage(channelSize, 8, 1);
r_R = cvCreateImage(channelSize, 8, 1);
r_G = cvCreateImage(channelSize, 8, 1);
r_B = cvCreateImage(channelSize, 8, 1);

cvSplit(imgLeft, l_R, l_G, l_B, NULL);
cvSplit(imgRight, r_R, r_G, r_B, NULL);
cvMerge(r_R, r_G, l_B, NULL, iplReturn);

cvReleaseImage(&l_R);
cvReleaseImage(&l_G);
cvReleaseImage(&l_B);
cvReleaseImage(&r_R);
cvReleaseImage(&r_G);
cvReleaseImage(&r_B);
return iplReturn;
}

*/

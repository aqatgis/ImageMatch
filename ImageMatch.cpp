
#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
#include <iostream>  

using namespace cv;
using namespace std;


int main()
{
	// ��Ӱ��
	Mat image01 = imread("img505.jpg", 1);
	Mat image02 = imread("img507.jpg", 1);
	namedWindow("��Ƭ", 0);
	imshow("��Ƭ", image01);
	namedWindow("��Ƭ", 0);
	imshow("��Ƭ", image02);

	//�Ҷ�ͼת��  
	Mat image1, image2;
	cvtColor(image01, image1, CV_RGB2GRAY);
	cvtColor(image02, image2, CV_RGB2GRAY);


	//SIFT������ȡ������    
	SiftFeatureDetector siftDetector(800);  
	vector<KeyPoint> keyPoint1, keyPoint2;
	siftDetector.detect(image1, keyPoint1);
	siftDetector.detect(image2, keyPoint2);

	//������������Ϊ�±ߵ�������ƥ����׼��    
	SiftDescriptorExtractor SiftDescriptor;
	Mat imageDesc1, imageDesc2;
	SiftDescriptor.compute(image1, keyPoint1, imageDesc1);
	SiftDescriptor.compute(image2, keyPoint2, imageDesc2);

	FlannBasedMatcher matcher;
	vector<vector<DMatch> > matchePoints;
	vector<DMatch> GoodMatchePoints;

	vector<Mat> train_desc(1, imageDesc1);
	matcher.add(train_desc);
	matcher.train();

	matcher.knnMatch(imageDesc2, matchePoints, 2);
	cout << "total match points: " << matchePoints.size() << endl;

	// Lowe's algorithm,��ȡ����ƥ���
	for (int i = 0; i < matchePoints.size(); i++)
	{
		if (matchePoints[i][0].distance < 0.6 * matchePoints[i][1].distance)
		{
			GoodMatchePoints.push_back(matchePoints[i][0]);
		}
	}

	Mat first_match;
	drawMatches(image02, keyPoint2, image01, keyPoint1, GoodMatchePoints, first_match);
	namedWindow("match ", 0);
	imshow("match ", first_match);
	imwrite("match.jpg", first_match);
	waitKey();
	return 0;
}
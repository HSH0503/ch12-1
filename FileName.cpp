// **********************************************
// 제 목 : 실습과제1
// 날 짜 : 2024년 5월13일
// 작성자 : 2001237 한세훈
// **********************************************
/*
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main() 
{
	Mat src = imread("labelex.png", IMREAD_GRAYSCALE);
	Mat bin, labels, stats, mid;
	double x, y;
	threshold(src, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	int cnt = connectedComponentsWithStats(bin, labels, stats, mid);
	double max_x = mid.at<double>(1, 0), max_y = mid.at<double>(1, 1), min_x = mid.at<double>(1, 0), min_y = mid.at<double>(1, 1);
	int area, max_area = 0, max = 1, min_area = 0, min = 1;
	for (int i = 1; i < stats.rows; i++) 
	{
		x = mid.at<double>(i, 0);
		y = mid.at<double>(i, 1);
		area = stats.at<int>(i, 4);
		if (area > max_area) {max_area = area;max = i;max_x = x;max_y = y;}
		if (area < min_area) {min_area = area;min = i;min_x = x;min_y = y;}
	}
	cvtColor(src, src, COLOR_GRAY2BGR);
	rectangle(src, Rect(stats.at<int>(min, 0), stats.at<int>(min, 1), stats.at<int>(min, 2), stats.at<int>(min, 3)), Scalar(255, 0, 0));
	rectangle(src, Rect(stats.at<int>(max, 0), stats.at<int>(max, 1), stats.at<int>(max, 2), stats.at<int>(max, 3)), Scalar(0, 0, 255));
	cout << "면적이 최대인 객체의 레이블:" << max << endl;
	cout << "무게중심:(x,y): " << max_x << "\t" << max_y << endl;
	cout << "면적이 최소인 객체의 레이블:" << min << endl;
	cout << "무게중심:(x,y): " << min_x << "\t" << min_y << endl;
	imshow("src", src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
*/

// **********************************************
// 제 목 : 실습과제2
// 날 짜 : 2024년 5월13일
// 작성자 : 2001237 한세훈
// **********************************************
/*
#include <opencv2/opencv.hpp> 
#include <iostream>
using namespace cv;
using namespace std;
int main() 
{
	Mat src = imread("polygon.bmp");
	Mat gray, bin, labels, stats, mid;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, bin, 150, 255, THRESH_BINARY_INV);
	int cnt = connectedComponentsWithStats(bin, labels, stats, mid);
	cout << "갯수 " << cnt << endl;
	cout << "label x y width height area color[B,G,R]" << endl;
	for (int i = 1; i < cnt; i++) 
	{
		double x = mid.at<double>(i, 0);
		double y = mid.at<double>(i, 1);
		rectangle(src, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(0, 255, 255));
		putText(src, to_string(i), Point(x, y),	FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
		cout << i << " " << stats.at<int>(i, 0) << " " << stats.at<int>(i, 1) << " "
			<< stats.at<int>(i, 2) << " " << stats.at<int>(i, 3) << " " << stats.at<int>(i, 4)<< " " << src.at<Vec3b>(y, x) << endl;
	}
	imshow("src", src);
	imshow("gray", gray);
	imshow("bin", bin);
	waitKey(0);
	return 0;
}
*/

// **********************************************
// 제 목 : 실습과제3
// 날 짜 : 2024년 5월13일
// 작성자 : 2001237 한세훈
// **********************************************
/*
#include <opencv2/opencv.hpp>
#include <iostream> 
using namespace cv;
using namespace std;
int main() 
{ 
	Mat src = imread("polygon.bmp");
	if (src.empty()) { cerr << "failed" << endl; return -1; }
	Mat gray, bin, labels, stats, centroids;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, bin, 150, 255, THRESH_BINARY_INV);
	double cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
	for (int i = 1; i < cnt; i++) 
	{
		int count = 0;
		double xsum = 0, ysum = 0;
		for (double x = stats.at<int>(i, 0); x < stats.at<int>(i, 0) + stats.at<int>(i, 2); x++) 
		{
			for (double y = stats.at<int>(i, 1); y < stats.at<int>(i, 1) + stats.at<int>(i, 3); y++) 
			{ 
				if (bin.at<uchar>(y, x) == 255) 
				{
					count++;
					xsum += x; 
					ysum += y; 
				}
			}
		}
		cout << i << "번 객체의 무게중심:( " << xsum / count << ", " << ysum / count << ")" << endl;
	}
	return 0;
} 
*/

// **********************************************
// 제 목 : 실습과제4
// 날 짜 : 2024년 5월13일
// 작성자 : 2001237 한세훈
// **********************************************
/*
#include <opencv2/opencv.hpp> 
#include <iostream>
using namespace cv;
using namespace std;
int main() 
{
	Mat src = imread("car.jpg");
	if (src.empty()) { cerr << "이미지를 불러오는 데 실패하였습니다!" << endl; return -1; }
	Mat gray, blurred, edges, binary, closed, labels, stats, centroids, dst;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	blur(gray, blurred, Size(5, 5));
	Sobel(blurred, edges, CV_8U, 1, 0, 3);
	threshold(edges, binary, 150, 255, THRESH_BINARY);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(50, 5));
	morphologyEx(binary, closed, MORPH_CLOSE, kernel);
	int cnt = connectedComponentsWithStats(closed, labels, stats, centroids);
	cvtColor(src, dst, COLOR_BGR2BGRA);
	for (int i = 1; i < cnt; ++i) 
	{
		int area = stats.at<int>(i, 4);
		if (area > 2000) 
		{
			int x = stats.at<int>(i, 0),y = stats.at<int>(i, 1),w = stats.at<int>(i, 2),h = stats.at<int>(i, 3);
			rectangle(src, Point(x, y), Point(x + w, y + h), Scalar(0, 0, 255), 2);
		}
	}
	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	return 0;
}
*/

#include <opencv\cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0,0,255))
{
    if(line[1]!=0)
    {
        float m = -1/tan(line[1]);

        float c = line[0]/sin(line[1]);

        cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width+c), rgb);
    }
    else
    {
        cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
    }

}

void mergeRelatedLines(vector<Vec2f> *lines,Mat &images)
{
	vector<Vec2f>::iterator current;
	for(current = lines->begin(); current > lines->end(); current++)
	{
		if((*current)[0]==0 && (*current)[1]==-100) continue;

		float p1 = (*current)[0];
		float theta1 = (*current)[1];

		Point pt1current,pt2current;



	}
}

int main()
{
	Mat sudoku = imread("E://Images//T9K2.jpg",0);
	Mat outerBox = Mat(sudoku.size(),CV_8UC1);

	GaussianBlur(sudoku,sudoku,Size(11,11),0);
	adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 25, 2);

	//imshow("Gaussian blur & Thresholding", sudoku);

	bitwise_not(outerBox, outerBox);

	//imshow("Bitwise not", outerBox);

	Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
	dilate(outerBox,outerBox,kernel);

	//imshow("Dilated outerbox",outerBox);

	int count = 0;
	int max = -1;

	Point maxPt;

	for(int y = 0; y < outerBox.size().height; y++)
	{
		uchar *row = outerBox.ptr(y);
		for(int x = 0; x < outerBox.size().width; x++)
		{
			if(row[x] >= 128)
			{
				int area = floodFill(outerBox, Point(x,y), CV_RGB(0,0,64));

				if(area > max)
				{
					maxPt = Point(x,y);
					max  = area; 
				}
			}
		}
	}

	floodFill(outerBox, maxPt, CV_RGB(255,255,255));
	 
	for(int y = 0; y < outerBox.size().height; y++)
	{
		uchar* row = outerBox.ptr(y);

		for(int x = 0; x < outerBox.size().width; x++)
		{
			 if(row[x]==64 && x!=maxPt.x && y!=maxPt.y)
            {
                int area = floodFill(outerBox, Point(x,y), CV_RGB(0,0,0));
            }
		}
	}

	erode(outerBox, outerBox, kernel);
    imshow("thresholded", outerBox);

	vector<Vec2f> lines;
	HoughLines(outerBox, lines, 1, CV_PI/180,200);

	for(int i = 0; i < lines.size(); i++)
	{
		drawLine(lines[i], outerBox, CV_RGB(0,0,128));
	}

	imshow("lined",outerBox);

	waitKey(0);
	return 0;
}
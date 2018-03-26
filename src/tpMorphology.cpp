#include "tpMorphology.h"
#include <cmath>
#include <algorithm>
#include <tuple>
#include <limits>
#include "common.h"
using namespace cv;
using namespace std;


/**
  Compute a median filter of the input float image.
  The filter window is a square of (2*size+1)*(2*size+1) pixels.

  Values outside image domain are ignored.

  The median of a list l of n>2 elements is defined as:
  - l[n/2] if n is odd
  - (l[n/2-1]+l[n/2])/2 is n is even
  */
Mat median(Mat image, int size)
{
    Mat res = image.clone();
    assert(size>0);

    Rect imageRect = Rect(0, 0, image.cols, image.rows);
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            Point2i pixel = {y, x};
            std::vector<float> values;

            for (int i = -size; i <= size; ++i) {
                for (int j = -size; j <= size; ++j) {

                    Point2i neighbour = {j, i};
                    neighbour += pixel;

                    if (!neighbour.inside(imageRect)) continue;

                    values.push_back(image.at<float>(neighbour));
                }
            }
            std::sort(values.begin(), values.end());

            int value;
            if (values.size() % 2 == 0) {
                value = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
            } else {
                value = values[values.size() / 2];
            }
            res.at<float>(x, y) = value;
        }
    }

    return res;
}


/**
  Compute the erosion of the input float image by the given structuring element.
  Pixel outside the image are supposed to have value 1.
  */
Mat erode(Mat image, Mat structuringElement)
{
    Mat res = image.clone();
    /********************************************
      YOUR CODE HERE
     *********************************************/

    /********************************************
      END OF YOUR CODE
     *********************************************/
    return res;
}


/**
  Compute the dilation of the input float image by the given structuring element.
  Pixel outside the image are supposed to have value 0
  */
Mat dilate(Mat image, Mat structuringElement)
{
    Mat res = Mat::zeros(1,1,CV_32FC1);
    /********************************************
      YOUR CODE HERE
hint : 1 line of code is enough
     *********************************************/

    /********************************************
      END OF YOUR CODE
     *********************************************/
    return res;
}


/**
  Compute the opening of the input float image by the given structuring element.
  */
Mat open(Mat image, Mat structuringElement)
{

    Mat res = Mat::zeros(1,1,CV_32FC1);
    /********************************************
      YOUR CODE HERE
hint : 1 line of code is enough
     *********************************************/

    /********************************************
      END OF YOUR CODE
     *********************************************/
    return res;
}


/**
  Compute the closing of the input float image by the given structuring element.
  */
Mat close(Mat image, Mat structuringElement)
{

    Mat res = Mat::zeros(1,1,CV_32FC1);
    /********************************************
      YOUR CODE HERE
hint : 1 line of code is enough
     *********************************************/

    /********************************************
      END OF YOUR CODE
     *********************************************/
    return res;
}


/**
  Compute the morphological gradient of the input float image by the given structuring element.
  */
Mat morphologicalGradient(Mat image, Mat structuringElement)
{

    Mat res = Mat::zeros(1,1,CV_32FC1);
    /********************************************
      YOUR CODE HERE
hint : 1 line of code is enough
     *********************************************/

    /********************************************
      END OF YOUR CODE
     *********************************************/
    return res;
}


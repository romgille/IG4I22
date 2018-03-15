#include "tpGeometry.h"
#include <cmath>
#include <algorithm>
#include <tuple>
using namespace cv;
using namespace std;

/**
  Transpose the input image,
  ie. performs a planar symmetry according to the
  first diagonal (upper left to lower right corner).
  */
Mat transpose(Mat image)
{
    Mat res = Mat::zeros(1,1,CV_32FC1);

    resize(res, res, Size(image.rows, image.cols));

    for (int y = 0; y < res.rows; ++y)
        for (int x = 0; x < res.cols; ++x)
            res.at<int>(y, x) = image.at<int>(x, y);

    return res;
}

/**
  Compute the value of a nearest neighbour interpolation
  in image Mat at position (x,y)
  */
float interpolate_nearest(Mat image, float y, float x)
{
    float v=0;

    int vx = (round(x) > image.cols - 1) ? x : round(x);
    int vy = (round(y) > image.rows - 1) ? y : round(y);

    return image.at<float>(vy, vx);
}


/**
  Compute the value of a bilinear interpolation in image Mat at position (x,y)
  */
float interpolate_bilinear(Mat image, float y, float x)
{
    float v=0;
    float x1 = floor(x);
    float y1 = floor(y);
    float x2 = x1 + 1;
    float y2 = y1 + 1;
    float alpha = (x - x1)/(x2 - x1);
    float beta = (y - y1)/(y2 - y1);

    return
        (1 - alpha) * (1 - beta) * image.at<float>(y1, x1) +
        alpha * (1 - beta) * image.at<float>(y1, x2) +
        beta * (1 - alpha) * image.at<float>(y2, x1) +
        alpha * beta * image.at<float>(y2, x2);
}

/**
  Multiply the image resolution by a given factor using the given interpolation method.
  If the input size is (h,w) the output size shall be ((h-1)*factor, (w-1)*factor)
  */
Mat expand(Mat image, int factor, float(* interpolationFunction)(cv::Mat image, float y, float x))
{
    assert(factor>0);
    Mat res = Mat::zeros((image.rows-1)*factor,(image.cols-1)*factor,CV_32FC1);

    for (float y = 0; y < res.rows; ++y)
        for (float x = 0; x < res.cols; ++x)
            res.at<float>(y, x) = interpolationFunction(image, y/factor, x/factor);

    return res;
}

/**
  Performs a rotation of the input image with the given angle (clockwise) and the given interpolation method.
  The center of rotation is the center of the image.

  Ouput size depends of the input image size and the rotation angle.

  Output pixels that map outside the input image are set to 0.
  */
Mat rotate(Mat image, float angle, float(* interpolationFunction)(cv::Mat image, float y, float x))
{
    Mat res = Mat::zeros(1,1,CV_32FC1);
    /********************************************
      YOUR CODE HERE
hint: to determine the size of the output, take
the bounding box of the rotated corners of the
input image.
     *********************************************/

    /********************************************
      END OF YOUR CODE
     *********************************************/
    return res;

}

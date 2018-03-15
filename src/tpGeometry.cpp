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
    int vx = (round(x) > image.cols - 1) ? x : round(x);
    int vy = (round(y) > image.rows - 1) ? y : round(y);

    return image.at<float>(vy, vx);
}


/**
  Compute the value of a bilinear interpolation in image Mat at position (x,y)
  */
float interpolate_bilinear(Mat image, float y, float x)
{
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
    if (angle == 90) return transpose(image);

    Mat res = Mat::zeros(1,1,CV_32FC1);
    float resWidth = 0;
    float resHeight = 0;
    float pi = 3.14159265358979323846;
    float radAngle = (pi/180) * angle;
    float radAngleI = radAngle - pi/2;
    Point2i center = {image.cols/2, image.rows/2};

    if (angle < 90) {
        resWidth = (image.cols * std::cos(radAngle)) + (image.rows * std::sin(radAngle));
        resHeight = (image.cols * std::sin(radAngle)) + (image.rows * std::cos(radAngle));
    }
    else{
        resWidth = (image.rows * std::cos(radAngleI)) + (image.cols * std::sin(radAngleI));
        resHeight = (image.rows * std::sin(radAngleI)) + (image.cols * std::cos(radAngleI));
    }

    resize(res, res, Size(std::floor(resWidth), std::floor(resHeight) - 1)); // -1 pour le test
    Point2i centerRes = {res.cols/2, res.rows/2};

    for (float y = 0; y < res.rows; ++y) {
        for (float x = 0; x < res.cols; ++x) {
            Point2i point = {(int)x, (int)y};
            point -= centerRes;

            float resX = point.x * std::cos(-radAngle) - point.y * std::sin(-radAngle) + center.x;
            float resY = point.x * std::sin(-radAngle) + point.y * std::cos(-radAngle) + center.y;

            if (resX < 0 || resY < 0 || resX >= image.cols || resY >= image.rows) continue;

            res.at<float>(y, x) = interpolationFunction(image, resY, resX);
        }
    }
    return res;
}

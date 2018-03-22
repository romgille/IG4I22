#include "tpConvolution.h"
#include <cmath>
#include <algorithm>
#include <tuple>
using namespace cv;
using namespace std;

/**
  Compute a mean filter of size 2k+1.

  Pixel values outside of the image domain are supposed to have a zero value.
  */
cv::Mat meanFilter(cv::Mat image, int size){
    Mat res = image.clone();

    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            float sum = 0;
            Point2i pixel = {x, y};
            Rect imageRect = Rect(0, 0, res.cols, res.rows);

            for (int i = -size; i <= size; ++i) {
                for (int j = -size; j <= size; ++j) {

                    Point2i neighbour = {j, i};
                    neighbour += pixel;

                    if (!neighbour.inside(imageRect)) continue;

                    sum += image.at<float>(neighbour.y, neighbour.x);
                }
            }
            res.at<float>(y, x) = sum / std::pow(2 * size + 1, 2);
        }
    }

    return res;
}

/**
  Compute the convolution of a float image by kernel.
  Result has the same size as image.

  Pixel values outside of the image domain are supposed to have a zero value.
  */
Mat convolution(Mat image, cv::Mat kernel)
{
    Mat res = image.clone();
    int size = (kernel.rows - 1) / 2;
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            float sum = 0;
            int count = -1;
            Point2i pixel = {x, y};
            Rect imageRect = Rect(0, 0, res.cols, res.rows);

            for (int i = -size; i <= size; ++i) {
                for (int j = -size; j <= size; ++j) {

                    Point2i neighbour = {j, i};
                    neighbour += pixel;
                    count++;

                    if (!neighbour.inside(imageRect)) continue;

                    sum += image.at<float>(neighbour.y, neighbour.x) *
                        kernel.at<float>(count%kernel.rows, count/kernel.rows);
                }
            }
            res.at<float>(y, x) = sum;
        }
    }

    return res;
}

/**
  Compute the sum of absolute partial derivative according to Sobel's method
  */
cv::Mat edgeSobel(cv::Mat image)
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
  Value of a centered gaussian of variance (scale) sigma at point x.
  */
float gaussian(float x, float sigma2)
{
    return 1.0/(2*M_PI*sigma2)*exp(-x*x/(2*sigma2));
}

/**
  Performs a bilateral filter with the given spatial smoothing kernel 
  and a intensity smoothing of scale sigma_r.

*/
cv::Mat bilateralFilter(cv::Mat image, cv::Mat kernel, double sigma_r)
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

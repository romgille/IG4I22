#include "tpHistogram.h"
#include <cmath>
#include <algorithm>
#include <tuple>
using namespace cv;


/**
  Inverse a grayscale image with float values.
  for all pixel p: res(p) = 1.0 - image(p)
  */
Mat inverse(Mat image)
{
    // clone original image
    Mat res = image.clone();

    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            res.at<float>(y, x) = 1.0 - image.at<float>(y, x);

    return res;
}


/**
  Thresholds a grayscale image with float values.
  for all pixel p: res(p) =
  | 0 if image(p) <= lowT
  | image(p) if lowT < image(p) <= hightT
  | 1 otherwise
  */
Mat threshold(Mat image, float lowT, float highT)
{
    assert(lowT <= highT);
    Mat res = image.clone();

    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x < image.cols; x++) {
            if (image.at<float>(y, x) <= lowT)      res.at<float>(y, x) = 0;
            else if (image.at<float>(y, x) > highT) res.at<float>(y, x) = 1;
        }
    }

    return res;
}


/**
  Quantize the input float image in [0,1] in numberOfLevels different gray levels.
  eg. for numberOfLevels = 3 the result should be for all pixel p: res(p) =
  | 0 if image(p) < 1/3
  | 1/2 if 1/3 <= image(p) < 2/3
  | 1 otherwise
  */
Mat quantize(Mat image, int numberOfLevels)
{
    assert(numberOfLevels>0);
    Mat res = image.clone();

    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x < image.cols; x++) {
            float interval = std::floor(res.at<float>(y, x) * numberOfLevels);
            float result = (interval / (numberOfLevels - 1));
            res.at<float>(y, x) = result > 1 ? 1 : result;
        }
    }

    return res;
}


/**
  Normalize a grayscale image with float values
  Target range is [minValue, maxValue].
  */
Mat normalize(Mat image, float minValue, float maxValue)
{
    assert(minValue <= maxValue);
    Mat res = image.clone();
    float fmin = 1, fmax = 0;

    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x < image.cols; x++) {
            fmin = image.at<float>(y, x) < fmin ? image.at<float>(y, x) : fmin;
            fmax = image.at<float>(y, x) > fmax ? image.at<float>(y, x) : fmax;
        }
    }

    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x < image.cols; x++) {
            float relation = (maxValue - minValue) / (fmax - fmin);
            res.at<float>(y, x) = (image.at<float>(y, x) - fmin) * relation + minValue;
        }
    }

    return res;
}


/**
  Equalize image histogram with unsigned char values ([0;255])
  */
Mat equalize(Mat image)
{
    Mat res = image.clone();

    uchar vmin = 0, vmax = 255;
    int total = image.rows * image.cols, cf[256] = {0};

    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            cf[image.at<uchar>(y, x)] += 1;

    for(int i = 1; i < 256; ++i) cf[i] += cf[i - 1];

    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x < image.cols; x++) {
            float average = (float)(vmax - vmin) / (float)total;
            res.at<uchar>(y, x) = round(average * cf[image.at<uchar>(y, x)] + vmin);
        }
    }

    return res;
}


/**
  Compute a binarization of the input float image using an automatic Otsu threshold.
  Input image is of type unsigned char ([0;255])
  */
Mat thresholdOtsu(Mat image)
{
    Mat res = image.clone();
    int imgSize = res.rows * res.cols; // nombre de pixels
    int threshold = 0, q1 = 0, q2 = 0, histo[256] = {0};
    float sum = 0, sumB = 0, varMax = 0;

    for(int y = 0; y < res.rows; y++)
        for(int x = 0; x < res.cols; x++)
            histo[res.at<uchar>(y, x)]++;

    for(int i = 0; i < 256; i++)
        sum += i * histo[i];

    for(int i = 0; i < 256; i++){
        q1 += histo[i];

        if(q1 == 0) continue;

        q2 = imgSize - q1;

        if(q2 == 0) break;

        sumB += (float)(i *histo[i]);
        float m1 = sumB / q1, m2 = (sum - sumB) / q2;
        float varBetween = (float) q1 * (float) q2 * (m1 - m2) * (m1 - m2);

        if(varBetween >= varMax){
            threshold = i;
            varMax = varBetween;
        }
    }

    for(int y = 0; y < res.rows; y++){
        for(int x = 0; x < res.cols; x++){
            if(res.at<uchar>(y, x) > threshold) res.at<uchar>(y, x) = 255;
            else                                res.at<uchar>(y, x) = 0;
        }
    }

    return res;
}

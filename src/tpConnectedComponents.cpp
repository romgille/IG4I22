#include "tpConnectedComponents.h"
#include <cmath>
#include <algorithm>
#include <tuple>
#include <vector>
using namespace cv;
using namespace std;


/**
  Performs a labeling of image connected component with 4 connectivity
  with a depth-first exploration.
  Any non zero pixel of the image is considered as present.
  */
cv::Mat ccLabel(cv::Mat image)
{
    Mat res = Mat::zeros(image.rows, image.cols, CV_32SC1); // 32 int image
    int visited[image.rows][image.cols];
    std::vector<Point2i> stack;
    std::vector<Point2i> neighbours = {{-1,0}, {0,-1}, {0,1}, {1,0}};
    int group = 1;

    /* assign all pixels at not visited */
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            visited[y][x] = 0;

    /* main loop */
    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x < image.cols; x++) {
            /* if pixel is already visited, continue */
            if(visited[y][x] == 1) continue;

            /* if pixel is black continue */
            if(image.at<int>(y, x) == 0) continue;

            /* add curent pixel to stack */
            stack.push_back({y, x});

            /* while there is neighbours to visit */
            while(!stack.empty()) {
                /* set current pixel */
                Point2i pixel = stack.front();
                stack.erase(stack.begin());

                /* for each neighbour of the current pixel */
                for(Point2i neighbour: neighbours) {
                    neighbour += pixel;

                    /* if we are on the img and the pixel is not visited and the color of the
                     * neighbour is the same as the pixel's */
                    if (neighbour.y > 0 && neighbour.x > 0 &&
                            neighbour.x < image.rows && neighbour.y < image.cols &&
                            visited[neighbour.x][neighbour.y] == 0 &&
                            image.at<float>(y, x) == image.at<float>(neighbour.x, neighbour.y)) {
                        /* set the pixel to visited */
                        visited[neighbour.x][neighbour.y] = 1;
                        /* add it to the stack*/
                        stack.push_back(neighbour);
                        /* set group to the neighbour */
                        res.at<int>(neighbour.x, neighbour.y) = group;
                    }
                }
            }
            /* set pixel to visited and set group */
            visited[y][x] = 1;
            res.at<int>(y, x) = group;
            group++;
        }
    }

    return res;
}

/**
  Deletes the connected components (4 connectivity) containg less than size pixels.
  */
cv::Mat ccAreaFilter(cv::Mat image, int size)
{
    Mat res = Mat::zeros(image.rows, image.cols, image.type());
    assert(size>0);

    int visited[image.rows][image.cols];
    std::vector<Point2i> stack;
    std::vector<Point2i> neighbours = {{-1,0}, {0,-1}, {0,1}, {1,0}};
    int group = 1;
    int numberOfPixels = 0;
    std::vector<Point2i> pixelsSave;

    /* assign all pixels at not visited */
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            visited[y][x] = 0;

    /* main loop */
    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x < image.cols; x++) {
            /* if pixel is already visited, continue */
            if(visited[y][x] == 1) continue;

            /* if pixel is black continue */
            if(image.at<float>(y, x) == 0) continue;

            /* add curent pixel to stack */
            stack.push_back({y, x});

            numberOfPixels = 0;
            pixelsSave.clear();

            /* while there is neighbours to visit */
            while(!stack.empty()) {
                /* set current pixel */
                Point2i pixel = stack.front();
                stack.erase(stack.begin());
                pixelsSave.push_back(pixel);
                numberOfPixels++;

                /* for each neighbour of the current pixel */
                for(Point2i neighbour: neighbours) {
                    neighbour += pixel;

                    /* if we are on the img and the pixel is not visited and the color of the
                     * neighbour is the same as the pixel's */
                    if (neighbour.y > 0 && neighbour.x > 0 &&
                            neighbour.x < image.rows && neighbour.y < image.cols &&
                            visited[neighbour.x][neighbour.y] == 0 &&
                            image.at<float>(y, x) == image.at<float>(neighbour.x, neighbour.y)) {
                        /* set the pixel to visited */
                        visited[neighbour.x][neighbour.y] = 1;
                        /* add it to the stack*/
                        stack.push_back(neighbour);
                        /* set group to the neighbour */
                        res.at<float>(neighbour.x, neighbour.y) = group;
                    }
                }
            }
            /* set pixel to visited and set group */
            visited[y][x] = 1;
            res.at<float>(y, x) = group;
            group++;

            if (numberOfPixels < size)
               for (Point2i pixelSave : pixelsSave)
                    res.at<float>(pixelSave.x, pixelSave.y) = 0;
        }
    }

    return res;
}

/**
  Performs a labeling of image connected component with 4 connectivity using a
  2 pass algorithm.
  Any non zero pixel of the image is considered as present.
  */
cv::Mat ccTwoPassLabel(cv::Mat image)
{
    Mat res = Mat::zeros(image.rows, image.cols, CV_32SC1); // 32 int image
    /********************************************
      YOUR CODE HERE
     *********************************************/

    /********************************************
      END OF YOUR CODE
     *********************************************/
    return res;

}

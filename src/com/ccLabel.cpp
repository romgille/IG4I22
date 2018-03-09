
#include "../common.h"
#include "../tpConnectedComponents.h"

namespace po = boost::program_options;
using namespace cv;

int main( int argc, char** argv )
{
    po::variables_map vm=parseArgs(argc, argv, {"inputImage", "outputImage"});

     // custom loading function defined in common.cpp
    Mat image = imreadHelper(vm["inputImage"].as<std::string>());


    Mat res_image = ccLabel(image);

    Mat tmp;
    res_image.convertTo(tmp, CV_32FC1);
    cv::normalize(tmp,res_image,0.0,1.0,NORM_MINMAX,CV_32FC1);
    // custom writing function defined in common.cpp
    imwriteHelper(res_image, vm["outputImage"].as<std::string>());

    // maybe show result
    if (vm.count("show")) {
        showimage(image, "Input Image");
        showimage(res_image, "Output Image");
        waitKey(0);
    }


    return 0;
}


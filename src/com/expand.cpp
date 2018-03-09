
#include "../common.h"
#include "../tpGeometry.h"

namespace po = boost::program_options;
using namespace cv;

int main( int argc, char** argv )
{
    po::variables_map vm = parseArgs(argc, argv, {"inputImage", "outputImage", "interpolation","sizeFactor"});

     // custom loading function defined in common.cpp
    Mat image = imreadHelper(vm["inputImage"].as<std::string>());

    float (* interpolationMethod)(Mat, float, float);
    if(vm["interpolation"].as<std::string>().compare("bilinear")==0)
        interpolationMethod = interpolate_bilinear;
    else if(vm["interpolation"].as<std::string>().compare("nearest")==0)
        interpolationMethod = interpolate_nearest;
    else
    {
        std::cerr << "Interpolation method unknown:" << vm["interpolation"].as<std::string>() << std::endl;
        exit(1);
    }

    Mat gray_image = expand(image, vm["sizeFactor"].as<int>(), interpolationMethod);

    // custom writing function defined in common.cpp
    imwriteHelper(gray_image, vm["outputImage"].as<std::string>());

    // maybe show result
    if (vm.count("show")) {
        showimage(image, "Input Image");
        showimage(gray_image, "Output Image");
        waitKey(0);
    }


    return 0;
}


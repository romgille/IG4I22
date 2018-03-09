
#include "../common.h"
#include "../tpConvolution.h"

namespace po = boost::program_options;
using namespace cv;

int main( int argc, char** argv )
{
    po::variables_map vm=parseArgs(argc, argv, {"inputImage", "outputImage","filterSize"});

     // custom loading function defined in common.cpp
    Mat image = imreadHelper(vm["inputImage"].as<std::string>());
    int filterSize = vm["filterSize"].as<int>();


    Mat res_image = meanFilter(image, filterSize);

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


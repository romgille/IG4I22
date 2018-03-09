
#include "../common.h"
#include "../tpConvolution.h"

namespace po = boost::program_options;
using namespace cv;

int main( int argc, char** argv )
{
    po::variables_map vm = parseArgs(
            argc,
            argv,
            {"inputImage", "outputImage","kernel","intensityScale"}
    );

     // custom loading function defined in common.cpp
    Mat image = imreadHelper(vm["inputImage"].as<std::string>());
    Mat kernel = imreadHelper(vm["kernel"].as<std::string>());
    float scale = vm["intensityScale"].as<float>();

    kernel = kernel / sum(kernel)[0];

    Mat res_image = bilateralFilter(image, kernel, scale);

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


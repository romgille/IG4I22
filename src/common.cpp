#include "common.h"
#include <exception>
#include <iostream>
#include "stdio.h"

using namespace cv;
using namespace std;
namespace po = boost::program_options;

cv::Mat imreadHelper(std::string filename, bool forceFloat, bool forceGrayScale)
{
    cv::Mat image;
    image = cv::imread( filename.c_str(), (forceGrayScale)? 0 : -1 );

    if( !image.data )
    {
        throw std::runtime_error("No Image Data");
    }
    cv::Mat tmp;

    if(forceFloat)
    {
        int channels = image.channels();
        int depth = image.depth();
        int targetType;
        if(channels == 1)
            targetType = CV_32FC1;
        else if(channels == 3)
            targetType = CV_32FC3;
        else
            throw std::runtime_error("Unsupported number of channels");


        image.convertTo(tmp, targetType);

        if(depth<=1)
            tmp /= 255.0;
    } else {
        tmp = image;
    }


    return tmp;

}

void imwriteHelper(cv::Mat image, std::string filename)
{
    int depth = image.depth();
    if(depth<=1)
    {
        cv::imwrite(filename.c_str(), image);
    } else {
        double min, max;
        cv::minMaxLoc(image, &min, &max);
        if(min < -0.000001 || max > 1.000001)
            std::cerr << "!!!  Warning, saved image values not between 0 and 1." << std::endl;

        cv::Mat tmp = image.clone();
        tmp *= 255;

        cv::imwrite(filename.c_str(), tmp);
    }

}

void showimage(cv::Mat image, const char * name)
{
    static int count = 1;
    const char * tn;
    if (name==NULL)
    {
         char op[30];
         sprintf(op, "%d", count++);
         tn = op;
    }else
    {
        tn = name;
    }

    cv::namedWindow( tn, cv::WINDOW_AUTOSIZE );
    cv::imshow( tn, image );
}

boost::program_options::variables_map parseArgs(int argc, char ** argv, initializer_list<string> arglist)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help","Produce help message")
        ("show,S", "Open windows to show intermediate images and results.")
        ;

    for (string s : arglist)
    {
        if (s.compare("inputImage")==0)
            desc.add_options()("inputImage,I", po::value<string>()->default_value("cat.jpg"), "Input image file");
        else if (s.compare("outputImage")==0)
            desc.add_options()("outputImage,O", po::value<string>()->default_value("out.jpg"), "Ouput image file");
        else if (s.compare("thresholdLow")==0)
            desc.add_options()("thresholdLow,L", po::value<float>()->default_value(0), "Low threshold");
        else if (s.compare("thresholdHigh")==0)
            desc.add_options()("thresholdHigh,H", po::value<float>()->default_value(1), "High threshold");
        else if (s.compare("quantizeLevel")==0)
            desc.add_options()("quantizeLevel,Q", po::value<int>()->default_value(3), "Number of quantization levels");
        else if (s.compare("rotationAngle")==0)
            desc.add_options()("rotationAngle,A", po::value<float>()->default_value(20), "Rotation angle (degree)");
        else if (s.compare("interpolation")==0)
            desc.add_options()("interpolation,P", po::value<string>()->default_value("bilinear"), "Interpolation method ('nearest' or 'bilinear')");
        else if (s.compare("extrapolation")==0)
            desc.add_options()("extrapolation,X", po::value<string>()->default_value("mirror"), "Extrapolation method ('zero', nearest' or 'mirror')");
        else if (s.compare("areaThreshold")==0)
            desc.add_options()("areaThreshold,F", po::value<int>()->default_value(50), "Size limit for area filter");
        else if (s.compare("structuringElement")==0)
            desc.add_options()("structuringElement,E", po::value<string>()->default_value("morphoCross.png"), "Structuring element");
        else if (s.compare("kernel")==0)
            desc.add_options()("kernel,K", po::value<string>()->default_value("maskGauss5x5.png"), "Convolution kernel");
        else if (s.compare("filterSize")==0)
            desc.add_options()("filterSize,M", po::value<int>()->default_value(5), "Filter size ((X*2+1)*(X*2+1) square)");
        else if (s.compare("intensityScale")==0)
            desc.add_options()("intensityScale,C", po::value<float>()->default_value(0.1f), "Intensity scale (Gaussian standard deviation)");
        else if (s.compare("sizeFactor")==0)
            desc.add_options()("sizeFactor,F", po::value<int>()->default_value(3), "Each dimension d of size sd is increased to size (sd-1)*sizeFactor");
        else {
            throw std::runtime_error(string("parseArgs: unknown parameter name : ") + s);
        }
    }

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);


    if (vm.count("help")) {
        cout << desc << endl;
        exit(0);
    }


   return vm;


}

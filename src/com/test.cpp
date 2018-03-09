
#include <map>
#include <iostream>
#include "../common.h"
#include <vector>
#include <stdlib.h>
#include <sys/stat.h>
#include <fstream>
#include <exception>


namespace po = boost::program_options;
using namespace cv;
using namespace std;


/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BOLD "\x1B[1m"
#define UNDL "\x1B[4m"




std::string outname = std::string("out.png");

#define SENSITIVITY 1e-5


bool compIm(Mat im1, Mat im2, std::string msg, bool show)
{
    if(im1.cols != im2.cols || im1.rows != im2.rows)
    {
        cerr << "\tDimensions incorrect, command:" << msg << endl;
        return false;
    }

    if(im1.channels() != im2.channels())
    {
        cerr << "\tChannel number incorrect, command:" << msg << endl;
        return false;
    }

    Mat tmp0 = im1 - im2;
    Mat tmp;
    pow(tmp0,2.0,tmp);
    double err = sum(tmp)[0];

    bool flag=false;
    //cout << err << endl;
    if(err > SENSITIVITY)
    {
        cerr <<  "\tRelative error " << err << ": problem detected, command: " << msg << endl;
        flag=true;
    }

    if(show && flag)
    {
        Mat op;
        normalize(tmp,op,0,255,NORM_MINMAX);
        showimage(op, msg.c_str());
        waitKey(0);
    }
    return !flag;
}

bool exists_test (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

void copyFile(std::string source, std::string destination)
{
    ifstream  src(source, std::ifstream::in | std::ifstream::binary);
    std::ofstream  dst(destination, std::ifstream::out | std::ifstream::binary);

    dst << src.rdbuf();
}

void printFail()
{
    cout << KRED << "\tFailed" << RST << endl;

}

void process(std::string name, vector<std::string> coms, bool record, bool show, bool memorycheck)
{
    if(exists_test(name))
    {
        int c=0;
        cout << KCYN << BOLD << name << RST << RST  << endl;
        for(std::string com : coms)
        {
            c++;
            std::string refOutput = std::string("expected_results/") + std::string(name) + to_string(c) + std::string(".png");

            if(!record && !exists_test(refOutput))
            {
                cout << "\tCannot find reference result, command: " << com << endl;
                cout << "\tExpected file is: " << refOutput << endl;
                printFail();
                break;
            }

            if(memorycheck)
            {
                std::string comValgrind = std::string("valgrind --error-exitcode=1 -q --log-file='/dev/null' ") + com;
                int rValgrind  = system (comValgrind.c_str());

                if(rValgrind != 0)
                {
                    cout << "\tValgrind has detected a memory corruption ! command: " << com << endl;
                    printFail();
                    break;
                }
            }

            int r = system (com.c_str());
            if(r!=0)
            {
                cout << "\tReturn code different from 0, command: " << com << endl;
                printFail();
                break;
            }

            if(!exists_test(outname))
            {
                cout << "\tOutput file does not exist, command: " << com << endl;
                printFail();
                break;
            }

            if(record)
            {
                copyFile(outname, refOutput);
                cout << KGRN << "\tok" << RST << endl;
            } else
            {
                bool r = false;
                try
                {
                    Mat refIm = imreadHelper(refOutput,true,false);
                    Mat testIm = imreadHelper(outname,true,false);
                    r = compIm(refIm, testIm, com, show);
                }
                catch (exception& e)
                {
                    cout << "\tException " << e.what() << endl;
                }
                if(r)
                    cout << "\tCommand: "<< com << endl << KGRN << "\tok" << RST << endl;
                else
                    printFail();
            }


        }
    }


}

int main( int argc, char** argv )
{
    map<std::string,vector<std::string>> p;
    p["inverse"] = {"./inverse -I cat.jpg -O out.png"};
    p["normalize"] = {"./normalize -I blobs-bad.png -O out.png"};
    p["ccAreaFilter"] = {"./ccAreaFilter -I binary.png -F 200 -O out.png"};
    p["ccLabel"] = {"./ccLabel -I binary.png -O out.png"};
    p["ccLabel2pass"] = {"./ccLabel2pass -I binary.png -O out.png"};
    p["equalize"] = {"./equalize -I camera_mauvaise_balance.png -O out.png"};
    p["expand"] = {"./expand -I cat.jpg -P nearest -O out.png", "./expand -I cat.jpg -P bilinear -O out.png"};
    p["quantize"] = {"./quantize -I cat.jpg -O out.png"};
    p["rotate"] = {"./rotate -I cat.jpg -A 30 -P nearest -O out.png", "./rotate -I cat.jpg -A 30 -P bilinear -O out.png" };
    p["threshold"] = {"./threshold -I cat.jpg -L 0.2 -H 0.8 -O out.png"};
    p["transpose"] = {"./transpose -I cat.jpg -O out.png"};

    p["convolution"] = {"./convolution -I cat.jpg -O out.png -K maskGauss5x5.png"};
    p["meanFilter"] = {"./meanFilter -I cat.jpg -O out.png"};
    p["edgeSobel"] = {"./edgeSobel -I cat.jpg -O out.png"};
    p["bilateralFilter"] = {"./bilateralFilter -I cat.jpg -K maskGauss5x5.png -O out.png"};

    p["median"] = {"./median -I camera_bruit_poivre_et_sel.png -M 2 -O out.png"};
    p["erode"] = {"./erode -I binary.png -E morphoLineV.png -O out.png",
                    "./erode -I cat.jpg -E morphoLineV.png -O out.png"};
    p["dilate"] = {"./dilate -I binary.png -E morphoLineV.png -O out.png",
                    "./dilate -I cat.jpg -E morphoLineV.png -O out.png"};
    p["open"] = {"./open -I binary.png -E morphoLineV.png -O out.png",
                "./open -I cat.jpg -E morphoLineV.png -O out.png"};
    p["close"] = {"./close -I binary.png -E morphoCircle.png -O out.png",
                "./close -I cat.jpg -E morphoCircle.png -O out.png"};
    p["morphologicalGradient"]  = {"./morphologicalGradient -I binary.png -E morphoCross.png -O out.png",
                                "./morphologicalGradient -I cat.jpg -E morphoCross.png -O out.png"};

    p["thresholdOtsu"] = {"./thresholdOtsu -I cat.jpg -O out.png"};

    /*p["detectRectangle"] = {"./detectRectangle -I cas1.png -O out.png",
                            "./detectRectangle -I cas2.png -O out.png",
                            "./detectRectangle -I cas3.png -O out.png",
                            "./detectRectangle -I cas4.png -O out.png",
                            "./detectRectangle -I cas5.png -O out.png",
                            "./detectRectangle -I cas6.png -O out.png"};

    p["thresholdKMean"] = {"./thresholdKMean -I cat.jpg -O out.png"};

    p["thresholdSigmaClipping"] = {"./thresholdSigmaClipping -I img1-11.tiff -O out.png"};*/




    po::options_description desc("Options");
    desc.add_options()
        ("help","Produce help message")
        ("program,P", po::value<std::string>(), "command to test")
        ("show,S", "show difference map if anomaly detected")
        ("recordmode", "record new test results")
        ("fastmode,F", "disable memory corruption detection")
        ;


    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
        exit(0);
    }
    bool fastmode=false;
    if(vm.count("fastmode")){
        fastmode = true;
    }
    bool record = vm.count("recordmode")>0;
    bool show = vm.count("show")>0;
    if(vm.count("program"))
    {
        std::string prog = vm["program"].as<std::string>();
        if (p.count(prog))
        {
            if(!exists_test(prog))
            {
                cerr << "Cannot find program " << prog << endl;
                exit(1);
            }
            process(prog, p[prog],record, show, !fastmode);
        }else{
            cerr << "Unknown program " << prog << endl;
            exit(1);
        }
    }else{
        for(auto iter = p.begin(); iter != p.end(); ++iter)
        {
            std::string k = iter->first;
            vector<std::string> v = iter->second;
            process(k, v, record, show, !fastmode);


        }

    }




    return 0;
}


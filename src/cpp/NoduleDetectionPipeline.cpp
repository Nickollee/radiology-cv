#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "NoduleDetectionPipeline.hpp"
#include "CSVReader.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ostream>
#include <fstream>

NoduleDetectionPipeline::NoduleDetectionPipeline(std::string metadataFileName)
{
    _metadataFileName = metadataFileName;
}

void NoduleDetectionPipeline::readInMetadata()
{
    CSVReader reader(_metadataFileName, true);
    std::vector < std::vector < std::string > > records = reader.Read(',');

    for (int i = 0; i < records.size(); i++)
    {
        std::vector<std::string> record = records[i];

        std::string filename = record[0];

        std::istringstream is1(record[1]);
        int subtlety;
        is1 >> subtlety;

        std::istringstream is2(record[2]);
        int size;
        is2 >> size;

        std::istringstream is3(record[3]);
        int age;
        is3 >> age;

        std::istringstream is5(record[5]);
        int x;
        is5 >> x;

        std::istringstream is6(record[6]);
        int y;
        is6 >> y;

        bool isMale = record[4] == "TRUE";
        bool isMalignant = record[7] == "TRUE";
        bool hasNodule = false;
        if (record[8].find("TRUE") != std::string::npos)
        {
            hasNodule = true;
        }

        Radiograph xray(filename, subtlety, size, age, isMale, x, y, isMalignant, hasNodule);
        _xrays.push_back(xray);
    }
    std::cout << "Metadata read!";        
}

void NoduleDetectionPipeline::splitTrainTest(double trainSplit, double testSplit)
{
    if (trainSplit <= 0 || trainSplit >= 1 || testSplit <= 0 || testSplit >= 1)
    {
        return;
    }
    srand (time(NULL));
    int numTrain = round(_xrays.size() * trainSplit);
    int numTest = _xrays.size() - numTrain;

    // std::unordered_set<int> selected;
    bool selected[_xrays.size()];
    while (_xraysTrain.size() < numTrain)
    {
        int recordNum;
        recordNum = rand() % _xrays.size();
        // std::unordered_set<int>::const_iterator got = selected.find (recordNum);
        if (!selected[recordNum])
        {
            _xraysTrain.push_back(_xrays[recordNum]);
            selected[recordNum] = true;
        }
    }

    for (int i = 0; i < _xrays.size(); i++)
    {
        if (!selected[i])
        {
            _xraysTest.push_back(_xrays[i]);
        }
    }
    std::cout << "Data split!";
}

int NoduleDetectionPipeline::computeMeanNoduleBoxHeight()
{
    int numNodules, totalHeight;
    for (int i = 0; i < _xraysTrain.size(); i++)
    {
        Radiograph r = _xraysTrain[i];
        if (r.hasNodule())
        {
            numNodules++;
            totalHeight += r.getHeight();
        }
    }
    std::cout << "Mean box nodule height computed!";
    return round(totalHeight/numNodules);
}

void NoduleDetectionPipeline::Prepare(std::string rootDataDir, std::string relativeSourceImgDir, double trainSplit, double testSplit)
{
    readInMetadata();
    splitTrainTest(trainSplit, testSplit);

    std::string positiveFile = rootDataDir + "info.dat";
    std::string negativeFile = rootDataDir + "bg.txt";
    std::ofstream posFile(positiveFile.c_str(), std::ios_base::out);
    std::ofstream negFile(negativeFile.c_str(), std::ios_base::out);
    int h = computeMeanNoduleBoxHeight();

    for (int i = 0; i < _xraysTrain.size(); i++)
    // for (int i = 0; i < _xrays.size(); i++)
    {
        Radiograph r = _xraysTrain[i];
        std::string fn = r.getFilename();
        // Radiograph r = _xrays[i];
        if (r.hasNodule())
        {
            if (posFile.is_open())
            {
                posFile <<  relativeSourceImgDir + fn.substr(1, 12).replace(9, 3, "jpg") + " 1" + " " + IntToString(r.getX()) + " " + IntToString(r.getY()) + " " + IntToString(h) + " " + IntToString(h) + "\n";
            }
        }
        else
        {
            if (negFile.is_open())
            {
                negFile << relativeSourceImgDir + fn.substr(1, 12).replace(9, 3, "jpg") + "\n";
            }
        }
    }

    posFile.close();
    negFile.close();

    std::string sysStr1 = "opencv_createsamples -num 10 -vec " + rootDataDir + "pos.vec -info " + rootDataDir + "info.dat";
    std::system(sysStr1.c_str());

    std::cout << "Data prepared!";
}

void NoduleDetectionPipeline::Train(std::string posVectorFile, std::string negFile, std::string modelDestDir)
{
    //int numPos = round(_xraysTrain.size() - 1);
    //int numNeg = numPos * 4;
    int numPos = 1;
    int numNeg = 5;
    std::string sysStr2 = "opencv_traincascade -data haarcascade_nodule_cxr.xml -vec " + posVectorFile + " -bg " + negFile + " -w " + IntToString(computeMeanNoduleBoxHeight()) + " -h " + IntToString(computeMeanNoduleBoxHeight()) + " -numPos " + IntToString(numPos) + " -numNeg " + IntToString(numNeg) + " -precalcValBufSize 1024 -precalcIdxBufSize 1024 -featureType HAAR";
    std::system(sysStr2.c_str());
    std::cout << "Model trained!";
    return;
}

void NoduleDetectionPipeline::Test(std::string model, std::string testImgDir, std::string outputDir)
{
    int truePositive;
    int trueNegative;
    int falsePositive;
    int falseNegative;
    int total;

    std::cout << "TEST!!!!!!!!!!!!!!!!!";

    cv::CascadeClassifier nodule_cascade;
    nodule_cascade.load(model);
    for (int i = 0; i < _xraysTest.size(); i++)
    {

        std::string imgFileName = _xraysTest[i].getFilename();
        std::string clnImgFileName = imgFileName.substr(1, 12).replace(9, 3, "jpg");

        cv::Mat frame = cv::imread(testImgDir + clnImgFileName, CV_LOAD_IMAGE_COLOR);
        std::vector<cv::Rect> nodules;
        //cv::Mat frame_gray;
        //cv::cvtColor( frame, frame_gray, cv::COLOR_BGR2GRAY );
        equalizeHist( frame, frame );
        //-- Detect faces
        nodule_cascade.detectMultiScale( frame, nodules, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(10, 100) );
        if (nodules.size() > 0)
        {
            if (_xraysTest[i].hasNodule())
            {
                truePositive++;
            }
            else
            {
                falsePositive++;
            }
            for (int j = 0; j < nodules.size(); j++)
            {
                cv::rectangle(frame, nodules[j], cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
            }
            cv::imwrite(outputDir + _xraysTest[i].getFilename(), frame);
            total++;
        }
        else if (_xraysTest[i].hasNodule())
        {
            falseNegative++;
            total++;
        }
        else
        {
            trueNegative++;
            total++;
        }
    }
    std::string outputPath = outputDir + "result.txt";
    std::ofstream resultFile (outputPath.c_str(), std::ios_base::out);
    if (resultFile.is_open())
    {
        resultFile << "TP: " + IntToString(truePositive) + "\tFP: " + IntToString(falsePositive) + "\nFN: " + IntToString(falseNegative) + "\tTN: " + IntToString(trueNegative) + "\nTotT: " + IntToString(truePositive + falseNegative) + "\tTotN: " + IntToString(trueNegative + falsePositive);
    }
    resultFile.close();
    std::cout << "Model tested!";
}

void NoduleDetectionPipeline::PrintMetadata()
{
    for (int i = 0; i < _xrays.size(); i++)
    {
        std::cout << _xrays[i] << "\n";
    }
}

int main()
{
  NoduleDetectionPipeline ndp("../data/clinical/xray_metadata.csv");
  ndp.Prepare("/home/brvanove/toil/radiology-cv/data/", "img/", 0.7, 0.3);
  ndp.Train("/home/brvanove/toil/radiology-cv/data/pos.vec", "/home/brvanove/toil/radiology-cv/data/bg.txt", "/usr/local/bin/");
  ndp.Test("/home/brvanove/toil/radiology-cv/data/haarcascade_nodule_cxr.xml", "img/", "/home/brvanove/toil/radiology-cv/data/out/");
}


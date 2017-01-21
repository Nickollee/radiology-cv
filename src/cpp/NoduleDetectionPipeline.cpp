#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "NoduleDetectionPipeline.hpp"
#include "CSVReader.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <unordered_set>

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
}

void NoduleDetectionPipeline::splitTrainTest(double trainSplit, double testSplit)
{
    if (trainSplit <= 0 || trainSplit >= 1 || testSplit <= 0 || testSplit >= 1)
    {
        return;
    }
    srand (time(NULL));
    int numTrain = std::trunc(_xrays.size() * trainSplit);
    int numTest = _xrays.size() - numTrain;

    std::unordered_set<int> selected;
    while (_xraysTrain.size() < numTrain)
    {
        int recordNum;
        recordNum = rand() % _xrays.size();
        std::unordered_set<int>::const_iterator got = selected.find (recordNum);
        if (got != selected.end())
        {
            _xraysTrain.push_back(_xrays[recordNum]);
            selected.insert(recordNum);
        }
    }

    for (int i = 0; i < _xrays.size(); i++)
    {
        std::unordered_set<int>::const_iterator got2 = selected.find (i);
        if (got2 == selected.end())
        {
            _xraysTest.push_back(_xrays[i]);
        }
    }
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
    return round(totalHeight/numNodules);
}

void NoduleDetectionPipeline::Prepare(std::string rootDataDir, std::string relativeSourceImgDir, double trainSplit, double testSplit)
{
    readInMetadata();
    splitTrainTest(trainSplit, testSplit);

    std::ofstream posFile (rootDataDir + "info.dat");
    std::ofstream negFile (rootDataDir + "bg.txt");
    int h = computeMeanNoduleBoxHeight();

    for (int i = 0; i < _xraysTrain.size(); i++)
    // for (int i = 0; i < _xrays.size(); i++)
    {
        Radiograph r = _xraysTrain[i];
        // Radiograph r = _xrays[i];
        if (r.hasNodule())
        {
            if (posFile.is_open())
            {
                posFile << rootDataDir + relativeSourceImgDir + r.getFilename() + "\t1" + "\t" + r.getX() + "\t" + r.getY() + "\t" + h + "\t" + h + "\n";
            }
        }
        else
        {
            if (negFile.is_open())
            {
                negFile << rootDataDir + relativeSourceImgDir + r.getFilename() + "\n";
            }
        }
    }

    posFile.close();
    negFile.close();

    std::system("opencv_createsamples -vec " + rootDataDir + "pos.vec -info " + rootDataDir + "info.dat");
}

void NoduleDetectionPipeline::Train(std::string posVectorFile, std::string negFile, std::string modelDestDir)
{
    int numPos = round(_xraysTrain.size() - 1);
    int numNeg = numPos * 4;
    std::system("opencv_traincascade -data " + modelDestDir + "/haarcascade_nodule_cxr.xml" + " -vec " + posVectorFile + " -bg " + negFile + " -w " + TRAINING_WINDOW_WIDTH + " -h " + TRAINING_WINDOW_HEIGHT + " -numPos " + numPos + " -numNeg " + numNeg + " -precalcValBufSize 1024 -precalcIdxBufSize 1024 -featureType HAAR";
    return;
}

void NoduleDetectionPipeline::Test(std::string model, std::string testImgDir, std::string outputDir)
{
    int truePositive;
    int trueNegative;
    int falsePositive;
    int falseNegative;
    int total;

    cv::CascadeClassifier nodule_cascade;
    nodule_cascade.load(model);
    for (int i = 0; i < _xraysTest.size(); i++)
    {
        cv::Mat frame = imread(testImgDir + _xraysTest[i].getFilename(), CV_LOAD_IMAGE_COLOR);
        std::vector<cv::Rect> nodules;
        cv::Mat frame_gray;
        cv::cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
        equalizeHist( frame_gray, frame_gray );
        //-- Detect faces
        nodule_cascade.detectMultiScale( frame_gray, nodules, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(10, 100) );
        if (nodules.size() > 0)
        {
            if (_xraysTest.hasNodule())
            {
                truePositive++;
            }
            else
            {
                falsePositive++;
            }
            for (int j = 0; j < nodules.size(); j++)
            {
                cv::rectangle(frame, nodules[j], cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0)
            }
            cv::imwrite(outputDir + _xraysTest[i].getFilename(), frame);
            total++
        }
        else if (xraysTest[i].hasNodule())
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
    ofstream resultFile (outputDir + "result.txt");
    if (posFile.is_open())
    {
        resultFile << "TP: " + truePositive + "\tFP: " + falsePositive + "\nFN: " + falseNegative + "\tTN: " + trueNegative + "\nTotT: " + (truePositive + falseNegative) + "\tTotN: " + (trueNegative + falsePositive);
    }
    resultFile.close();
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
  ndp.Train("/home/brvanove/toil/radiology-cv/data/pos.vec", "/home/brvanove/toil/radiology-cv/data/info.dat", "/home/brvanove/toil/radiology-cv/data/");
  ndp.Test("/home/brvanove/toil/radiology-cv/data/haarcascade_nodule_cxr.xml", "/home/brvanove/toil/radiology-cv/data/img/", "/home/brvanove/toil/radiology-cv/data/out/");
}
#include "NoduleDetectionPipeline.hpp"
#include "CSVReader.hpp"

#include <iostream>
#include <cstdlib>

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

void NoduleDetectionPipeline::splitTrainTest()
{
    if (trainSplit <= 0 || trainSplit >= 1 || testSplit <= 0 || testSplit >= 1)
    {
        return;
    }
    srand (time(NULL));
    int numTrain = std::trunc(_xrays.size() * trainSplit);
    int numTest = _xrays._size() - numTrain;

    std::unordered_set<int> selected;
    while (_xraysTrain.size() < numTrain)
    {
        int recordNum;
        recordNum = rand() % x_rays.size();
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

void NoduleDetectionPipeline::extractNodules(std::string extractionSourceDir, std::string extractionDestDir)
{
    return;
}

int NoduleDetectionPipeline::getAverageNoduleBoxHeight()
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

void NoduleDetectionPipeline::PrepareTrainingData(std::string rootDataDir, std::string relativeSourceImgDir)
{
    ofstream posFile (rootDataDir + "info.dat");
    ofstream negFile (rootDataDir + "bg.txt");
    int h = getAverageNoduleBoxHeight();

    for (int i = 0; i < _xraysTrain.size(); i++)
    {
        Radiograph r = _xraysTrain[i];
        if (r.hasNodule())
        {
            if (posFile.is_open())
            {
                posFile << sourceImgDir + relativeSourceImgDir + r.getFilename() + "\t1" + "\t" + r.getX() + "\t" + r.getY() + "\t" + h + "\t" + h + "\n";
            }
        }
        else
        {
            if (negFile.is_open())
            {
                negFile << sourceImgDir + relativeSourceImgDir + r.getFilename() + "\n";
            }
        }
    }

    posFile.close();
    negFile.close();
}

void NoduleDetectionPipeline::TrainNoduleDetector(std::string modelDestDir)
{
    return;
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
  ndp.ReadInMetadata();
  ndp.PrintMetadata();
}
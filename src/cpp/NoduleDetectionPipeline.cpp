#include "NoduleDetectionPipeline.hpp"
#include "CSVReader.hpp"

#include <iostream>

NoduleDetectionPipeline::NoduleDetectionPipeline(std::string metadataFileName)
{
    _metadataFileName = metadataFileName;
}

void NoduleDetectionPipeline::ReadInMetadata()
{
    CSVReader reader(_metadataFileName, true);
    std::vector<std::vector<std::string>> records = reader.Read(',');

    for (int i = 0; i < records.size(); i++)
    {
        std::vector<std::string> record = records[i];
        std::string filename = record[0];
        int subtlety = std::stoi(record[1]);
        int size = std::stoi(record[2]);
        int age = std::stoi(record[3]);
        bool isMale = record[4] == "TRUE";
        int x = std::stoi(record[5]);
        int y = std::stoi(record[6]);
        bool isMalignant = std::stoi(record[7]);
        bool hasNodule = std::stoi(record[8]);
        Radiograph xray(fileName, subtlety, size, age, isMale, x, y, isMalignant, hasNodule);
        std::cout << xray << "\n";
        _xrays.push_back(xray);        
    }
}

void NoduleDetectionPipeline::PrintMetadata()
{
    for (std::vector<Radiograph>::const_iterator iterator = _xrays.begin(), end = _xrays.end(); iterator != end; ++iterator) {
        std::cout << *iterator << "\n";
    }
}

void NoduleDetectionPipeline::ExtractNodules()
{
    return;
}

int main()
{
  NoduleDetectionPipeline ndp("../data/clinical/xray_metadata.csv");
  ndp.ReadInMetadata();
  ndp.PrintMetadata();
}
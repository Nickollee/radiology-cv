#include "NoduleDetectionPipeline.hpp"
#include "CSVReader.hpp"

#include <iostream>
#include <cstdlib>

NoduleDetectionPipeline::NoduleDetectionPipeline(std::string metadataFileName)
{
    _metadataFileName = metadataFileName;
}

void NoduleDetectionPipeline::ReadInMetadata()
{
    CSVReader reader(_metadataFileName, true);
    std::vector < std::vector < std::string > > records = reader.Read(',');

    for (int i = 0; i < records.size(); i++)
    {
        std::vector<std::string> record = records[i];

        std::cout << "######################\n";
        std::cout << "0: " << record[0] << "\n";
        std::cout << "1: " << record[1] << "\n";
        std::cout << "2: " << record[2] << "\n";
        std::cout << "3: " << record[3] << "\n";
        std::cout << "4: " << record[4] << "\n";
        std::cout << "5: " << record[5] << "\n";
        std::cout << "6: " << record[6] << "\n";
        std::cout << "7: " << record[7] << "\n";
        std::cout << "8: " << record[8] << ":\n";
        std::cout << "######################\n"; 
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
        bool hasNodule = record[8] == "TRUE";

        Radiograph xray(filename, subtlety, size, age, isMale, x, y, isMalignant, hasNodule);
        _xrays.push_back(xray);        
    }
}

void NoduleDetectionPipeline::PrintMetadata()
{
    for (int i = 0; i < _xrays.size(); i++)
    {
        std::cout << _xrays[i] << "\n";
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
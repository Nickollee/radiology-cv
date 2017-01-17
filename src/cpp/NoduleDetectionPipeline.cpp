#include "NoduleDetectionPipeline.hpp"

NoduleDetectionPipeline::NoduleDetectionPipeline(std::string metadataFileDir, std::string metadataFileName, std::string extractionDir)
{
    _metadataFileDir = metadataFileDir;
    _metadataFileName = metadataFileName;
    _extractionDir = extractionDir;
}

void NoduleDetectionPipeline::ReadInMetadata()
{
    ifstream file ( _metadataFileDir + _metadataFileName ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
    std::string value;
    std::list<std::string> values;
    while ( file.good() )
    {
        std::getline ( file, value, ',' ); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
        if (value.find('\n') != std::string::npos) {
            split_line(value, "\n", values);
        } else {
            values.push_back(value);
        }
    }

    std::list<std::string>::const_iterator it = values.begin();
    int currentCol = 0;
    std::string fileName;
    int subtlety;
    int size;
    int age;
    bool isMale;
    int x;
    int y;
    bool isMalignant;
    bool hasNodule;

    for (it = values.begin(); it != values.end(); it++) {
        string tmp = *it;
        switch (currentCol)
        {
            case 0:
                fileName = tmp;
                break;
            case 1:
                subtlety = (int)(std::strtol(tmp.c_str(), NULL));
                break;
            case 2:
                size = (int)(std::strtol(tmp.c_str(), NULL));
                break;
            case 3:
                age = (int)(std::strtol(tmp.c_str(), NULL));
                break;
            case 4:
                isMale = tmp.compare("TRUE");
                break;
            case 5:
                x = (int)(std::strtol(tmp.c_str(), NULL));
                break;
            case 6:
                y = (int)(std::strtol(tmp.c_str(), NULL));
                break;
            case 7:
                isMalignant = tmp.compare("TRUE");
                break;
            case 8:
                hasNodule = tmp.compare("TRUE");
                break;
            default:
                break;
        }
        Radiograph xray(fileName, subtlety, size, age, isMale, x, y, isMalignant, hasNodule);
        _xrays.Insert(xray);
    }
}

void NoduleDetectionPipeline::PrintMetadata()
{
    for (std::list<Radiograph>::const_iterator iterator = _xrays.begin(), end = _xrays.end(); iterator != end; ++iterator) {
        std::cout << *iterator << "\n";
    }
}

void NoduleDetectionPipeline::ExtractNodules()
{
    return;
}
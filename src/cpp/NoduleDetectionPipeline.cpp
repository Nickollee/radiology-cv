#include "NoduleDetectionPipeline.hpp"

NoduleDetectionPipeline::NoduleDetectionPipeline(std::string metadataFileName)
{
    _metadataFileName = metadataFileName;
}

void NoduleDetectionPipeline::ReadInMetadata()
{
    std::ifstream ifs;
    ifs.open (_metadataFileName.c_str(), std::ifstream::in);
    std::string value;
    std::list<std::string> values;
    std::getline ( ifs, value, '\n' );

    vector< vector<string> > result;
    while (!file.eof())
    {
        //go through every line
        string line;
        vector<string> tmp;
        size_t pos=string::npos;
        getline(file,line);
        //loop through the ,
        while ((pos=line.find_first_of(","))!=string::npos)
        {
        //extract the component sans ,
        tmp.push_back(line.substr(0,pos-1));
        //erase the val including ,
        line.erase(0,pos);
        }
        result.push_back(tmp);
    }

    // while ( ifs.good() )
    // {
    //     std::getline ( ifs, value, '\n' ); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
    //     if (value.find(',') != std::string::npos) {
    //         split_line(value, ",", values);
    //     } else {
    //         values.push_back(value);
    //     }
    // }

    // std::list<std::string>::const_iterator it = values.begin();

    for (int i = 0; i < result.size(); i++)
    {
        std::string fileName;
        int subtlety;
        int size;
        int age;
        bool isMale;
        int x;
        int y;
        bool isMalignant;
        bool hasNodule;

        switch (i)
        {
            case 0:
                fileName = tmp;
                break;
            case 1:
                subtlety = (int)(std::strtol(tmp.c_str(), NULL, 10));
                break;
            case 2:
                size = (int)(std::strtol(tmp.c_str(), NULL, 10));
                break;
            case 3:
                age = (int)(std::strtol(tmp.c_str(), NULL, 10));
                break;
            case 4:
                isMale = tmp.compare("TRUE");
                break;
            case 5:
                x = (int)(std::strtol(tmp.c_str(), NULL, 10));
                break;
            case 6:
                y = (int)(std::strtol(tmp.c_str(), NULL, 10));
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
        std::cout << xray << "\n";
        _xrays.push_back(xray);
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

int main()
{
  NoduleDetectionPipeline ndp("../data/clinical/xray_metadata.csv");
  ndp.ReadInMetadata();
  ndp.PrintMetadata();
}
#include "Radiograph.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>

class NoduleDetectionPipeline
{
    private:
        std::string _metadataFileName;
        std::list<Radiograph> _xrays;
        void split_line(std::string& line, std::string delim, std::list<std::string>& values)
        {
            size_t pos = 0;
            while ((pos = line.find(delim, (pos + 1))) != std::string::npos) {
                std::string p = line.substr(0, pos);
                values.push_back(p);
                line = line.substr(pos + 1);
            }

            if (!line.empty()) {
                values.push_back(line);
            }
        }
    public:
        NoduleDetectionPipeline(std::string metadataFileName);
        std::string getMetadataFileName(){return _metadataFileName;}
        void ReadInMetadata();
        void ExtractNodules();
        void PrintMetadata();
};
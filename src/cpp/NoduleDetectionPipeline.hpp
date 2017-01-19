#include "Radiograph.hpp"
#include "CSVReader.hpp"

#include <string>
#include <vector>


class NoduleDetectionPipeline
{
    private:
        std::string _metadataFileName;
        std::vector<Radiograph> _xrays;
    public:
        NoduleDetectionPipeline(std::string metadataFileName);
        std::string getMetadataFileName(){return _metadataFileName;}
        void ReadInMetadata();
        void ExtractNodules();
        void PrintMetadata();
};
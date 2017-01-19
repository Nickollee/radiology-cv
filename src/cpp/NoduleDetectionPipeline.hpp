#include "Radiograph.hpp"

#include <string>
#include <vector>


class NoduleDetectionPipeline
{
    private:
        std::string _metadataFileName;
        std::vector<Radiograph> _xrays;
        std::vector<Radiograph> _xraysTrain;
        std::vector<Radiograph> _xraysTest;
        std::vector<Radiograph> _xraysXValid;
        void readInMetadata();
        void splitTrainTest(double trainSplit, double testSplit);
        void extractNodules(std::string extractionSourceDir, std::string extractionDestDir);
        int getAverageNoduleBoxHeight();

    public:
        NoduleDetectionPipeline(std::string metadataFileName);
        std::string getMetadataFileName(){return _metadataFileName;}
        void PrepareTrainingData(std::string rootDataDir, std::string relativeSourceImgDir);
        void PrintMetadata();
};
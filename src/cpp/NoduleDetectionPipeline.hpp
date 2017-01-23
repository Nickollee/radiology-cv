#include "Radiograph.hpp"

#include <string>
#include <vector>
#include <sstream>

int const TRAINING_WINDOW_WIDTH = 15;
int const TRAINING_WINDOW_HEIGHT = 15;

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
        int computeMeanNoduleBoxHeight();
        std::string IntToString(int a)
        {
            std::ostringstream temp;
            temp<<a;
            return temp.str();
        }
        int _meanNoduleBoxHeight;

    public:
        NoduleDetectionPipeline(std::string metadataFileName);
        std::string getMetadataFileName(){return _metadataFileName;}
        void Prepare(std::string rootDataDir, std::string relativeSourceImgDir, double trainSplit, double testSplit);
        void Train(std::string posVectorFile, std::string negFile, std::string modelDestDir);
        void Test(std::string model, std::string testImgDir, std::string outputDir);
        void PrintMetadata();
        int getMeanNoduleBoxHeight(){return _meanNoduleBoxHeight;}
};
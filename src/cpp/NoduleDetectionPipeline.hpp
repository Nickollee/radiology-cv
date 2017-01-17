#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>

class NoduleDetectionPipeline
{
    private:
        std::string _metadataFileDir
        std::string _metadataFileName
        std::string _extractionDir
        std::string _trainingDir
        std::string _testDir
        std::string _recordPrefix
        std::list<Radiograph> _xrays
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
        NoduleDetectionPipeline(std::string metadataFileDir, std::string metadataFileName, std::string extractionDir);
        std::string getMetadataFileDirectory(){return _metadataFileDir;}
        std::string getMetadataFileName(){return _metadataFileName;}
        std::string getExtractionDirectory(){return _extractionDir;}
        std::string getTrainingDirectory(){return _trainingDir;}
        std::string getTestDir(){return _testDir;}
        std::string getRecordPrefix(){return _recordPrefix;}
        void setTrainingDirectory(std::string trainingDir){_trainingDir = trainingDir;}
        void setTestDirectory(std::string testDir){_testDir = testDir;}
        void setRecordPrefix(std::string recordPrefix) {_recordPrefix = recordPrefix;}
        void ReadInMetadata();
        void ExtractNodules();
        void PrintMetadata();
}
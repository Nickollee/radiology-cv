#include <string>
#include <vector>

class CSVReader
{
    private:
        std::string _filepath;
        bool _containsHeader
    public:
        CSVReader(std::string filepath, bool containsHeader);
        CSVReader(std::string filepath);
        std::vector<std::vector<std::string>> Read(recordDelimeter);
};
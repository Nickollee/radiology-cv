#include "CSVReader.hpp"

#include <sstream>
#include <fstream>

CSVReader::CSVReader(std::string filepath, bool containsHeader)
{
    _filepath = filepath;
    _containsHeader = containsHeader;
}

CSVReader::CSVReader(std::string filepath)
{
    _filepath = filepath;
    _containsHeader = false;
}

std::vector < std::vector < std::string > > CSVReader::Read(char recordDelimeter)
{
    std::vector <std::vector <std::string> > data;
    std::ifstream infile(_filepath.c_str(), std::ios_base::in);

    while (infile)
    {
        std::string s;
        if (!std::getline( infile, s )) break;

        std::istringstream ss( s );
        std::vector <std::string> record;

        while (ss)
        {
            std::string s;
            if (!std::getline( ss, s, recordDelimeter )) break;
            record.push_back( s );
        }

        data.push_back( record );
    }
    if (!infile.eof())
    {
        return data;
    }
    return data;
}

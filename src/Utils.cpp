#include "Utils.h"

#include <string>
#include <fstream>
#include <exception>
#include <iostream>

bool Utils::isValidString(const std::string& str)
{
    for(size_t i = 0; i < str.size(); i++)
    {
        // Ensures given character contains only [a-z][A-Z].
        if(!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')))
        {
            return false;
        }
    }

    return true;
}

void Utils::initFiles(int argc, char* argv[], std::ifstream& wheelsFile, std::ifstream& dictionaryFile)
{
    // Not enough input arguments. A file name for the dictionary and the wheel combination must be provided.
    if(argc < 3)
    {
        std::string out("At least 2 CLI arguments must be provided to run the program.\n"
            "Argument one must be the file name/path for the lettered wheels (.txt).\n"
            "Argument two must be the file name/path for the dictionary (.txt).\n"
            "Aborting the program...");

        throw(std::invalid_argument(out));
    }

    wheelsFile.open(argv[1]);
    dictionaryFile.open(argv[2]);

    // Unable to create a stream for the lettered wheels - the file could not be found or accessed.
    if(wheelsFile.fail() || dictionaryFile.fail())
    {
        std::string out;

        // Both files have failed to be accessed.
        if(wheelsFile.fail() && dictionaryFile.fail())
        {
            out.append("Unable to locate / access the files with paths : ");
            out.append(argv[1]);
            out.append(" & ");
            out.append(argv[2]);
        }
        // One file has failed to be accessed.
        else
        {
            out.append("Unable to locate / access the file with path : ");
            out.append(wheelsFile.fail() ? argv[1] : argv[2]);
        }

        throw(std::invalid_argument(out));
    }
}

void Utils::closeFiles(std::ifstream& wheelsFile, std::ifstream& dictionaryFile)
{
    if(dictionaryFile.is_open())
    {
        dictionaryFile.close();
    }

    if(wheelsFile.is_open())
    {
        wheelsFile.close();
    }
}

void Utils::printResult(const std::unordered_set<std::string>& result)
{
    for(const auto& foundWord : result)
    {
        std::cout << foundWord << std::endl;
    }

    std::cout << "Found " << result.size() << " unique words" << std::endl;
}
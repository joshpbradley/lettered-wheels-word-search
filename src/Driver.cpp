#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <memory>
#include <unordered_set>

#include "Wheels.h"
#include "Dictionary.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
    std::ifstream wheelsFile;
    std::ifstream dictionaryFile;

    try
    {
        // Attempts to open files from CLI arguments.
        Utils::initFiles(argc, argv, wheelsFile, dictionaryFile);
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << e.what();
        return -1;
    }

    std::unique_ptr<Dictionary> dictionary;
    std::unique_ptr<Wheels> letteredWheels;

    try
    {
        // Initialises the lettered wheels.
        letteredWheels = std::make_unique<Wheels>(wheelsFile);
        // Initialises the dictionary, encoded as a trie structure.
        dictionary = std::make_unique<Dictionary>(dictionaryFile, letteredWheels->getNumberOfWheels());
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    Utils::closeFiles(wheelsFile, dictionaryFile);

    // The collection of unique words generated from the lettered wheels, and found in the dictionary.
    std::unordered_set<std::string> result;
    // The current word to attempt to find in the dictionary.
    std::string permutation = letteredWheels->getCurrentPermutation();

    while(letteredWheels->canPermute())
    {
        // Attempts a search for the current word, permutation, in the dictionary.
        Dictionary::SEARCH_STATUS state = dictionary->find(permutation);
        
        // The word has been found, so add it to the result set.
        if(state == Dictionary::HIT_PARENT || state == Dictionary::HIT_TERMINAL)
        {
            result.insert(permutation);
        }

        // Generate a new word to attempt to find.
        permutation = letteredWheels->advancePermutation(state);
    }

    Utils::printResult(result);
}
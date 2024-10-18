#pragma once

#include <string>
#include <fstream>
#include <unordered_set>

/// A series of helper functions for improved code readability.
class Utils
{
  public:
     /// The number of letters in the Latin alphabet
    static const unsigned char LETTERS_IN_ALPHABET = 26;

    /**
     * Determines whether all characters in the given string are in the set [A-Z][a-z].
     * 
     * @param: str the string to query
     * @return true if the string is valid ([a-z][A-Z]), else false
     */
    static bool isValidString(const std::string& str);
    /**
     * Attempts to open two files: one representing the lettered wheels and one for the dictionary.
     * The second and third CLI arguments should both refer to .txt files, the wheels file and the dictionary file, respectively.
     * 
     * An exception will be thrown if either file cannot be located/accessed and the program will terminate.
     * 
     * @param argc: the number of provided CLI arguments
     * @param argv: the CLI arguments
     * @param wheelsFile: the file object pointing to the .txt file representing the lettered wheels
     * @param dictionaryFile: the file object pointing to the .txt file representing the dictionary
     */
    static void initFiles(int argc, char* argv[], std::ifstream& wheelsFile, std::ifstream& dictionaryFile);
    /**
     * Closes two files: one representing the lettered wheels and one for the dictionary.
     *
     * @param wheelsFile: the file object pointing to the .txt file representing the lettered wheels
     * @param dictionaryFile: the file object pointing to the .txt file representing the dictionary
     */
    static void closeFiles(std::ifstream& wheelsFile, std::ifstream& dictionaryFile);
    /**
     * Prints each string entry in the provided unordered_set and the total number of entries.
     * Each entry represents a unique word found in the lettered wheels that has been found in the provided dictionary.
     * 
     * @param result: the collection of unique words formed by the lettered wheels and present in the dictionary
     */
    static void printResult(const std::unordered_set<std::string>& result);
};
#include "Wheels.h"

#include <fstream>
#include <vector>
#include <exception>
#include <string>
#include <iostream>

#include "Utils.h"
#include "Dictionary.h"

Wheels::Wheels() {}

Wheels::Wheels(std::ifstream& file)
{
    std::string line;

    try
    {
        // Reads the number of wheels from the wheels file.
        std::getline(file, line);
        numberOfWheels = std::stoll(line);
        // Reserves space for the number of wheels in a vector, using an integer read from the wheels file.
        wheels.reserve(numberOfWheels);

        // Reads the number of letters per wheel from the wheel file.
        std::getline(file, line);
        lettersPerWheel = std::stoll(line);
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << "Unable to convert string value to an integer. Value: " << line << std::endl;
        throw e;
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << "This value exceeds the limits of the long long data type. Value: " << line << std::endl;
        throw e;
    }

    // Ensures that the there are enough wheels (1) required to form a word with enough letters in each wheel (2).
    if(numberOfWheels < 1 || lettersPerWheel < 2)
    {
        throw std::invalid_argument("Not enough rows or columns inside the lettered wheels."
            " A wheel must contain at least two letters and there must be at least one wheel.");
    }

    while(std::getline(file, line))
    {
        // Letters representing an individual wheel are not in the expected values [a-z][A-Z].
        if(!Utils::isValidString(line))
        {
            std::string arg;
            arg.append("Encountered a character \"").append({ line }).append("\" not in the expected range of {[a-z][A-Z]}.").append(
                "\nAborting insertion of the wheel letters.\n");

            throw std::invalid_argument(arg);
        }
        
        // Reserves space for the number of letters in each wheel in a vector, using an integer read from the wheels file.
        std::vector<char> wheel;
        wheel.reserve(lettersPerWheel);

        // Create a vector representing an individual wheel and push it to the enclosing wheels vector.
        for(size_t i = 0; i < line.length(); i++)
        {
            wheel.push_back(std::toupper(line[i]));
        }

        wheels.push_back(wheel);
    }

    // Push the first first two characters to the WheelPermutation. The starting point of the DFS as it has the length of a legal word.
    wheelPermutation.push({ wheels[wheelIndex][letterIndex], letterIndex });
    wheelPermutation.push({ wheels[endWheelIndex][endLetterIndex], endLetterIndex });
}

size_t Wheels::getLettersPerWheel(void) const
{
    return lettersPerWheel;
}

size_t Wheels::getNumberOfWheels(void) const
{
    return numberOfWheels;
}

std::string Wheels::advancePermutation(Dictionary::SEARCH_STATUS state)
{
    switch(state)
    {
        // In these states the search cannot continue in depth, so progress to the next letter on the last wheel.
        case Dictionary::HIT_TERMINAL:
        case Dictionary::MISS:
            wheelPermutation.pop();
            endLetterIndex++;
            break;
        // In these states more words in the dictionary continue from the current string permutation, so continue in depth
        // by adding on another wheel.
        case Dictionary::HIT_PARENT:
        case Dictionary::MISS_PARENT:
            endWheelIndex++;
            endLetterIndex = 0;
            break;
    }

    // Handles attempting to access a letter beyond the size of the wheel.
    if(state == Dictionary::HIT_TERMINAL || state == Dictionary::MISS)
    {
        // Continues to pop back on the wheel letters until a letter that isn't at the end of the wheel is accessed.
        while(wheelPermutation.size() > 1 && endLetterIndex >= Wheels::getLettersPerWheel())
        {
            revertAndProgressPermutation();
        }
    }

    // Handles attempting to access a wheel beyond the last wheel.
    if(state == Dictionary::HIT_PARENT || state == Dictionary::MISS_PARENT)
    {
        // Progresses to the next element along the last wheel.
        if(endWheelIndex >= getNumberOfWheels())
        {
            revertAndProgressPermutation();
        }

        // Continues to pop back on the wheel letters until a letter that isn't at the end of the wheel is accessed.
        while(wheelPermutation.size() > 1 && endLetterIndex >= Wheels::getLettersPerWheel())
        {
            revertAndProgressPermutation();
        }
    }

    // Handles cases where all paths for a particular starting node has been exhausted,
    // so the starting node must be progressed.
    if(wheelPermutation.size() <= 1 && endLetterIndex >= getLettersPerWheel())
    {
        // Increase the starting letter index on its current wheel.
        if(letterIndex < getLettersPerWheel() - 1)
        {
            letterIndex++;
        }
        // Increase the starting wheel indexand resets it to the first letter position.
        else
        {
            wheelIndex++;
            letterIndex = 0;
        }

        // Sets the end node to be in the next wheel, also in the first letter position.
        endWheelIndex = wheelIndex + 1;
        endLetterIndex = 0;

        // Empties the wheelPermutation stack, resetting it for the new starting letter.
        while(!wheelPermutation.empty())
        {
            wheelPermutation.pop();
        }

        // Sets the initial permutation. The conditional ensures that there isn't an attempt to
        // access beyond the last wheel, causing an out of bounds error.
        if(canPermute())
        {
            wheelPermutation.push(std::pair<char, size_t>({ wheels[wheelIndex][letterIndex] }, letterIndex));
            wheelPermutation.push(std::pair<char, size_t>({ wheels[endWheelIndex][endLetterIndex] }, endLetterIndex));
        }
    }
    // Paths are not exhausted for the starting node, so continue.
    else
    {
        wheelPermutation.push(std::pair<char, size_t>({ wheels[endWheelIndex][endLetterIndex] }, endLetterIndex));
    }

    return Wheels::getCurrentPermutation();
}

bool Wheels::canPermute(void) const
{
    // Can continue permutation as long as there is at least one wheel that can form the second character in a permutation..
    return wheelIndex < numberOfWheels - 1;
}

std::string Wheels::getCurrentPermutation()
{
    std::string resultString;
    // Copies the container storing the permutation to prevent compromising the original.
    auto permutationCopy(wheelPermutation);

    // Pop all elements of the stack, prepending each found character to the result string.
    while(!permutationCopy.empty())
    {
        resultString.insert(0, { permutationCopy.top().first });
        permutationCopy.pop();
    }

    return resultString;
}

void Wheels::revertAndProgressPermutation()
{
    endWheelIndex--;
    endLetterIndex = wheelPermutation.top().second + 1;
    wheelPermutation.pop();
}
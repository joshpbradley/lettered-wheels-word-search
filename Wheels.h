#pragma once

#include <fstream>
#include <vector>
#include <stack>
#include <utility>

#include "Dictionary.h"

/*
 * Represents a series of uniform rotating wheels containing characters [A-Z].
 * By taking a character from a subset of contiguous wheels, words can be formed.
 * 
 * To generate the wheels, the program expects a .txt file first containing two integers
 * each on separate lines. These represent the number of wheels and the number of letters
 * per wheel, respectively.
 * 
 * The following lines should be a grid of characters [A-Z] (case-insensitive) that follow
 * the previously described proportions. Each horizontal lines forms a single wheel.
 * 
 * Example input:
 * 3
 * 5
 * ABCDE
 * FGHIJ
 * KLMNO
 * 
 * Words most flow from an upper wheel to a lower wheel and words do not wrap.
 * 
 * Words from this input (non-exhaustive): DIM, AH, IN.
 */
class Wheels
{
  public:
    /**
     * Constructor.
     * 
     * @param file: a file object pointing to a .txt file, containing
     * information to construct the Wheels object.
     */
    Wheels(std::ifstream& file);
    /**
     * Gets the number of lettered wheels.
     * 
     * @return the number of lettered wheels
     */
    size_t getNumberOfWheels(void) const;
    /**
     * Gets the number of letters that appear on each wheel.
     *
     * @return the number of letters that appear on each wheel
     */
    size_t getLettersPerWheel(void) const;
    /**
     * Generates a new permutation in the wheels via DFS traversal.
     * The new permutation is calculated based on the state of a dictionary lookup
     * for a given permutation.
     * 
     * If the search status is MISS or HIT_TERMINAL then the most recent
     * character is swapped (i.e. the last wheel in the permutation rotates.)
     * 
     * Otherwise, an extra character is sourced from the next wheel and appended
     * to the permutation.
     * 
     * @param state: the search status that is used to compute
     * @return a new permutation of the letered wheels represented as a string
     */
    std::string advancePermutation(Dictionary::SEARCH_STATUS state);
    /**
     * Returns true if a new permutation can be safely computed, else false.
     * 
     * This check should occur before any call to the advancePermutation() method.
     * 
     * @return true if a new permutation can be safely computed, else false
     */
    bool canPermute(void) const;
    /**
     * Gets the next permutation of the lettered wheels.
     * 
     * @return the next permutation of the lettered wheels
     */
    std::string getCurrentPermutation(void);

  private:
    size_t numberOfWheels = 0;
    size_t lettersPerWheel = 0;
    std::vector<std::vector<char>> wheels;
    std::stack<std::pair<char, size_t>> wheelPermutation;

    // The index of the wheel starting the permutation.
    size_t wheelIndex = 0;
    // The index of the wheel ending the permutation.
    size_t endWheelIndex = 1;
    // The index of the letter position starting the permutation.
    size_t letterIndex = 0;
    // The index of the letter position ending the permutation.
    size_t endLetterIndex = 0;

    Wheels(void);
    // Used to handle permutations when attempting to access letter positions and wheels out of bounds.
    void revertAndProgressPermutation(void);
};

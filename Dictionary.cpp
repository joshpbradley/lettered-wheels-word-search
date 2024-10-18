#include "Dictionary.h"

#include <array>
#include <memory>
#include <string>
#include <fstream>
#include <assert.h>
#include <cctype>

#include "Utils.h"

std::array<std::unique_ptr<Dictionary::Node>, Utils::LETTERS_IN_ALPHABET>& Dictionary::Node::GetArray() {
	return letters;
}

bool Dictionary::Node::getIsWord(void) const
{
    return isWord;
}

void Dictionary::Node::setIsWord(bool b)
{
    isWord = b;
}

bool Dictionary::Node::getIsTerminal(void) const
{
    return isTerminal;
}

void Dictionary::Node::setIsNotTerminal(void)
{
    isTerminal = false;
}

Dictionary::Dictionary(void) : root(std::make_unique<Node>()) {}

Dictionary::Dictionary(std::ifstream& file, const size_t maximumWordSize) : root(std::make_unique<Dictionary::Node>())
{
    std::string word;

    while(std::getline(file, word))
    {
        // Ensures words inserted in the dictionary are in range 2 <= word length <= maximumWordSize.
        if(word.length() > 1 && word.length() <= maximumWordSize)
        {
            Dictionary::insert(word);
        }
    }
}

void Dictionary::insert(const std::string& word) const
{
    // Ensures given character contains only {[a-z][A-Z]}.
    if (!Utils::isValidString(word))
    {
        std::string arg;
        arg.append("Encountered a character in the word \"").append({ word }).
            append("\" not in the expected range of {[a-z][A-Z]}.\n").append(
                "Aborting insertion of the dictionary.\n");

        throw std::invalid_argument(arg);
    }

    Node* node = root.get();

    // Iterates through each character in the input word.
    for(size_t i = 0; i < word.length(); i++)
    {
        auto& arr = node->GetArray();

        // The lowercase character index in the alphabet.
        unsigned char c = (std::toupper(word[i]) - 'A');

        assert(c >= 0 && c <= 25);
        // The current location in the array is not occupied, so fill it.
        if(arr[c] == nullptr)
        {
            // Create a new node and insert in the array.
            arr[c] = std::make_unique<Node>();

            // Set previous node to not be a terminal node.
            if(node->getIsTerminal())
            {
                node->setIsNotTerminal();
            }
        }

        // Gets the raw pointer to the node just inserted in the array and makes it the current node.
        node = arr[c].get();
    }

    node->setIsWord(true);
}

Dictionary::SEARCH_STATUS Dictionary::find(const std::string& word)
{
    // Immediately returns a MISS if the word to find is not in the expected format.
    if(!Utils::isValidString(word))
    {
        return MISS;
    }

    Dictionary::Node* node = root.get();

    // Iterate through each letter in the word.
    for(size_t i = 0; i < word.length(); i++)
    {
        // The character's index in the alphabet.
        // A = 0, B = 1, ..., Z = 25.
        unsigned char c = (std::toupper(word[i]) - 'A');
        auto& arr = node->GetArray();

        assert(c >= 0 && c <= 25);
        // The letter that has attempted to be found is not present in the structure.
        if(arr[c] == nullptr)
        {
            return MISS;
        }

        // Gets the raw pointer to the node just accessed in the array and makes it the current node.
        node = arr[c].get();
    }

    // Node is terminal node - it does not have any children and it is a word.
    if(node->isTerminal)
    {
        return HIT_TERMINAL;
    }
    
    // This node is a parent because the final letter in the word is present in the structure, but not a terminal node.
    // If the final node has been labelled as a word, return HIT_PARENT, else MISS_PARENT.
    return node->getIsWord() ? HIT_PARENT : MISS_PARENT;
}
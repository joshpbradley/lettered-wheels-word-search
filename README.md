# lettered-wheels-word-search
 Finds all dictionary-supplied words contained in a series of lettered wheels.

This program finds all possible words of two letters or more created by a series of lettered wheels.

The lettered wheels are represented in a plain text file wheels.txt. The first two lines define the number of wheels and the number of letters per wheel, respectively. Each following line describes the letters in one wheel. E.g. 

5 
4 
ETMJ 
NTAO 
RNTC 
TOCS 
AEHS

Words are formed from a continuous forward run of letters, with one letter selected per wheel. Words can start at any wheel, but do not wrap around the final wheel. The maximum word length is therefore limited by the number of wheels.

The dictionary file is a plain text file dictionary.txt which contains a list of valid words with one word per line.

The output of this program is printed in the console window. It prints the set of all found words, each on a new line, followed by the number of words found.

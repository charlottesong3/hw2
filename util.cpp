#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords){


    set<string> result;
    string currentWord;

    //:3

    rawWords = convToLower(rawWords); 
    //boom converted now

    for(size_t i = 0; i < rawWords.size(); i++) {

        unsigned char character = static_cast<unsigned char>(rawWords[i]);

        if(isspace(character) || ispunct(character)) { 

          if(currentWord.size() >= 2) {

            result.insert(currentWord);
            
          }
          
          currentWord.clear();
        }
        else {

          currentWord += rawWords[i];

        }
    }

    // Handles the last word if the string does not end with a separator
    if(currentWord.size() >= 2) {
        result.insert(currentWord);
    }

    return result;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

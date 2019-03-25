#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <functional>
#include <cctype>
#include <iostream>
#include <fstream>

class WordListImpl{
public:
	bool loadWordList(std::string filename);
	bool contains(std::string word) const;
	std::vector<std::string> findCandidates(std::string cipherWord, std::string currTranslation) const;
private:
	MyHash<std::string,std::vector<std::string>> m_words;
	std::string convertToPattern(std::string word) const;
};

//CONVERTTOPATTERN
//////////////////////////////////////////////////////
std::string WordListImpl::convertToPattern(std::string word) const {
	//Create a temporary string
	std::string temp;
	//Create a character array that can take 27 characters and set each character to a dash (-)
	char els[27] = "--------------------------";
	//Initialize a character i = '1'. It will be used as a counter.
	char i = '1';
	//Go through all the characters in the word passed into the function
	for (int j = 0; j < word.size(); j++){
		//if the character is not an alphabet, continue the loop without executing any other lines
		if (!isalpha(word[j])) { continue; }
		//Else if the character is a -
		else if (els[tolower(word[j]) - 'a'] == '-') {
			//set the character at that position to i
			els[tolower(word[j]) - 'a'] = i;
			//Increment i
			i++;
		}
		//Concatenate this element in the string to the temporary string
		temp += els[tolower(word[j]) - 'a'];
	}
	//Return the temporary string
	return temp;
}

//LOADWORDLIST
//////////////////////////////////////////////////////
bool WordListImpl::loadWordList(std::string filename){
	//Reset hashTable every time this function is called
	m_words.reset();
	// create an ifstream to read input from file
	//std:: ifstream infile("wordlist.txt");
	std::ifstream infile(filename);
	//Check if failed to connect file to ifstream
	if (!infile) {return false;}
	//Create a temp string s
	std::string s;
	//Read lines from the text file until it is in an invalid state
	while (std::getline(infile, s)) {
		//For each string s, go through all of its characters
		for (int i = 0; i < s.size(); i++) {
			//If not an alphabet or a slash, leave the for loop
			if (!isalpha(s[i]) || s[i] == '\'') { break; } 
			//Set the character to a lowercase version of itself 
			s[i] = tolower(s[i]);
		}
		//Convert the word from the text file into a number pattern
		std::string pattern = convertToPattern(s);
		//Create a temp vector
		std::vector<std::string> temp;
		//If the pattern doesn't exist in the hash table
		if (m_words.find(pattern) == nullptr) {
			//push the word back onto the vector
			temp.push_back(s);
			//add it to the hash table
			m_words.associate(pattern, temp);
		}
		//Else push it back onto the vector at that key
		else {m_words.find(pattern)->push_back(s);}
	}
	//Return true
	return true;
}

//CONTAINS
////////////////////////////////////////////////////
bool WordListImpl::contains(std::string word) const {
	//Search for the pattern. If there does exist a key with this pattern
	if (m_words.find(convertToPattern(word)) != nullptr) {
		//create a temp vector and set it to the vector(value) at that key
		std::vector<std::string> temp = *(m_words.find(convertToPattern(word)));
		//traverse through this temp vector
		for (int i = 0; i < temp.size(); i++) {
			//If a word from this vector matches to one of the words we want
			if (temp[i] == word) {
				//Return true
				return true;
			}
		}
	}
	//return false if the word is not in the word list
	return false;
}

//FINDCANDIDATES
/////////////////////////////////////////////////////
std::vector<std::string> WordListImpl::findCandidates(std::string cipherWord, std::string currTranslation) const{
	//Check if cipherWord and currTranslation are the same size
	if (cipherWord.size() != currTranslation.size()) {
		//Return an empty vectorif this is true
		std::vector<std::string> candidates;
		return candidates;
	}
	//Process cipherword and currTranslation according to given spec
	for (int i = 0; i < cipherWord.size(); i++) {
		//cipherword should only contain letters and apostrophes
		if (!isalpha(cipherWord[i]) && cipherWord[i] != '\'') {
			//Return empty vector if false
			std::vector<std::string> candidates;
			return candidates;
		}
		//currTranslation should only contain letters, apostrophes and question marks
		if (!isalpha(currTranslation[i]) && currTranslation[i] != '\'' && currTranslation[i] != '?') {
			//Returns empty vector if false
			std::vector<std::string> candidates;
			return candidates;
		}
	}
	//check if the bucket is a nullptr
	if (m_words.find(convertToPattern(cipherWord)) == nullptr) {
		//If it is, return an empty vector
		std::vector<std::string> temp;
		return temp;
	}
	//otherwise get the vector at that bucket and save it in a vector (say, vector_words)
	std::vector<std::string> vector_words = *(m_words.find(convertToPattern(cipherWord)));
	//Create an empty temp vector
	std::vector<std::string> temp;
	//Traverse through vector_words
	for (int j = 0; j < vector_words.size(); j++){
		//Create a new string and initialize it to the word at the jth index in this vector
		std::string word = vector_words[j];
		//Go through each character of this word
		for (int i = 0; i < word.size(); i++) {
			//if i == word.size()-1, push it on the vector since it is a potential candidate
			if (i == word.size() - 1) {
				//push the word onto the vector
				temp.push_back(word);
				//Break out of this for loop so you can move onto the next word
				break;
			}
			//If the character is an apostrophe, make sure it is in the same place
			if (currTranslation[i] == '\'' && word[i] == '\'') {
				//In this case, just continue to the next iteration
				continue;
			}
			//if the character is a question mark
			else if (currTranslation[i] == '?') {
				//In this case, just continue to the next iteration
				continue;
			}
			//if it is a letter, then the letters in currTranslation and the word must match at this position irrespective of case
			else if (tolower(currTranslation[i]) == tolower(word[i])) {
				//If true, continue to next character
				continue;
			}
			//Else break out of the inner for loop
			else {
				break;
			}
		}
	}
	//return the vector
	return temp;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
	return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
	return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
	return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
	m_impl = new WordListImpl;
}

WordList::~WordList()
{
	delete m_impl;
}

bool WordList::loadWordList(std::string filename)
{
	return m_impl->loadWordList(filename);
}

bool WordList::contains(std::string word) const
{
	return m_impl->contains(word);
}

std::vector<std::string> WordList::findCandidates(std::string cipherWord, std::string currTranslation) const
{
	return m_impl->findCandidates(cipherWord, currTranslation);
}

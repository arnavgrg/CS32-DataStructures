#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

class TranslatorImpl {
public:
	TranslatorImpl();
	bool pushMapping(string ciphertext, string plaintext);
	bool popMapping();
	string getTranslation(const string& ciphertext) const;
private:
	//Array of characters
	char mapTableTo[26];
	//Array of integers
	int github[26];
	//Keep track of the version
	int versionCount;
};

//CONSTRUCTOR
//////////////////////////////////////////////
TranslatorImpl::TranslatorImpl() {
	//Initialize version count to zero
	versionCount = 0;
	//Go through a loop 26 times to represent the 26 alphabets
	for (int i = 0; i < 26; i++) {
		//Populate mapTableTo with ?
		mapTableTo[i] = '?';
		//Version control (to keep track of which level of the stack) gitHub variable to 0
		github[i] = versionCount;
	}
}

//PUSHMAPPING
///////////////////////////////////////////////////////
bool TranslatorImpl::pushMapping(string ciphertext, string plaintext) {
	//Check if the two parameters have the same length, else return false
	if (ciphertext.size() != plaintext.size()) { return false; }

	//Ensure that if the character is an alphabet and doesn't map to ?, it can only map to one letter
	for (int j = 0; j < ciphertext.size(); j++) {
		if (!isalpha(ciphertext[j])) { continue; }
		if (mapTableTo[tolower(ciphertext[j]) - 'a'] != '?' && mapTableTo[tolower(ciphertext[j]) - 'a'] != tolower(plaintext[j])) {
			return false;
		}
	}

	//Check for collisions
	//Run through all the 26 slots of the array
	for (int i = 0; i < 26; i++) {
		//Run through all the characters in plaintext
		for (int j = 0; j < plaintext.size(); j++) {
			//Ensuring that two characters don't map to the same letter
			if (mapTableTo[i] == tolower(plaintext[j]) && i != (tolower(ciphertext[j]) - 'a')) {
				return false;
			}
		}
	}
	//Update version count if push-mapping is successful
	versionCount++;
	//while i is less than the length of ciphertext
	for (int i = 0; i < ciphertext.size(); i++) {
		//If it is not an alphabet, continue
		if (!isalpha(ciphertext[i])) { continue; }
		//Create an int j and set it to the character at ciphertext[i]
		int j = tolower(ciphertext[i]) - 'a';
		//find the letter in our 2D character array
		//If the jth character in the character array is a ?
		if (mapTableTo[j] == '?') {
			//assign the jth character of plaintext to it
			mapTableTo[j] = tolower(plaintext[i]);
			//the the jth integer in gitHub to the current version count
			github[j] = versionCount;
		}
	}
	//Otherwise return true
	return true;
}

//POPMAPPING
/////////////////////////////////////////////////////
bool TranslatorImpl::popMapping() {
	//If the version is zero, return false
	if (versionCount == 0) { return false; }
	//Pop the last version off the stack
	//Go through all 26 slots in the array
	for (int i = 0; i < 26; i++) {
		//If the version at github[i] == versionCount
		if (github[i] == versionCount) {
			//Set the version back to zero
			github[i] = 0;
			//Set the mapping back to ?
			mapTableTo[i] = '?';
		}
	}
	//Decrement version count
	versionCount--;
	//Return true if all goes correctly
	return true;
}

//GETTRANSLATION
//////////////////////////////////////////////////////////
string TranslatorImpl::getTranslation(const string& ciphertext) const {
	//Create an empty string called plaintext
	string plaintext;
	//Go through all the characters in ciphertext
	for (int i = 0; i < ciphertext.size(); i++) {
		//create a temp character and set it to a lowercase version of the current character
		char temp = tolower(ciphertext[i]);
		//If it isn't an alphabet, just add it on to plaintext
		if (!isalpha(temp)) {
			plaintext += ciphertext[i];
		}
		//Otherwise
		else {
			//If it is uppercase, concatenate an uppercased version to plaintext
			if (isupper(ciphertext[i]))
				plaintext += toupper(mapTableTo[temp - 'a']);
			//Else if it is lowercase, concatenate a lowercase version to plaintext
			else
				plaintext += tolower(mapTableTo[temp - 'a']);
		}
	}
	//Return plaintext
	return plaintext;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
	m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
	delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
	return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
	return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const {
	return m_impl->getTranslation(ciphertext);
}

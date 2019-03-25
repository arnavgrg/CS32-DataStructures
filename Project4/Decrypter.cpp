#include "provided.h"
#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <algorithm>
using namespace std;

class DecrypterImpl {
public:
	bool load(string filename);
	vector<string> crack(const string& ciphertext);
private:
	//Wordlist object to load up all the words
	WordList m_wordlist;
	//Create a mapping (a Translator object)
	Translator mapping;
	//Encrypted Message/Possible Correct Translation
	string text;
	//Vector containing all the tokenized words
	vector<string> tokenizedWords;
	//Final vector that we're going to return with all the possible translations
	vector<string> result;
	//Helper function to decide which word has most ciphertext letters for which we don't have any translation
	string selectWord(vector<string> words);
	//Recursive Function to be called inside crack
	void recurse(vector<string> words);
	//Converting text to lower case
	string lower(string text);
};

//SELECTWORD
////////////////////////////////////////////
string DecrypterImpl::selectWord(vector<string> word) {
	//Create an empty string called result which will be returned at the end
	string result = "";
	//Create a counter for question marks
	int questionCount;
	//Create an int variable to keep track of the word with the maximum number of question marks
	int maxCount = 0;
	//While i is less than the length of the vector
	for (int i = 0; i < word.size(); i++) {
		//Initializ the question mark counter to zero each time the loop is called
		questionCount = 0;
		//Get the current translation of the word in the vector
		string currTranslate = mapping.getTranslation(word[i]);
		//Count the number of question marks in the current translation of the word
		for (int j = 0; j < currTranslate.size(); j++) {
			//If the character in the translation of the word is a question mark, increment the questionCount counter
			if (currTranslate[j] == '?') { questionCount++; }
		}
		//If this word has more question marks than the previous word with maxCount, set result to this word
		if (questionCount >= maxCount) {
			result = word[i];
			//Update maxCount to this value of QuestionCount
			maxCount = questionCount;
		}
	}
	//Return the word with the most number of question marks
	return result;
}

//LOWER
/////////////////////////////////////////////////////
string DecrypterImpl::lower(string text) {
	//Create a temporary string
	string temp;
	//Run through all the characters in the input string
	//Convert them to lower case and concatenate to our temp string
	for (int i = 0; i < text.size(); i++) {
		temp += tolower(text[i]);
	}
	//Return the temp string
	return temp;
}

//RECURSE
////////////////////////////////////////////
void DecrypterImpl::recurse(vector<string> words) {
	//Base Case
	//If the vector is empty, then we've tried all the words
	if (words.empty()) { return; }
	//Algorithm	
	//Pick the word that is least translated
	string selectedWord = selectWord(words);
	//Pop this word from the vector to indicate that it has already been chosen
	for (int i = 0; i < words.size(); i++) {
		if (words[i] == selectedWord) {
			words.erase(words.begin() + i);
		}
	}
	//Get a partial translation for the selected encrypted word
	string translatedWord = mapping.getTranslation(selectedWord);
	//Find the vector of words that the selected encrypted word maps to
	vector<string> translate = m_wordlist.findCandidates(selectedWord, translatedWord);
	//Recurse through the remaining words that match up to the currest partial decryption
	for (int i = 0; i < translate.size(); i++) {
		//Assume the temporary mapping is correct and push this mapping. If it returns false, it tries the next word.
		if (mapping.pushMapping(selectedWord, translate[i])) {
			/*Since this mapping works, translate all the words in the ciphertext
			using this new/updated mapping and push them back onto the new vector of strings*/
			vector<string> newTranslate;
			for (int i = 0; i < tokenizedWords.size(); i++) {
				newTranslate.push_back(mapping.getTranslation(tokenizedWords[i]));
			}
			//Evaluate the just-decrypted message to see if all fully-translated words are in the word list
			int wordCount = 0;
			//boolean as helper
			bool something = true;
			for (int i = 0; i < newTranslate.size(); i++) {
				//If the word does not have a question mark, it is completely translated
				if (newTranslate[i].find('?') == -1) {
					if (!m_wordlist.contains(newTranslate[i])) {
						//pop the mapping
						mapping.popMapping();
						something = false;
						//break out of the function
						break;
					}
					//Else you increment the word counter 
					else { wordCount++; }
				}
			}
			if (!something) { continue; }
			//If the number of words completely translated equals to the tokenized vector's length
			if (wordCount == tokenizedWords.size()) {
				//Push the translation onto the result vector
				result.push_back(mapping.getTranslation(text));
				//Pop the latest mapping
				mapping.popMapping();
			}
			//Otherwise the translation is promising but it is not complete
			else {
				//Call the recurse function again
				recurse(words);
				//pop the last mapping
				mapping.popMapping();
			}
		}
	}
}

//LOAD
////////////////////////////////////////////
bool DecrypterImpl::load(string filename) {
	//if it is able to load the wordlist, return true
	if (m_wordlist.loadWordList(filename)) { return true; }
	//if it is unsuccessful, return false
	else { return false; }
}

//CRACK
///////////////////////////////////////////
vector<string> DecrypterImpl::crack(const string& ciphertext) {
	//Set text to ciphertext
	text = ciphertext;

	//If the translator object has any mappings, pop them.
	while (mapping.popMapping()) { ; }

	//Reset the result vector so that it is empty
	for (int i = 0; i < result.size(); i++) {
		result.erase(result.begin());
	}

	//Create a tokenizer with all the separators
	Tokenizer t(" 0123456789,;:.!()[]{}-#$%^&\"");
	//Update the tokenizedWords vector with all the words after they are tokenized
	tokenizedWords = t.tokenize(lower(ciphertext));

	//Call our recurse 
	recurse(tokenizedWords);

	//Sort the results in order
	sort(result.begin(), result.end());
	//Return result
	return result;
}

//******************** Decrypter functions ***********************************
// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter() {
	m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter() {
	delete m_impl;
}

bool Decrypter::load(string filename) {
	return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext) {
	return m_impl->crack(ciphertext);
}

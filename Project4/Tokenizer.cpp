#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl{
public:
	TokenizerImpl(string separators);
	vector<string> tokenize(const std::string& s) const;
private:
	string m_separators;
};

//Constructor
/////////////////////////////////////////////
TokenizerImpl::TokenizerImpl(string separators){
	m_separators = separators;
}

//Tokenize
/////////////////////////////////////////////
vector<string> TokenizerImpl::tokenize(const std::string& s) const {
	//create temp word
	string temp = "";
	//create a vector of strings
	vector<string> m_wordtokens;
	//Let an int i = 0
	int i = 0;
	//while i is less than the size of the string
	while (i < s.size()) {
		//Let there be an int k that finds the first position where one of the separators exist in the string
		//Use find_first_of and pass in m_separators, i
		int k = s.find_first_of(m_separators, i);
		//update temp to the substr from i to the point k-i
		temp = s.substr(i, k - i);
		//update i using find_first_not_of_ passing m_separators and k.
		i = s.find_first_not_of(m_separators, k);
		//push temp onto the vector we initially created
		m_wordtokens.push_back(temp);
	}
	//return the vector
	return m_wordtokens;
}

//******************** Tokenizer functions ***********************************
// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators){
	m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer(){
	delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const{
	return m_impl->tokenize(s);
}

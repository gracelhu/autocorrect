#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "graceAVL/AVLTree.cpp"
#include "hashtable/HashTable.cpp"
#include "redblack/RedBlack.cpp"
#include <stdlib.h>
#include <queue>
#include <windows.h>
#include <utility>
using namespace std;

void stdHashTable();
void gracesAVL(string userInput);
void graceHashTable(string userInput);
void iansRedBlack(string userInput);

// Hash to convert string to int
//LGTM
int do_hash(string str) {
	unsigned long hash = 5381;
	int c;
	for (auto& ch : str) {
		hash = ((hash << 5) + hash) + ch;
	}
	return (int)hash;
}

// Generates the 4 types of permutations in the article
set<string> scramble(string str) {
	set<string> out = {};
	// #1 insert = helo -> hello 
	//LGTM 
	for (int i = 0; i < str.length()+1; i++) {
		for (int j = 0; j < 26; j++) {
			//converts the ascii (j + 97) to a character and stores it in temp --> going through every letter of alphabet 
			char temp = (char)(j + 97);
			out.insert(str.substr(0, i) + temp + str.substr(i, str.length()));
		}
	}
	// #2 remove = hellol -> hello
	//LGTM 
	for (int i = 0; i < str.length(); i++) {
		out.insert(str.substr(0, i) + str.substr(i+1, str.length()));
	}
	// #3 exchange = hlelo -> hello
	//LGTM 
	for (int i = 0; i < str.length()-1; i++) {
		out.insert(str.substr(0, i) + str.at(i+1) + str.at(i) + str.substr(i + 2, str.length()));
	}
	// #4 replace = helko -> hello
	//LGTM 
	for (int i = 0; i < str.length(); i++) {
		for (int j = 0; j < 26; j++) {
			char temp = (char)(j + 97);
			out.insert(str.substr(0, i) + temp + str.substr(i+1, str.length()));
		}
	}
	return out;
}

// Custom split() function  
vector<pair<string, bool>> split(string str, char seperator) {
	vector<pair<string, bool>> out = {};
	int currIndex = 0, i = 0;
	int startIndex = 0, endIndex = 0;
	while (i <= str.length()) {
		if (str[i] == seperator || i == str.length()) {
			endIndex = i;
			string subStr = "";
			subStr.append(str, startIndex, endIndex - startIndex);

			//Get rid of special characters like period (.) and exclamation point (!)
			if(subStr.find("!") != 4294967295 || subStr.find(".") != 4294967295)
			{
				subStr = subStr.substr(0, subStr.length() - 1);
			}
			out.push_back(make_pair(subStr, false));
			currIndex += 1;
			startIndex = endIndex + 1;
		}
		i++;
	}
	return out;
}

int main() 
{
	/*
	Sample user input to use:
	computer sciencew is the studyt of computatioon, automation, and infirmation. computer sciene spans theoretikal disciplimes to practical disciplinnes.
	compter science is generaly considered an area of accademic research and dastinct from computer programing.
	*/
  
	string userInput;
	int dataStructure;

	cout << "Type in a sentence to be autocorrected: " << endl;
	getline(cin, userInput);
	cout << endl;
	cout << "Would you like to use a red black tree (1) or hash table (2) to autocorrect?" << endl;
	cin >> dataStructure;
	cout << endl;

	if(dataStructure == 1)
	{
		iansRedBlack(userInput);
	}
	if(dataStructure == 2)
	{
		graceHashTable(userInput);
	}  
}

void iansRedBlack(string userInput)
{
	ifstream wordlist("freq.txt");
	string text = "";
	RBNode* root = new RBNode("test", 0.0);
	redBlack* tree = new redBlack(root);

	int stopper = 333333;
	int counter = 0;
	vector<pair<string, bool>> temp = {};
	//only takes about 2.5 seconds to insert everything for avl tree 
	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		tree->insert(temp[0].first, stod(temp[1].first));
		counter++;
	}

	//string test = "i reallly like fruitd and sciencee";
	string test = userInput;

	vector<pair<string, bool>> sentence = split(test, ' ');
	vector<pair<string, bool>> correctedSentence = sentence;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//mark mispelled words bool variable as true + print out the user input with the mispelled ones in a different color :D 
	for(int x = 0; x < sentence.size(); x++)
	{
		if(tree->search(sentence.at(x).first) == nullptr)
		{
			sentence.at(x).second = true;
			SetConsoleTextAttribute(hConsole, 4); 
			cout << sentence.at(x).first << " ";
			SetConsoleTextAttribute(hConsole, 15); 
		}
		else
		{
			cout << sentence.at(x).first << " ";
		}
	}

	cout << endl;
	cout << endl;

	for (int x = 0; x < sentence.size(); x++) 
	{
		if(sentence.at(x).second == true)
		{
		string word = sentence.at(x).first;

		set<string> scram = scramble(word);
		map<double, string> valid = {};
		// Check which permutations exist in dictionary
		for (string i : scram) {
			if (tree->search(i)) {
				valid.emplace(tree->search(i)->frequency, i);
			}
		}

		// Print based on which valid permutation has highest frequency
		if (!valid.empty()) 
		{
			string print = word;
			double max = 0.0;
			double original = 0.0;
			// If original word exists in dictionary, grab its frequency too
			if (tree->search(word)) {
				double max = tree->search(word)->frequency;
				original = max;
			}
			//cout << "valid size: " << valid.size() << endl;
			vector<pair<string, int>> topThree;
			auto iter = valid.end();
			iter--;
			while(topThree.size() != 3 && iter != valid.begin())
			{
					topThree.push_back(make_pair(iter->second, iter->first));
					iter--;
			}

			cout << "Did you mean these words for " << '"' << word << '"' << "?" << endl;
			for(int x = 1; x <= topThree.size(); x++)
			{
				cout << x << ".) " << topThree.at(x - 1).first << endl;
			}
			string yesOrNo;
			cout << endl;
			cout << "Y or N?" << endl;
			cin >> yesOrNo;
			
			if(yesOrNo == "Y")
			{
				int option;
				cout << "1 2 or 3?" << endl;
				cin >> option;

				correctedSentence.at(x) = make_pair(topThree.at(option - 1).first, true);
			}
			cout << endl; 
		} 
		}
	}

	cout << "Corrected sentence: " << endl;
	for(int x = 0; x < correctedSentence.size(); x++)
	{
		if(correctedSentence.at(x).second == true)
		{
			SetConsoleTextAttribute(hConsole, 10); 
			cout << correctedSentence.at(x).first << " ";
			SetConsoleTextAttribute(hConsole, 15); 
		}
		else
		{
			cout << correctedSentence.at(x).first << " ";
		}
	} 

	//you need to do this and not just called delete tree because delete tree will only delete the root and not all the other nodes 
	//tree->destruct();
	wordlist.close();   

}

void graceHashTable(string userInput)
{
	ifstream wordlist("freq.txt");
	string text = "";
	HashTable table;

	int stopper = 333333;
	int counter = 0;
	vector<pair<string, bool>> temp = {};

	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		table.insertWord(temp[0].first, stod(temp[1].first));
		counter++;
	}

	//string test = "i reallly like fruitd and sciencee";
	string test = userInput;
	
	//string of pair represents the word, bool of pair represents if it's a mispelled word or not 
	vector<pair<string, bool>> sentence = split(test, ' ');
	vector<pair<string, bool>> correctedSentence = sentence;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for(int x = 0; x < sentence.size(); x++)
	{
		if(table.find(sentence.at(x).first) == nullptr)
		{
			sentence.at(x).second = true;
			SetConsoleTextAttribute(hConsole, 4); 
			cout << sentence.at(x).first << " ";
			SetConsoleTextAttribute(hConsole, 15); 
		}
		else
		{
			cout << sentence.at(x).first << " ";
		}
	}

	cout << endl;
	cout << endl;

	for (int x = 0; x < sentence.size(); x++) 
	{
		//only do this if the word is considered mispelled (not found in dictionary)
		if(sentence.at(x).second == true)
		{
		string word = sentence.at(x).first;
		set<string> scram = scramble(word);
		map<double, string> valid = {};
		// Check which permutations exist in dictionary
		for (string i : scram) {
			if (table.find(i)) {
				valid.emplace(table.find(i)->frequency, i);
			}
		}
		
		// Print based on which valid permutation has highest frequency
		if (!valid.empty()) 
		{
			string print = word;
			double max = 0.0;
			double original = 0.0;
			// If original word exists in dictionary, grabs its frequency too
			if (table.find(word)) {
				double max = table.find(word)->frequency;
				original = max;
			}

			vector<string> topThree;
			auto iter = valid.end();
			iter--;
			while(topThree.size() != 3 && iter != valid.begin())
			{
					topThree.push_back(iter->second);
					iter--;
			}

			cout << "Did you mean these words for " << '"' << word << '"' << "?" << endl;
			for(int x = 1; x <= topThree.size(); x++)
			{
				cout << x << ".) " << topThree.at(x - 1) << endl;
			}
			string yesOrNo;
			cout << endl;
			cout << "Y or N?" << endl;
			cin >> yesOrNo;
			
			if(yesOrNo == "Y")
			{
				int option;
				cout << "1, 2, or 3?" << endl;
				cin >> option;

				correctedSentence.at(x) = make_pair(topThree.at(option - 1), true);
			}
			cout << endl; 
		}
		}
	}

	cout << "Corrected sentence: " << endl;
	for(int x = 0; x < correctedSentence.size(); x++)
	{
		if(correctedSentence.at(x).second == true)
		{
			SetConsoleTextAttribute(hConsole, 10); 
			cout << correctedSentence.at(x).first << " ";
			SetConsoleTextAttribute(hConsole, 15); 
		}
		else
		{
			cout << correctedSentence.at(x).first << " ";
		}
	} 

	//destruct table 
	table.deleteAllWords();
	wordlist.close();   

}
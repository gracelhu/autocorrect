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
using namespace std;

void stdHashTable();
void gracesAVL();
void iansRedBlack();
void graceHashTable();

// Generates the 4 types of permutations in the article
set<string> scramble(string str) {
	set<string> out = {};
	// #1 insert = helo -> hello 
	for (int i = 0; i < str.length()+1; i++) {
		for (int j = 0; j < 26; j++) {
			//converts the ascii (j + 97) to a character and stores it in temp --> going through every letter of alphabet 
			char temp = (char)(j + 97);
			out.insert(str.substr(0, i) + temp + str.substr(i, str.length()));
		}
	}
	// #2 remove = hellol -> hello
	for (int i = 0; i < str.length(); i++) {
		out.insert(str.substr(0, i) + str.substr(i+1, str.length()));
	}
	// #3 exchange = hlelo -> hello
	for (int i = 0; i < str.length()-1; i++) {
		out.insert(str.substr(0, i) + str.at(i+1) + str.at(i) + str.substr(i + 2, str.length()));
	}
	// #4 replace = helko -> hello
	for (int i = 0; i < str.length(); i++) {
		for (int j = 0; j < 26; j++) {
			char temp = (char)(j + 97);
			out.insert(str.substr(0, i) + temp + str.substr(i+1, str.length()));
		}
	}
	return out;
}

// Custom split() function  
vector<string> split(string str, char seperator) {
	vector<string> out = {};
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
			out.push_back(subStr);
			currIndex += 1;
			startIndex = endIndex + 1;
		}
		i++;
	}
	return out;
}

int main() 
{
	//stdHashTable();
	gracesAVL();
	//graceHashTable();
}

void gracesAVL()
{
	ifstream wordlist("freq.txt");
	string text = "";
	Node* root = new Node("test", 0.0);
	AVLTree tree(root);

	int stopper = 10;
	int counter = 0;
	vector<string> temp = {};
	//only takes about 2.5 seconds to insert everything for avl tree 
	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		tree.insert(temp[0], stod(temp[1]));
		counter++;
	}

	tree.printLevelCount();
	tree.printInorderWords();
	tree.printInorderFreqs();

	string test = "i reallly like fruitd and sciencee";
	
	cout << test << endl;
	vector<string> sentence = split(test, ' ');

	for (string word : sentence) 
	{
		set<string> scram = scramble(word);
		map<double, string> valid = {};
		// Check which permutations exist in dictionary
		for (string i : scram) {
			if (tree.search(i)) {
				//why is the tree node's name not initialized?? 
				//cout << tree.search(i)->NAME << endl;
				valid.emplace(tree.search(i)->freq, i);
			}
		}
		cout << "valid size: " << valid.size() << endl;
		// Print based on which valid permutation has highest frequency
		if (!valid.empty()) 
		{
			string print = word;
			double max = 0.0;
			double original = 0.0;
			// If original word exists in dictionary, grabs its frequency too
			if (tree.search(word)) {
				double max = tree.search(word)->freq;
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
			cout << endl;
		}
	}

	//you need to do this and not just called delete tree because delete tree will only delete the root and not all the other nodes 
	tree.destruct();
	wordlist.close();   
}

void iansRedBlack()
{
	ifstream wordlist("freq.txt");
	string text = "";
	Node* root = new Node("test", 0.0);
	redBlack tree(root);

	int stopper = 10;
	int counter = 0;
	vector<string> temp = {};
	//only takes about 2.5 seconds to insert everything for avl tree 
	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		tree.insert(temp[0], stod(temp[1]));
		counter++;
	}

	tree.printLevelCount();
	tree.printInorderWords();
	tree.printInorderFreqs();

	string test = "i reallly like fruitd and sciencee";
	
	cout << test << endl;
	vector<string> sentence = split(test, ' ');

	for (string word : sentence) 
	{
		set<string> scram = scramble(word);
		map<double, string> valid = {};
		// Check which permutations exist in dictionary
		for (string i : scram) {
			if (tree.search(i)) {
				//why is the tree node's name not initialized?? 
				//cout << tree.search(i)->NAME << endl;
				valid.emplace(tree.search(i)->freq, i);
			}
		}
		cout << "valid size: " << valid.size() << endl;
		// Print based on which valid permutation has highest frequency
		if (!valid.empty()) 
		{
			string print = word;
			double max = 0.0;
			double original = 0.0;
			// If original word exists in dictionary, grabs its frequency too
			if (tree.search(word)) {
				double max = tree.search(word)->freq;
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
			cout << endl;
		}
	}

	//you need to do this and not just called delete tree because delete tree will only delete the root and not all the other nodes 
	tree.destruct();
	wordlist.close();   
}

void graceHashTable()
{
	ifstream wordlist("freq.txt");
	string text = "";
	HashTable table;

	int stopper = 333333;
	int counter = 0;
	vector<string> temp = {};

	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		table.insertWord(temp[0], stod(temp[1]));
		counter++;
	}

	string test = "i reallly like fruitd and sciencee";
	
	cout << test << endl;
	vector<string> sentence = split(test, ' ');

	for (string word : sentence) 
	{
		set<string> scram = scramble(word);
		map<double, string> valid = {};
		// Check which permutations exist in dictionary
		for (string i : scram) {
			if (table.find(i)) {
				valid.emplace(table.find(i)->frequency, i);
			}
		}
		
		cout << "valid size: " << valid.size() << endl;
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
			cout << endl;
		}
	}
	//destruct table 
	table.deleteAllWords();
	wordlist.close();   

}

void stdHashTable()
{
	ifstream wordlist("freq.txt");
	string text = "";
	unordered_map<string, int> dictionary;

	int stopper = 333333;
	int counter = 0;
	vector<string> temp = {};
	//only takes about 2.3 seconds to insert everything for unordered_map (avl is only slightly slower)
	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		dictionary[temp.at(0)] = stod(temp.at(1));
		counter++;
	}

	string test = "i reallly like fruitd and sciencee";

	cout << test << endl;
	vector<string> sentence = split(test, ' ');

	for (string word : sentence) {
		set<string> scram = scramble(word);
		//sort ordered valid map in descending order instead of ascending order so I can just get the top 3 
		map<double, string> valid = {};
		// Check which permutations exist in dictionary
		for (string i : scram) {
			if (dictionary.find(i) != dictionary.end()) {
				valid.emplace(dictionary[i], i);
			}
		}
		
		// Print based on which valid permutation has highest frequency
		if (!valid.empty()) 
		{
			string print = word;
			double max = 0.0;
			double original = 0.0;
			// If original word exists in dictionary, grabs its frequency too
			if (dictionary.find(word) != dictionary.end()) {
				double max = dictionary[word];
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
			cout << endl;
		}
	}
}
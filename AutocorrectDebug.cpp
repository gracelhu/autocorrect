#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <regex>
#include <unordered_set>
#include "graceAVL/AVLTree.h"
#include "hashtable/HashTable.cpp"
#include <stdlib.h>
#include <queue>
using namespace std;

//this file is for playing around with making the autocorrect algorithm more accurate 

void stdHashTable();
AVLTree* gracesAVL();
void graceHashTable();
string fixtypo(string word, AVLTree* tree);
map<string, string> getTypoBody();

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
	//cout << "str = " << str << endl;
	set<string> out = {};
	// #1 insert = helo -> hello 
	for (int i = 0; i < str.length() + 1; i++) {
		for (int j = 0; j < 26; j++) {
			//converts the ascii (j + 97) to a character and stores it in temp --> going through every letter of alphabet 
			char temp = (char)(j + 97);
			out.insert(str.substr(0, i) + temp + str.substr(i, str.length()));
		}
	}
	if (str.length() > 1) {
		// #2 remove = hellol -> hello
		for (int i = 0; i < str.length(); i++) {
			out.insert(str.substr(0, i) + str.substr(i + 1, str.length()));
		}
		// #3 exchange = hlelo -> hello
		for (int i = 0; i < str.length() - 1; i++) {
			out.insert(str.substr(0, i) + str.at(i + 1) + str.at(i) + str.substr(i + 2, str.length()));
		}
		// #4 replace = helko -> hello
		for (int i = 0; i < str.length(); i++) {
			for (int j = 0; j < 26; j++) {
				char temp = (char)(j + 97);
				out.insert(str.substr(0, i) + temp + str.substr(i + 1, str.length()));
			}
		}
	}
	return out;
}

set<string> scramble(set<string> str) {
	set<string> out = {};
	//for (set<string>::iterator itr = str.begin(); itr != str.end(); itr++) {
	for (string itr : str) {
		//cout << itr << ".";
		if (out.size() == 0 || out.find(itr) != out.end()) {
			set<string> scram = scramble(itr);
			merge(out.begin(), out.end(), scram.begin(), scram.end(), inserter(out, out.begin()));
		}
	}
	return out;
}

// Custom split() function  
vector<string> split(string str, char delimiter) {
	vector<string> out = {};
	int prev = 0;
	int i = 0;
	regex purge("[.!]");
	str = regex_replace(str, purge, "");
	while (i <= str.length()) {
		if (str[i] == delimiter || i == str.length()) {
			out.push_back(str.substr(prev, i-prev));
			prev = i+1;
		}
		i++;
	}
	return out;
}

int main()
{
	cout << "HEWOWOO" << endl;
	//stdHashTable();
	//gracesAVL();
	//graceHashTable();
	map<string, string> typobody = getTypoBody();
	/*
	string test = "test";
	set<string> tv = scramble(scramble(test));
	for (set<string>::iterator itr = tv.begin(); itr != tv.end(); itr++) {
		cout << *itr << " ";
	}
	*/
	
	
	AVLTree* tree = gracesAVL();
	/*
	string test = "i reallly like fruitdd and sciencee";
	cout << test << endl;
	vector<string> sentence = split(test, ' ');
	string print = "";
	for (int i = 0; i < sentence.size(); i++) {
		cout << "word = " << sentence[i] << endl;
		print += fixtypo(sentence[i], tree) + " ";
	}
	cout << print << endl;
	*/
	int worked = 0;
	float display = 0.01;
	int total = typobody.size();
	for (auto const& x : typobody) {
		string fix = fixtypo(x.first, tree);
		worked += (fix == x.second);
		if (((float)worked / (float)total) > display) {
			cout << ((float)worked / (float)total) << endl;
			display += 0.01;
		}
		if (fix == x.second) {
			cout << (to_string(fix == x.second) + ": " + x.first + ", " + x.second + ", " + fix) << endl;
		}
	}
	cout << "worked = " << worked << endl;
	cout << "total = " << total << endl;
	
	tree->destruct();
	
}

map<string, string> getTypoBody() {
	cout << "Loading typo corpus..." << endl;
	ifstream wordlist("typos.txt");
	string text = "";
	string target_word = "";
	int stopper = 4000;
	int counter = 0;
	map<string, string> typo_body = {};
	while (getline(wordlist, text) && counter < stopper) {
		if (text.substr(0, 1) == "$") {
			target_word = text.substr(1, text.length() - 1);
		} else {
			typo_body.emplace(text, target_word);
		}
		counter++;
	}
	wordlist.close();
	cout << "...Typo corpus loaded." << endl;
	return typo_body;
}

AVLTree* gracesAVL() {
	cout << "Loading dictionary with AVL tree..." << endl;
	ifstream wordlist("freq.txt");
	string text = "";
	Node* root = new Node("test", do_hash("test"), 0.0);
	//AVLTree* tree = new AVLTree(root);
	//We actually don't need to dynamically allocate our tree because the tree just contains the root of the tree 
	//However, we dynamically allocate the nodes of the tree because it's possible we'll need to insert millions of nodes, and the stack might be 
	//too small to store all those nodes. But the tree object itself can go on the stack and point to the root that's located in dynamic memory 
	AVLTree* tree = new AVLTree(root);

	int stopper = 100000;//333333;
	int counter = 0;
	vector<string> temp = {};
	//only takes about 2.5 seconds to insert everything for avl tree 
	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		tree->insert(temp[0], do_hash(temp[0]), stod(temp[1]));
		counter++;
	}
	wordlist.close();
	cout << "...Finished loading dictionary with AVL tree." << endl;
	return tree;
}

string fixtypo(string word, AVLTree* tree) {
	string out = word;
	set<string> scram = scramble(word);
	map<double, string> valid = {};
	// Check which permutations exist in dictionary
	for (string i : scram) {
		//cout << i << " ";
		if (tree->search(do_hash(i))) {
			valid.emplace(tree->search(do_hash(i))->freq, i);
		}
	}
	// Perform second iteration if word not found
	/*
	if (valid.empty()) {
		cout << "Doing second loop" << endl;
		scram = scramble(scram);
		for (string i : scram) {
			if (tree.search(do_hash(i))) {
				valid.emplace(tree.search(do_hash(i))->freq, i);
			}
		}
	}
	*/

	//cout << "valid size: " << valid.size() << endl;
	// Print based on which valid permutation has highest frequency
	if (!valid.empty()) {
		string print = word;
		double max = 0;
		double original = -1;
		// If original word exists in dictionary, grabs its frequency too
		if (tree->search(do_hash(word))) {
			double max = tree->search(do_hash(word))->freq;
			original = max;
		}
		// Iterates naively to find max freq permutation
		for (auto const& x : valid) {
			if (x.first > max) {
				max = x.first;
				print = x.second;
			}
		}
		// If the premutation with max freq is 100 times more
		// common than the original word, replace it
		// Otherwise just print the original word
		if (original != 0 && (max / original > 100 || original == -1)) {
			out = print;
		} else {
			out = word;
		}
	}
	return out;
}

/*
void AVLparse_vector(AVLTree tree, string test) {
	cout << test << endl;
	vector<string> sentence = split(test, ' ');

	set<string> scram = scramble(word);
		map<double, string> valid = {};
		// Check which permutations exist in dictionary
		for (string i : scram) {
			if (tree.search(do_hash(i))) {
				valid.emplace(tree.search(do_hash(i))->freq, i);
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
			if (tree.search(do_hash(word))) {
				double max = tree.search(do_hash(word))->freq;
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
*/

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
			while (topThree.size() != 3 && iter != valid.begin())
			{
				topThree.push_back(iter->second);
				iter--;
			}

			cout << "Did you mean these words for " << '"' << word << '"' << "?" << endl;
			for (int x = 1; x <= topThree.size(); x++)
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
	//Used a hash table (O(1) access time) to store all the words in the dictionary) --> unordered_map (key: word, value: frequency)
	//Executes much faster --> takes about 2.4-2.5 seconds for a sentence, which means it only takes about 0.1-0.2 seconds 
	//to actually autocorrect (2.3 seconds to insert everything)
	//try to figure out how to make a hash table from scratch 

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
			while (topThree.size() != 3 && iter != valid.begin())
			{
				topThree.push_back(iter->second);
				iter--;
			}

			cout << "Did you mean these words for " << '"' << word << '"' << "?" << endl;
			for (int x = 1; x <= topThree.size(); x++)
			{
				cout << x << ".) " << topThree.at(x - 1) << endl;
			}
			cout << endl;
		}
	}
}
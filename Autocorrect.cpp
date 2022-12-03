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
//#include "redblack/RedBlack.cpp"
#include <stdlib.h>
#include <queue>
#include <regex>
using namespace std;

//void stdHashTable();
//AVLTree gracesAVL();
//void iansRedBlack();
//void graceHashTable();

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

AVLTree gracesAVL() {
	cout << "Building tree" << endl;
	ifstream wordlist("freq.txt");
	string text = "";
	Node* root = new Node("test", 0.0);
	AVLTree tree(root);

	int stopper = 10000;
	int counter = 0;
	vector<string> temp = {};
	//only takes about 2.5 seconds to insert everything for avl tree 
	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		tree.insert(temp[0], stod(temp[1]));
		counter++;
	}
	wordlist.close();
	cout << "Finished building tree" << endl;
	return tree;
}

HashTable gracesHashTable() {
	ifstream wordlist("freq.txt");
	string text = "";
	HashTable table;

	int stopper = 10000;
	int counter = 0;
	vector<string> temp = {};

	while (getline(wordlist, text) && counter < stopper) {
		temp = split(text, ',');
		table.insertWord(temp[0], stod(temp[1]));
		counter++;
	}

	wordlist.close();
	return table;
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

string fixtypo(string word, AVLTree& tree) {
	string out = word;
	set<string> scram = scramble(word);
	map<double, string> valid = {};
	// Check which permutations exist in dictionary
	for (string i : scram) {
		//cout << i << " ";
		Node* temp = tree.search(i);
		if (temp) {
			cout << temp->freq << endl;
			//valid.emplace(temp->freq, i);
		}
	}
	/*
	
	*/
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
	/*
	if (!valid.empty()) {
		string print = word;
		double max = 0;
		double original = -1;
		// If original word exists in dictionary, grabs its frequency too
		if (tree.search(word)) {
			double max = tree.search(word)->freq;
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
	*/
	return out;
}

string fixtypo(string word, HashTable& table) {
	string out = word;
	set<string> scram = scramble(word);
	map<double, string> valid = {};
	// Check which permutations exist in dictionary
	for (string i : scram) {
		//cout << i << " ";
		Word* temp = table.find(i);
		if (temp) {
			//cout << temp->frequency << endl;
			valid.emplace(temp->frequency, i);
		}
	}
	//cout << "valid size: " << valid.size() << endl;
	// Print based on which valid permutation has highest frequency
	
	if (!valid.empty()) {
		string print = word;
		double max = 0;
		double original = -1;
		// If original word exists in dictionary, grabs its frequency too
		if (table.find(word)) {
			double max = table.find(word)->frequency;
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

int main() 
{
	//stdHashTable();
	//gracesAVL();
	//graceHashTable();
	map<string, string> typobody = getTypoBody();
	cout << "Exiting main function to build tree" << endl;
	AVLTree tree = gracesAVL();
	HashTable table = gracesHashTable();
	cout << "Back into main function" << endl;
	int worked = 0;
	float display = 0.01;
	int total = typobody.size();
	int counter = 0;
	cout << "size = " << typobody.size() << endl;
	for (auto const& x : typobody) {
		counter++;
		//cout << "Looping " << counter << " word = " << x.first << endl;
		
		//string fix = fixtypo(x.first, tree);
		string fix = fixtypo(x.first, table);
		//cout << "fix = " << fix << endl;
		worked += (fix == x.second);
		
		/*
		if (((float)worked / (float)total) > display) {
			cout << ((float)worked / (float)total) << endl;
			display += 0.01;
		}
		*/
		if (fix == x.second) {
			cout << (to_string(fix == x.second) + ": " + x.first + ", " + x.second + ", " + fix) << endl;
		}
		//cout << (to_string(fix == x.second) + ": " + x.first + ", " + x.second + ", " + fix) << endl;
		
	}
	cout << "worked = " << worked << endl;
	cout << "total = " << total << endl;
	
	tree.destruct();
}



/*
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
*/

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


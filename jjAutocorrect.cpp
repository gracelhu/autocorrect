#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "Autocorrector/Autocorrector.cpp"
#include <stdlib.h>
#include <queue>
#include <regex>
using namespace std;

#include <algorithm>
#include <chrono>
#include <iostream>
#include <ctime>
using namespace std::chrono;

#include <stdlib.h>
#include <queue>
#include <windows.h>
#include <utility>

string permute(string str) {
	string out = "";
	int rnd = rand();
	if (rnd % 4 == 0) {
		// #1 insert = helo -> hello
		int i = rand() % str.length();
		int j = rand() % 26;
		// converts the ascii (j + 97) to a character and stores it in temp
		// loop is going through every letter of alphabet 
		char temp = (char)(j + 97);
		out = (str.substr(0, i) + temp + str.substr(i, str.length()));
	} if (rnd % 4 == 1) {
		// #2 remove = hellol -> hello
		int i = rand() % str.length();
		for (int i = 0; i < str.length(); i++) {
			out = (str.substr(0, i) + str.substr(i + 1, str.length()));
		}
	} if (rnd % 4 == 2) {
		// #3 exchange = hlelo -> hello
		int i = rand() % str.length();
		out = (str.substr(0, i) + str.at(i + 1) + str.at(i) + str.substr(i + 2, str.length()));

	} if (rnd % 4 == 3) {
		// #4 replace = helko -> hello
		int i = rand() % str.length();
		int j = rand() % 26;
		char temp = (char)(j + 97);
		out = (str.substr(0, i) + temp + str.substr(i + 1, str.length()));
	}
	return out;
}

int main() {
	srand(time(nullptr));
	Autocorrector redBlackAC = Autocorrector();
	redBlackAC.buildRBtree();
	Autocorrector hashTableAC = Autocorrector();
	hashTableAC.buildHashTable();
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	
	// PART 1 - SAMPLE TEXT

	cout << endl << "PART 1: SAMPLE TEXT" << endl;

	string sample = "";
	string text = "";
	ifstream excerpt("excerpt.txt");
	while (getline(excerpt, text)) {
		sample += text + "\n";
	}
	excerpt.close();
	string scram = sample;
	//sample = "This is a test sentence.";
	for (int i = 0; i < 50; i++) {
		scram = permute(scram);
	}
	
	cout << "Original sample (1984, George Orwell)" << endl;
	cout << "---------------------" << endl;
	cout << sample << endl << endl;
	
	cout << "Scrambled sample" << endl; // scrambled sample contains the typos 
	cout << "---------------------" << endl;
	//cout << scram << endl << endl;
	
	vector<string> split_sample = redBlackAC.split(scram, " .,()");
	for (auto x : split_sample) {
		//if the word is misspelled (doesn't exist in dictionary), color it RED

		//search for the word lowercase version, and get rid of punctuation 
		string editedX = x;
		transform(editedX.begin(), editedX.end(), editedX.begin(), ::tolower);
		regex purge("[.!,:]");
		editedX = regex_replace(editedX, purge, "");

		if(redBlackAC.search(editedX) == false)
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << x;
			SetConsoleTextAttribute(hConsole, 7);
		}
		else
		{
			cout << x;
		}
	}
	cout << endl << endl;
	cout << "Autocorrected" << endl;
	cout << "---------------------" << endl; 
	//cout << print << endl << endl;

	for (auto x : split_sample) {
		string correctedWord = redBlackAC.fixtypo(x).at(0);	
		//if the word was misspelled before (doesn't exist in dictionary), color it green 
		string editedX = x;
		transform(editedX.begin(), editedX.end(), editedX.begin(), ::tolower);
		regex purge("[.!,:]");
		editedX = regex_replace(editedX, purge, "");

		if(redBlackAC.search(editedX) == false)
		{
			SetConsoleTextAttribute(hConsole, 10);
			cout << correctedWord;
			SetConsoleTextAttribute(hConsole, 7);
		}
		else
		{
			cout << correctedWord;
		}
	} 

	// PART 2 - TYPO CORPUS
	cout << endl << endl << "PART 2: TYPO CORPUS" << endl;

	map<string, string> typoBody;
	cout << "Loading typo corpus..." << endl;
	ifstream wordlist("typos.txt");
	text = "";
	string target_word = "";
	int stopper = 4000;
	int counter = 0;
	//map<string, string> typo_body = {};
	while (getline(wordlist, text) && counter < stopper) {
		if (text.substr(0, 1) == "$") {
			target_word = text.substr(1, text.length() - 1);
		} else {
			typoBody.emplace(text, target_word);
		}
		counter++;
	}
	wordlist.close();
	cout << "...Typo corpus loaded." << endl;

	cout << "Checking typos" << endl << endl;
	
	//For RB Tree 
	cout << "Using Red Black Tree: " << endl;
	float RBworked = 0;
	auto RBstart = high_resolution_clock::now();
	for (auto const& x : typoBody) {
		string fix = redBlackAC.fixtypo(x.first).at(0);
		RBworked += (fix == x.second);
		if (fix == x.second) {
			//cout << (to_string(fix == x.second) + ": " + x.first + " -> " + x.second + ", = " + fix) << endl;
		}
	}

	auto RBstop = high_resolution_clock::now();
	float RBtotal = (float)typoBody.size();
	cout << "worked = " << RBworked << endl;
	cout << "total = " << RBtotal << endl;
	auto RBduration = duration_cast<seconds>(RBstop - RBstart);
	cout << "success ratio = " << RBworked / RBtotal << endl;
	cout << "Red black execution time = " << RBduration.count() << " s" << endl << endl;

	//For Hash Table
	cout << "Using Hash Table: " << endl;
	float hashWorked = 0;
	auto hashStart = high_resolution_clock::now();
	for (auto const& x : typoBody) {
		string fix = hashTableAC.fixtypo(x.first).at(0);
		hashWorked += (fix == x.second);
		if (fix == x.second) {
			//cout << (to_string(fix == x.second) + ": " + x.first + " -> " + x.second + ", = " + fix) << endl;
		}
	}

	auto hashStop = high_resolution_clock::now();
	float hashTotal = (float)typoBody.size();
	cout << "worked = " << hashWorked << endl;
	cout << "total = " << hashTotal << endl;
	auto hashDuration = duration_cast<seconds>(hashStop - hashStart);
	cout << "success ratio = " << hashWorked / hashTotal << endl;
	cout << "Hash table execution time = " << hashDuration.count() << " s" << endl << endl;

	
	// PART 3 - INTERACTIVE

	cout << endl << "PART 3: INTERACTIVE" << endl;

	//fruitt
	string userInput;
	int dataStructure;

	cout << "Type in a sentence to be autocorrected: " << endl;
	getline(cin, userInput);
	cout << endl;

	vector<string> sentence = redBlackAC.split(userInput, " .,()");
	vector<string> fix = {};
	vector<string> fix_after = {};
	
	// Iterates through sentence and colors words based on whether they have a valid replacement
	for (auto it : sentence) {
		vector<string> options = redBlackAC.fixtypo(it, 3);
		if (options[0] == it) {
			cout << it;
		} else {
			SetConsoleTextAttribute(hConsole, 12);
			cout << it;
			SetConsoleTextAttribute(hConsole, 7);
			fix.push_back(it);
		}
	}
	cout << endl;

	// Loops through the red words and queries the user for input
	for (auto it : fix) {
		cout << "Did you mean these words for " << '"' << it << '"' << "?" << endl;
		vector<string> options = redBlackAC.fixtypo(it, 3);
		for (int i = 0; i < options.size(); i++) {
			cout << i+1 << ".) " << options[i] << endl;
		}
		int option;
		cout << "1 2 or 3?" << endl;
		cin >> option;

		// Clamps input
		option--;
		if (option >= options.size()) {
			option = options.size()-1;
		} else if (option < 0) {
			option = 0;
		}

		// Replaces word in original sentence with selected word
		auto index = find(sentence.begin(), sentence.end(), it);
		*index = options[option];
		fix_after.push_back(options[option]);
		cout << endl;
	}

	for (auto it : sentence) {
		if (find(fix_after.begin(), fix_after.end(), it) != fix_after.end()) {
			SetConsoleTextAttribute(hConsole, 10);
			cout << it;
			SetConsoleTextAttribute(hConsole, 7);
		} else {
			cout << it;
		}
	}


	// DESTRUCT
	
}
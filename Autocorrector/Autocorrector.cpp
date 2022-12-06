#include "Autocorrector.h"

Autocorrector::Autocorrector() {
    isRB = false;
	isHash = false;
    redBlack rbtree = redBlack();
}

// Generates the 4 types of permutations in the article
set<string> Autocorrector::scramble(string str) {
	set<string> out = {};
	if (str == "") { return out; }
	regex purge("[ .,]");
	if (regex_match(str, purge)) { return out; }
	// #1 insert = helo -> hello 
	for (int i = 0; i < str.length()+1; i++) {
		for (int j = 0; j < 26; j++) {
			// converts the ascii (j + 97) to a character and stores it in temp
			// loop is going through every letter of alphabet 
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
vector<string> Autocorrector::split(string str, char delimiter) {
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

vector<string> Autocorrector::split(string str, string delimiters) {
	vector<string> out = {};
	int prev = 0;
	int i = 0;
	regex delim("[" + delimiters + "]");
	while (i <= str.length()) {
		string it = string(1, str[i]);
		if (str.substr(prev, i - prev) != "" && (it == " " || regex_match(it, delim) || i == str.length() - 1)) {
			out.push_back(str.substr(prev, i - prev));
			out.push_back(it);
			prev = i + 1;
			it = string(1, str[prev]);
			while (it == " " || regex_match(it, delim)) {
				out.push_back(it);
				i++;
				prev++;
				it = string(1, str[prev]);
			}
		}
		i++;
	}
	return out;
}

void Autocorrector::buildRBtree() {
    if (isHash) {
        cout << "Already using hash table!" << endl;
        return;
    } else if (isRB) {
        cout << "Red-black tree has already been built!" << endl;
        return;
    } else {
        isRB = true;
        cout << "Building word corpus using red-black tree..." << endl;
    }
	//ifstream wordlist("freq.txt");
	ifstream wordlist("frequency_list.txt");
	string text = "";
	int stopper = 150000;
	int counter = 0;
	string word = "";
	vector<string> temp = {};
	while (getline(wordlist, text) && counter < stopper) {
		if (counter % 2 == 0) {
			word = text;
		} else {
			rbtree.insert(word, stod(text));
		}
		counter++;

	}
	wordlist.close();
	cout << "Finished building red-black tree" << endl;
}

void Autocorrector::buildHashTable() {
    if (isRB) {
        cout << "Already using red-black tree!" << endl;
        return;
    } else if (isHash) {
        cout << "Hash table has already been built!" << endl;
        return;
    } else {
        isHash = true;
        cout << "Building word corpus using hash table..." << endl;
    }
	ifstream wordlist("frequency_list.txt");
	string text = "";
	int stopper = 150000;
	int counter = 0;
	string word = "";
	vector<string> temp = {};
	while (getline(wordlist, text) && counter < stopper) {
		if (counter % 2 == 0) {
			word = text;
		} else {
			hashtable.insertWord(word, stod(text));
		}
		counter++;
	}
	wordlist.close();
    cout << "Finished building hash table" << endl;
}

bool Autocorrector::search(string word)
{
	if(isRB == true)
	{
		if(rbtree.search(word) != nullptr)
			return true;
		else
			return false;
	}
	else if(isHash == true)
	{
		if(hashtable.find(word) != nullptr)
			return true;
		else
			return false;
	}
	return false;
}

// Pair comparison
struct Comp {
	bool operator()(const pair<string, double>& s1, const pair<string, double>& s2) {
		return s1.second < s2.second;
	}
};

vector<string> Autocorrector::fixtypo(string word) {
	return fixtypo(word, 1);
}

vector<string> Autocorrector::fixtypo(string word, int count) {
	vector<string> out = {};
	if (!isRB && !isHash) {
        cout << "Dictionary has not been initialized!" << endl;
		out.push_back(word);
        return out;
    }
	if (word == "" || word == " ") {
		out.push_back(word);
		return out;
	}
	regex purge("[.,()!?]");
	if (regex_match(word, purge)) {
		out.push_back(word);
		return out;
	}

	// Generate permutations
	//string out = word;
	set<string> scram = scramble(word);

	// Check which permutations exist in dictionary
	// Adds them to a vector which gets converted to a heap
	vector<pair<string, double>> valid = {};
	for (string i : scram) {
        if (isRB) {
            RBNode* temp = rbtree.search(i);
			bool exists = false;
			for (auto it : valid) {
				if (it.first == i) { exists = true; }
			}
			if (temp && !exists) {
				valid.push_back(make_pair(i, abs(temp->frequency)));
			}
        } else if (isHash) {
            Word* temp = hashtable.find(i);
			bool exists = false;
			for (auto it : valid) {
				if (it.first == i) { exists = true; }
			}
			if (temp && !exists) {
				valid.push_back(make_pair(i, abs(temp->frequency)));
			}
        }
	}
	make_heap(valid.begin(), valid.end(), Comp());
	for (auto it : valid) {
		//cout << it.first << ", " << it.second << endl;
	}

	// If the original word exists, find its frequency
    double originalFrequency = -1.0;
	string copy = word;
	transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
	if (isRB && rbtree.search(copy)) {
		originalFrequency = rbtree.search(copy)->frequency;
	} else if (isHash && hashtable.find(copy)) {
		originalFrequency = hashtable.find(copy)->frequency;
	}
	//cout << "orig freq for " << word << " = " << originalFrequency << endl;

	// Output based on which valid permutation has highest frequency
	if (!valid.empty()) {
		//cout << valid.size() << ": ";
		//cout << "->";

		for (int i = 0; i < count; i++) {
			if (valid.size() > 0) {
				pair<string, double> top = valid.at(0);
				pop_heap(valid.begin(), valid.end());
				valid.pop_back();
				
				if (i == 0 && (originalFrequency / top.second < 0.01 || originalFrequency == -1.0)) {
					out.push_back(top.first);
					//cout << top.first << " " << top.second << " vs " << word << " " << originalFrequency << " go for max ";
				} else if (i == 0) {
					out.push_back(word);
					out.push_back(top.first);
					//cout << top.first << " " << top.second << " vs " << word << " " << originalFrequency << " go for orig ";
				} else {
					out.push_back(top.first);
					//cout << top.first << " " << top.second << ", ";
				}
				
			}
		}
		//cout << endl;
		/*
		// Uses a heap to find the valid permutation with the greatest frequency
		pop_heap(valid.begin(), valid.end());
		pair<string, double> top = valid.back();
		string print = top.first;
		double max = top.second;
		//cout << "print = " << print << ", max = " << max << ", original = " << word << endl;

		// If the premutation with max frequency is 100 times more common than the original word, replace it
		// Alternatively, if the word does not exist, also replace
		// Otherwise just print the original word
		if (abs(originalFrequency / max) < 0.01 || originalFrequency == -1.0) {
			out = print;
		} else {
			out = word;
		}
		*/
	} else {
		out.push_back(word);
	}

	//cout << endl;
	
	return out;
}
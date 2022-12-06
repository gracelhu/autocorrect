#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
//#include "../graceAVL/AVLTree.cpp"
#include "../hashtable/HashTable.cpp"
#include "../redblack/RedBlack.cpp"
#include <stdlib.h>
#include <queue>
#include <regex>
using namespace std;

class Autocorrector {
    private:
    bool isRB;
    bool isHash;
    redBlack rbtree;
    HashTable hashtable;
    
    public:
    Autocorrector();
    void buildRBtree();
    void buildHashTable();
    bool search(string word);
	void checkTypos();
	string permute(string str);
    set<string> scramble(string str);
	vector<string> split(string str, char delimiter);
	vector<string> split(string str, string delimiters);
	vector<string> fixtypo(string word);
	vector<string> fixtypo(string word, int count);
}; 

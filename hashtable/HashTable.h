#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Word {
    string word;
    double frequency;
    int hashCode;
    int index;
    Word* right;
    Word(string word, double frequency);
};

/* 
Collision Resolution Strategy: seperate chaining 
For hashing using separate chaining method, the load factor should be maintained as 1. For open addressing method, it should not exceed 0.5.
*/
class HashTable {
    private:

    public:
    void insertWord(string word, double frequency);
    void rehash();
    Word* find(string word);
    void deleteAllWords();
    float loadFactor = 0.0;
    float maxLoadFactor = 0.8;
    float numElements = 0.0;
    unsigned int tableSize = 5;
    vector<Word*> table{5};
};

int calculateHashCode(string word);

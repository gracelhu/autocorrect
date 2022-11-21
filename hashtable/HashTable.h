#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Word {
    string word;
    int frequency;
    int hashCode;
    int index;
    Word* right;
    Word(string word, int frequency);
};

//Collision Resolution Strategy: seperate chaining 
class HashTable {
    private:

    public:
    void insertWord(string word, int frequency);
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

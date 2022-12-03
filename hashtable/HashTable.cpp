#include "HashTable.h"

Word::Word(string word, int frequency)
{
    this->word = word;
    this->frequency = frequency;
    this->hashCode = calculateHashCode(word);
	this->right = nullptr;
	this->index = 0;

}

int calculateHashCode(string word)
{
	unsigned long hash = 5381;
	int c;
	for (auto& ch : word) {
		hash = ((hash << 5) + hash) + ch;
	}
	return (int)hash;
}

void HashTable::insertWord(string word, int frequency)
{
	//add word into the table 
	Word* newWord = new Word(word, frequency);
	newWord->index = newWord->hashCode % tableSize;

	//if there already exists an element at table.at(newWord->index), then iterate through the linked list and attach newWord to the end 
	if(table.at(newWord->index) == nullptr)
	{
		table.at(newWord->index) = newWord;
	}
	else
	{
		Word* curr = table.at(newWord->index);
		while(curr->right != nullptr)
			curr = curr->right;
		
		curr->right = newWord;
	}

	//update loadFactor and numElements;
	numElements++;
	loadFactor = numElements / tableSize;

	if(loadFactor >= maxLoadFactor)
	{
		//call rehash function 
		rehash();
	}
}

void HashTable::rehash()
{
	//need to double the size of the array, and put all the elements in new positions 
	tableSize = tableSize * 2;
	loadFactor = numElements / tableSize;
	vector<Word*> newTable{tableSize};

	//iterate through all the elements in the old table 
	//"curr" represents the iterator in the old table, "iter" represents the iterator in the new table --> 
	for(int x = 0; x < table.size(); x++)
	{
		if(table.at(x) != nullptr)
		{
			Word* oldTableIter = table.at(x);	//iterator in old table 
			while(oldTableIter != nullptr)
			{
				//two things can change about an object when I rehash: its index and its right pointer 
				//right pointer always going to start as nullptr because we add words into the new vector one by one 
				int newIndex = oldTableIter->hashCode % tableSize;
				Word* newWord = new Word(oldTableIter->word, oldTableIter->frequency);
				newWord->hashCode = oldTableIter->hashCode;
				newWord->index = newIndex;
				
				//if there already exists an element at newTable.at(newIndex), then iterate through the linked list and attach curr to the end 
				if(newTable.at(newIndex) == nullptr)
				{
					newTable.at(newIndex) = newWord;
				}
				else
				{
					Word* newTableIter  = newTable.at(newIndex);
					while(newTableIter->right != nullptr)
					{
						newTableIter = newTableIter->right;
					}
		
					newTableIter->right = newWord;
				}
				oldTableIter = oldTableIter->right;
			}
		}
	}

	//free up all dynamic memory from old table 
	deleteAllWords();
	
	//set the old table to newTable 
	table = newTable;
}

Word* HashTable::find(string word)
{
	int index = calculateHashCode(word) % tableSize;
	if(table.at(index) == nullptr)
		return nullptr;

	Word* curr = table.at(index);
	while(curr != nullptr)
	{
		if(curr->word == word)
			return curr;
		curr = curr->right;
	}

	return nullptr;
}

void HashTable::deleteAllWords()
{
	//iterate through all the elements in the table 
	for(int x = 0; x < table.size(); x++)
	{
		if(table.at(x) != nullptr)
		{
			//table.at(x) represents the head at an index 
			Word* temp;
			while(table.at(x) != nullptr)
			{
				temp = table.at(x);
				table.at(x) = table.at(x)->right;
				delete temp;
			}

		}
	}
}
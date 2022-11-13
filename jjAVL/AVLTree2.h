#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include "AVLTree2.cpp"
using namespace std;

class AVLTree2 {
private:
	struct Node2 {
		int ID;
		string name;
		double freq;
		Node2* left;
		Node2* right;
		int height;
		Node2(int x, string y, double z) {
			ID = x;
			name = y;
			left = nullptr;
			right = nullptr;
			height = 0;
			freq = z;
		}
	};
	Node2* root;
	vector<int> opcount;

	// rotation functions
	Node2* rotateLeft(Node2* Node2);
	Node2* rotateRight(Node2* Node2);
	Node2* rotateLeftRight(Node2* Node2);
	Node2* rotateRightLeft(Node2* Node2);
	int updateHeight(Node2* Node2, int ID);
	int getbalance(Node2* Node2);
	Node2* rotate(Node2* Node2);
	
	// private recursive versions of required functions
	Node2* insert(Node2* Node2, string name, int ID, double freq);
	Node2* search(Node2* Node2, int ID);
	Node2* search(Node2* Node2, string name);

	// private ver of print functions
	void printList(Node2* curr, int depth);
	vector<string> printTree(Node2* curr, int depth, vector<string> vec);

	// destruct
	void destruct(Node2* Node2);

public:
	AVLTree2() { root = nullptr; opcount = {0, 0, 0, 0, 0}; }
	~AVLTree2() { destruct(root); }

	// required functions
	Node2* insert(string _name, int _ID, double _freq);
	Node2* search(int _ID);
	Node2* search(string _name);
	double freq(Node2* Node2);

	// bonus print functions
	void printList();
	void printTree();
};

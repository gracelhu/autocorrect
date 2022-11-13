#include "AVLTree2.h"

/*
* 
* Extra print functions for self help during assignment
* 
*/

void AVLTree2::printList() { printList(root, 0); }
void AVLTree2::printList(Node2* node, int depth) {
	if (node == nullptr) {
		exit;
	} else {
		cout << node->ID << ", " << node->name << " depth = " << depth << " ";
		if (node->left != nullptr) {
			cout << "left child = " << node->left->name << " ";
		}
		if (node->right != nullptr) {
			cout << "right child = " << node->right->name;
		}
		cout << endl;
		printList(node->left, depth + 1);
		printList(node->right, depth + 1);
	}
}

vector<string> AVLTree2::printTree(Node2* node, int depth, vector<string> vec) {
	if (node == nullptr) {
		return vec;
	} else {
		if (vec.size() <= depth) {
			vec.push_back("");
		}
		vec.at(depth) = vec.at(depth) + node->name + " ";
		vec = printTree(node->left, depth + 1, vec);
		vec = printTree(node->right, depth + 1, vec);
		return vec;
	}
}

void AVLTree2::printTree() {
	vector<string> print = {};
	print = printTree(root, 0, print);
	for (int i = 0; i < print.size(); i++) {
		string temp = print.at(i);
		if (temp.length() > 400) {
			temp = temp.substr(0, 200) + " ... " + temp.substr(temp.length() - 200, temp.length());
		}
		cout << i << " = " << temp << endl;
	}
}
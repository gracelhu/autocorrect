#include "AVLTree2.h"

/*
*
* Insert, remove
* Rotation functions are defined in AVLTree2Rotations.cpp
*
*/

AVLTree2::Node2* AVLTree2::insert(string name, int ID, double freq) {
	// Validates for name and duplicate IDs
	// ID length is validated in main due to ID being converted to and passed as an int
	// No way to differentiate between "8" and "00000008" with an int
	bool invalid = false;
	regex name_regex("[A-Za-z\\s]+");
	//invalid |= !regex_match(name, name_regex);
	invalid |= search(root, ID) != nullptr;
	if (invalid) {
		cout << "unsuccessful" << endl;
		return nullptr;
	}
	if (root == nullptr) { // If root empty, insert as root
		root = new Node2(ID, name, freq);
		//cout << "successful" << endl;
		return root;
	}
	Node2* temp = insert(root, name, ID, freq); // Else, insert under root
	updateHeight(root, ID); // Recursively updates height
	root = rotate(root); // Rotates root
	//cout << "successful" << endl;
	return temp;
}

AVLTree2::Node2* AVLTree2::insert(Node2* node, string name, int ID, double freq) {
	if (node == nullptr)
		return new Node2(ID, name, freq);
	if (node->ID < ID) {
		node->right = insert(node->right, name, ID, freq);
	} else {
		node->left = insert(node->left, name, ID, freq);
	}
	return node;
}

int AVLTree2::updateHeight(Node2* node, int ID) {
	if (node == nullptr) { return 0; }
	if (node->ID == ID) {
		node->height = 0;
		return 0;
	}
	if (node->ID < ID) {
		// Recursively update height from deepest leaf
		node->height = max(node->height, updateHeight(node->right, ID) + 1);
		// Rotate child Node2 if necessary
		node->right = rotate(node->right);
		// Update height if rotation successful
		node->height = node->right->height + 1;
	} else {
		node->height = max(node->height, updateHeight(node->left, ID) + 1);
		node->left = rotate(node->left);
		node->height = node->left->height + 1;
	}
	return node->height;
}

/*
*
* Search ID, Search NAME, Remove inorder, destructor
*
*/

AVLTree2::Node2* AVLTree2::search(int ID) {
	Node2* query = search(root, ID);
	//if (query) { cout << query->name; }
	return query;
}

AVLTree2::Node2* AVLTree2::search(string name) {
	Node2* query = search(root, name);
	//if (!query) { cout << "unsuccessful" << endl; }
	return query;
}

/*
void AVLTree2::removeInorder(int index) {
	int query = removeInorder(root, 0, index);
	if (query != INT32_MAX - 1) { cout << "unsuccessful" << endl; }
	return;
}
*/

AVLTree2::Node2* AVLTree2::search(Node2* node, int ID) {
	if (node == nullptr) { return nullptr; }
	if (node->ID == ID) {
		return node;
	} else if (node->ID < ID) {
		return search(node->right, ID);
	} else {
		return search(node->left, ID);
	}
}

AVLTree2::Node2* AVLTree2::search(Node2* node, string name) {
	if (node == nullptr) { return nullptr; }
	Node2* out = nullptr;
	Node2* temp = nullptr;
	if (node->name == name) {
		temp = node;
		if (temp != nullptr) { out = temp; }
	}
	if (temp != nullptr) { cout << temp->ID << endl; }
	temp = search(node->left, name);
	if (temp != nullptr) { out = temp; }
	temp = search(node->right, name);
	if (temp != nullptr) { out = temp; }
	return out;
}

void AVLTree2::destruct(Node2* node) {
	if (node == nullptr) { return; }
	if (node->left)
		destruct(node->left);
	if (node->right)
		destruct(node->right);
	delete node;
}

double AVLTree2::freq(Node2* node) {
	return node->freq;
}
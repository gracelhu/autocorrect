#include "AVLTree2.h"

int AVLTree2::getbalance(Node2* node) {
	int lHeight = node->left ? node->left->height + 1 : 0;
	int rHeight = node->right ? node->right->height + 1 : 0;
	return rHeight - lHeight;
}

AVLTree2::Node2* AVLTree2::rotate(Node2* node) {
	if (node == nullptr) { return nullptr; }
	int balance = getbalance(node);
	while (balance >= 2) { // Too many elements on right
		if (getbalance(node->right) < 0) { // RL case
			node = rotateRightLeft(node);
		} else { // L case
			node = rotateLeft(node);
		}
		balance = getbalance(node);
	} while (balance <= -2) { // Too many elements on left
		if (getbalance(node->left) > 0) { // LR case
			node = rotateLeftRight(node);
		} else { // R case
			node = rotateRight(node);
		}
		balance = getbalance(node);
	}
	return node;
}

// Performs a left rotation, returns new root
AVLTree2::Node2* AVLTree2::rotateLeft(Node2* node) {
	Node2* grandchild = node->right->left;
	Node2* child = node->right;
	node->right = grandchild;
	child->left = node;
	node->height = max(0, node->height-2);
	return child;
}

// Performs a right rotation, returns new root
AVLTree2::Node2* AVLTree2::rotateRight(Node2* node) {
	Node2* grandchild = node->left->right;
	Node2* child = node->left;
	node->left = grandchild;
	child->right = node;
	node->height = max(0, node->height - 2);
	return child;
}

// Performs a left-right rotation, returns new root
AVLTree2::Node2* AVLTree2::rotateLeftRight(Node2* node) {
	node->left = rotateLeft(node->left);
	node = rotateRight(node);
	return node;
}

// Performs a right-left rotation, returns new root
AVLTree2::Node2* AVLTree2::rotateRightLeft(Node2* node) {
	node->right = rotateRight(node->right);
	node = rotateLeft(node);
	return node;
}

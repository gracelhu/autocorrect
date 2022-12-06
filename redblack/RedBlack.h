//We should use graceAVL because it's faster --> this works just fine, just don't do individual deletes 
#include <iostream>
#include <string> 
#include <vector>
using namespace std;

class RBNode
{
public:
	string word;
	double frequency;
	int height;
	RBNode* left;
	RBNode* right;
	RBNode* parent;
	bool red;
	bool validNameAndID;
	RBNode(string NAME, double frequency);
};

class redBlack
{
private:
	RBNode* root;
	//My Helper Methods: helper methods are to help make methods easier to implement + read by using recursion 
	void inorderHelper(RBNode* head, vector<RBNode*>& nodes);
	void preorderHelper(RBNode* head, vector<RBNode*>& nodes);
	void postorderHelper(RBNode* head, vector<RBNode*>& nodes);
	RBNode* searchHelper(RBNode* node, string word);

	//Testing Method for debugging or unit tests 
	void printHeights(RBNode* node);
	void printParents(RBNode* node);

public:
	redBlack();
	redBlack(RBNode* root);
	void rotateLeft(RBNode* RBNode);
	void rotateRight(RBNode* RBNode);
	void rotateLeftRight(RBNode* RBNode);
	void rotateRightLeft(RBNode* RBNode);
	void insert(string NAME, double frequency);
	void printInorder();
	void printPreorder();
	void printPostorder();
	void printLevelCount();
	bool isRedBlack(RBNode* root);
	void check_1();
	void check_2();
	void check_3();
	void check_4();
	void fix_insert(RBNode* node);
	RBNode* search(string word);
};

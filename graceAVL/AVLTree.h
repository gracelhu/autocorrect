//We should use graceAVL because it's faster --> this works just fine, just don't do individual deletes 
#include <iostream>
#include <string> 
#include <vector>
using namespace std;

class Node
{
    public:
        string NAME;
        int ID;
        int height;
		double freq;
        Node* left;
        Node* right;
        Node* parent;
        bool validNameAndID;
        Node(string NAME, int ID, double freq);
};

class AVLTree
{
    private:
    Node* root;
    //My Helper Methods: helper methods are to help make methods easier to implement + read by using recursion 
    void removeIDHelper0(Node* current);
    void removeIDHelper1(Node* current);
    void removeIDHelper2(Node* current);
    void removeInorderHelper(Node* node, vector<Node*>& inorderNodes);
    void inorderHelper(Node* head, vector<Node*>& nodes);
    void preorderHelper(Node* head, vector<Node*>& nodes);
    void postorderHelper(Node* head, vector<Node*>& nodes);
    void destructHelper(Node* root);

    //Testing Method for debugging or unit tests 
    void printHeights(Node* node);
    void printParents(Node* node);

    public:
    AVLTree(Node* root);
    void updateHeightsMinus(Node* node);
    void rotateLeft(Node* node);
    void rotateRight(Node* node);
    void rotateLeftRight(Node* node);
    void rotateRightLeft(Node* node);
    void updateHeightsAdd(Node* node);
    void balanceTree(Node* newNodeParentParent);
    Node* deepestNodeThatBreaksAVL(Node* node);
    bool idExists(Node* node, int ID);
    bool checkValidNode(string NAME, string ID);
    void insert(string NAME, int ID, double freq);
    int height(Node* node);
    void adjustAllNodeHeights(Node* node);
    void remove(int ID);
    Node* search(int ID);
    Node* searchHelper(Node* node, int ID);
    Node* search(string NAME);
    void printInorder();
    void printPreorder();
    void printPostorder();
    void printLevelCount();
    void removeInorder(int N);
    void destruct();   
    bool isAVL(Node* root);
    
    //just for unit tests 
    vector<int> inorder();
    vector<int> preorder();
}; 


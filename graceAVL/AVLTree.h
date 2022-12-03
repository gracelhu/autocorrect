//We should use graceAVL because it's faster --> this works just fine, just don't do individual deletes 
#include <iostream>
#include <string> 
#include <vector>
using namespace std;

class Node
{
    public:
        string NAME;
        double freq;
        int height;
        Node* left;
        Node* right;
        Node* parent;
        bool validNameAndID;
        Node(string NAME, double freq);
};

class AVLTree
{
    private:
    Node* root;
    //My Helper Methods: helper methods are to help make methods easier to implement + read by using recursion 
    void destructHelper(Node* root);

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
    void insert(string NAME, double freq);
    int height(Node* node);
    void adjustAllNodeHeights(Node* node);
    Node* searchHelper(Node* node, string NAME);
    Node* search(string NAME);
    void destruct();   
    bool isAVL(Node* root);
    void printLevelCount();
    void printInorderWords();
    void inorderHelperWords(Node* head, vector<Node*>& nodes);
    void printInorderFreqs();
    void inorderHelperFreqs(Node* head, vector<Node*>& nodes);
}; 


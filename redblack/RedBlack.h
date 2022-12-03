#include <iostream>
#include <string> 
#include <vector>
using namespace std;

class Node
{
public:
    string word;
    double freq;
    int height;
    Node* left;
    Node* right;
    Node* parent;
    bool red;
    bool validNameAndID;
    Node(string NAME, double freq);
};

class redBlack
{
private:
    Node* root;
    //My Helper Methods: helper methods are to help make methods easier to implement + read by using recursion 
    void inorderHelper(Node* head, vector<Node*>& nodes);
    void preorderHelper(Node* head, vector<Node*>& nodes);
    void postorderHelper(Node* head, vector<Node*>& nodes);
    Node* searchHelper(Node* root, string word);
    void destructHelper(Node* root);

    //Testing Method for debugging or unit tests 
    void printHeights(Node* node);
    void printParents(Node* node);

public:
    redBlack(Node* root);
    void rotateLeft(Node* node);
    void rotateRight(Node* node);
    void rotateLeftRight(Node* node);
    void rotateRightLeft(Node* node);
    void insert(string word, double freq);
    void printInorder();
    void printPreorder();
    void printPostorder();
    void printLevelCount();
    bool isRedBlack(Node* root);
    void check_1();
    void check_2();
    void check_3();
    void check_4();
    void fix_insert(Node* node);
    Node* search(string word);
    void destruct();
};

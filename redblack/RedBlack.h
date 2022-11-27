//We should use graceAVL because it's faster --> this works just fine, just don't do individual deletes 
#include <iostream>
#include <string> 
#include <vector>
using namespace std;

class Node
{
public:
    string word;
    int frequency;
    int height;
    Node* left;
    Node* right;
    Node* parent;
    bool red;
    bool validNameAndID;
    Node(string NAME, int frequency);
};

class redBlack
{
private:
    Node* root;
    //My Helper Methods: helper methods are to help make methods easier to implement + read by using recursion 
    void inorderHelper(Node* head, vector<Node*>& nodes);
    void preorderHelper(Node* head, vector<Node*>& nodes);
    void postorderHelper(Node* head, vector<Node*>& nodes);

    //Testing Method for debugging or unit tests 
    void printHeights(Node* node);
    void printParents(Node* node);

public:
    redBlack(Node* root);
    void rotateLeft(Node* node);
    void rotateRight(Node* node);
    void rotateLeftRight(Node* node);
    void rotateRightLeft(Node* node);
    void insert(string NAME, int frequency);
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
};

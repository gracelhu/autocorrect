#include "redBlack.h"
#include <queue>
#include <stack>
#include <math.h> 
#include <cctype> 
#include <sstream>

using namespace std;
Node::Node(string word, int frequency)
{
    this->word = word;
    this->frequency = frequency;
    this->height = 1;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    this->red = false;
    //is red
}

redBlack::redBlack(Node* root)
{
    this->root = root;
}

void redBlack::check_1() {

}
void redBlack::check_2() {

}
void redBlack::check_3() {

}
void redBlack::check_4() {

}
bool redBlack::isRedBlack(Node* root)
{
    if (root == nullptr)
        return true;

    if (root->red == true) {
        return false;
    }

    //check red-black conditions
    //root and leaves are black
    
    //if node is red, children are black

    //all paths from node to its null descendants have same number of black nodes.

    return false;
}

//Printing the parents of all the nodes inorder 
void redBlack::printParents(Node* node)
{
    if (node == NULL)
        return;
    printParents(node->left);
    if (node->parent != NULL)
        std::cout << node->parent->word << " ";
    else
        std::cout << 0 << " ";
    printParents(node->right);
}

void redBlack::rotateLeft(Node* node)
{
    Node* rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != NULL)
        rightChild->left->parent = node;
    rightChild->left = node;
    if (node->parent != NULL)
    {
        if (node->parent->word > node->word)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;

        rightChild->parent = node->parent;
        node->parent = rightChild;
    }
    else
    {
        rightChild->parent = NULL;
        node->parent = rightChild;
        root = rightChild;
    }
}

void redBlack::rotateRight(Node* node)
{
    Node* leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != NULL)
        leftChild->right->parent = node;
    leftChild->right = node;
    if (node->parent != NULL)
    {
        if (node->parent->word > node->word)
            node->parent->left = leftChild;
        else
            node->parent->right = leftChild;
        leftChild->parent = node->parent;
        node->parent = leftChild;
    }
    else
    {
        leftChild->parent = NULL;
        node->parent = leftChild;
        root = leftChild;
    }

}

void redBlack::rotateLeftRight(Node* node)
{
    rotateLeft(node->left);
    rotateRight(node);
}

void redBlack::rotateRightLeft(Node* node)
{
    rotateRight(node->right);
    rotateLeft(node);
}


void redBlack::insert(string word, int frequency)
{


    if (root == NULL)
    {
        root = new Node(word, frequency);
        root->red = false;
        return;
    }

    Node* parent = NULL;
    Node* current = root;
    string curWord = word;
    while (current != NULL)
    {
        parent = current;
        if (current->word < curWord)
            current = current->right;
        else if (current->word > curWord)
            current = current->left;
        else if (current->word == curWord)
        {
            return;
        }
    }
    
    Node* newNode = new Node(word,frequency);
    newNode->red = true;
    if (parent->word < word) {
        parent->right = newNode;
        //std::cout << "inserting to the right of " << parent->word << ", ";
        //std::cout << parent->right->word << "<- word here ";
    }
    else {
        parent->left = newNode;
        //std::cout << "inserting to the left of " << parent->word << ", ";
        //std::cout << parent->left->word << "<- word here ";
    }
    newNode->parent = parent;
    
    fix_insert(newNode);
}

void redBlack::fix_insert(Node* node) {
    while (node->parent && node->parent->red == true) {
        if (node->parent->parent!=nullptr && node->parent == node->parent->parent->left) {
                Node* u = node->parent->parent->right;
                //if right uncle is red
                if (u!=nullptr&&u->red == true) {
                    node->parent->red = false;
                    u->red = false;
                    node->parent->parent->red = true;
                    node = node->parent->parent;
                    //cout << "1";
                }
                else {
                    if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(node);
                    }
                    node->parent->red = false;
                    node->parent->parent->red = true;
                    rotateRight(node->parent->parent);
                    //cout << "2";
                }
            }
        else {
            if (node->parent->parent != nullptr) {
                Node* u = node->parent->parent->left;
                //if left uncle is red
                if (u!=nullptr && u->red == true) {
                    node->parent->red = false;
                    u->red = false;
                    node->parent->parent->red = true;
                    node = node->parent->parent;
                    if (node == root) {
                        node->red = false;
                    }
                    //cout << "3";
                }
                else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    }
                    node->parent->red = false;
                    node->parent->parent->red = true;
                    rotateLeft(node->parent->parent);
                    //cout << "4";
                }
            }
        }
        if (node == root) {
            //cout << "hi";
            node->red = false;
            break;
        }
    }

}

void redBlack::printInorder()
{
    vector<Node*> nodes;
    inorderHelper(root, nodes);
    for (int x = 0; x < nodes.size(); x++)
    {
        if (x != nodes.size() - 1)
            if (nodes.at(x)->red == true) {
                std::cout << nodes.at(x)->word << " (red)" << ", ";
            }
            else
                std::cout << nodes.at(x)->word << " (black)" << ", ";
        else
            if (nodes.at(x)->red == true) {
                std::cout << nodes.at(x)->word << " (red)" << endl;
            }
            else
                std::cout << nodes.at(x)->word << " (black)" << endl;
    }
}

/*vector<int> redBlack::inorder()
{
    vector<Node*> nodes;
    inorderHelper(root, nodes);
    vector<int> nodeIDs;
    for (int x = 0; x < nodes.size(); x++)
        nodeIDs.push_back(nodes.at(x)->ID);
    return nodeIDs;
}*/

void redBlack::printPreorder()
{
    vector<Node*> nodes;
    preorderHelper(root, nodes);
    for (int x = 0; x < nodes.size(); x++)
    {
        if (x != nodes.size() - 1)
            std::cout << nodes.at(x)->word << ", ";
        else
            std::cout << nodes.at(x)->word << endl;
    }
}

/*vector<int> redBlack::preorder()
{
    vector<Node*> nodes;
    preorderHelper(root, nodes);
    vector<int> nodeIDs;
    for (int x = 0; x < nodes.size(); x++)
        nodeIDs.push_back(nodes.at(x)->ID);
    return nodeIDs;
}*/

void redBlack::printPostorder()
{
    vector<Node*> nodes;
    postorderHelper(root, nodes);
    for (int x = 0; x < nodes.size(); x++)
    {
        if (x != nodes.size() - 1)
            std::cout << nodes.at(x)->word << ", ";
        else
            std::cout << nodes.at(x)->word << endl;
    }
}

void redBlack::inorderHelper(Node* head, vector<Node*>& nodes)
{
    if (head == NULL)
        return;
    else
    {
        inorderHelper(head->left, nodes);
        nodes.push_back(head);
        inorderHelper(head->right, nodes);
    }
}

void redBlack::preorderHelper(Node* head, vector<Node*>& nodes)
{
    if (head == NULL)
        return;
    else
    {
        nodes.push_back(head);
        preorderHelper(head->left, nodes);
        preorderHelper(head->right, nodes);
    }
}

void redBlack::postorderHelper(Node* head, vector<Node*>& nodes)
{
    if (head == NULL)
        return;
    else
    {
        postorderHelper(head->left, nodes);
        postorderHelper(head->right, nodes);
        nodes.push_back(head);
    }
}

void redBlack::printLevelCount()
{
    if (this->root == nullptr)
    {
        std::cout << 0 << endl;
        return;
    }

    int numLevels = 0;
    queue<Node*> queue;
    queue.push(this->root);
    while (!queue.empty())
    {
        int size = queue.size();
        for (int x = 0; x < size; x++)
        {
            if (queue.front()->left != NULL)
                queue.push(queue.front()->left);
            if (queue.front()->right != NULL)
                queue.push(queue.front()->right);
            queue.pop();
        }
        numLevels++;
    }
    std::cout << numLevels << endl;
}
int main() {
    //redBlack* tree = new redBlack();
    Node* root = new Node("b", 1000);
    redBlack tree(root);

    vector<string> Order;
    vector<int> IDList;
    vector<int> HeightList;
    vector<int> idList;
    string line;
    //getline(cin, line);
    //int count = stoi(line);
    string firstCom;
    string secondCom;
    int firstIndex = 0;
    int secondIndex = 0;

    tree.insert("a", 5);
    tree.printInorder();
    tree.insert("d", 5);
    tree.printInorder();
    tree.insert("e", 5);
    tree.printInorder();
    tree.insert("i", 5);
    tree.printInorder();
    tree.insert("c", 5);
    tree.printInorder();
    tree.insert("f", 5);
    tree.printInorder();
    tree.insert("g", 5);
    tree.printInorder();
    /*for (int i = 0; i < count; i++) {
        getline(cin, line);
        //std::cout << line.substr(0, 6) << endl;
        //std::cout << line;
        //std::cout << line << endl;
        if (line.substr(0, 6) == "insert") {
            //std::cout << "hi" << endl;
            firstIndex = 1 + line.find('"');
            firstCom = line.erase(0, firstIndex);
            secondIndex = firstCom.find('"');
            firstCom = line.substr(0, secondIndex);
            //line = NAME at this point in time
            //if line!=letters, return unsuccessful
            //std::cout << "name is " << line << endl;
            secondCom = line.substr(secondIndex + 2, line.size());
            int frequency = stoi(secondCom);
            //std::cout << "id " << id << endl;
            //std::cout << "name " << firstCom << endl;

            //if line!=8 numbers, return unsuccessful
            //line = ID at this point in time, but is still in string format
            //std::cout << "id is " << line;

            //ONLY INSERT IF: name is string enclosed in quotes, ID is 8 digit integer

                tree.insert(firstCom, frequency);
                tree.printInorder();
        }
        //currently will not read this since there is no space after it.
        else if (line == "printInorder") {
            Order.clear();
            tree.printInorder();
        }
        else if (line == "printPreorder") {
            Order.clear();
            tree.printPreorder();
        }
        else if (line == "printPostorder") {
            Order.clear();
            tree.printPostorder();
        }
    }

    */

}

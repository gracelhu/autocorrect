#include "AVLTree.h"
#include <queue>
#include <stack>
#include <math.h> 
#include <cctype> 
#include <sstream>

Node::Node(string NAME, double freq)
{
    this->NAME = NAME;
    this->freq = freq;
    this->height = 1;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
}

AVLTree::AVLTree(Node* root)
{
    this->root = root;
}

bool AVLTree::isAVL(Node* root)
{
    if(root == nullptr)
        return true;
    
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    
    if(abs(leftHeight - rightHeight) <= 1 && isAVL(root->left) && isAVL(root->right))
        return true;
    
    return false;
}

void AVLTree::updateHeightsMinus(Node* node)
{
    //any rotation will cause all parents of the node we're rotating about to decrease one in height 
    Node* nodeParent = node->parent;
    while(nodeParent != NULL)
    {
        nodeParent->height--;
        nodeParent = nodeParent->parent;
    }
}

void AVLTree::rotateLeft(Node* node)
{
    Node* rightChild = node->right; 
    node->right = rightChild->left; 
    if(rightChild->left != NULL)
        rightChild->left->parent = node;
    rightChild->left = node; 
    if(node->parent != NULL)
    {
        if(node->parent->NAME > node->NAME)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;
        
        rightChild->parent = node->parent;
        node->parent = rightChild;
    } 
    else
    {
        //This means the node was a root
        rightChild->parent = NULL;
        node->parent = rightChild;
        root = rightChild;
    }
}

void AVLTree::rotateRight(Node* node)
{
    Node* leftChild = node->left; 
    node->left = leftChild->right; 
    if(leftChild->right != NULL)
        leftChild->right->parent = node;
    leftChild->right = node; 
    if(node->parent != NULL)
    {
        if(node->parent->NAME > node->NAME)
            node->parent->left = leftChild;
        else
            node->parent->right = leftChild;
        leftChild->parent = node->parent;
        node->parent = leftChild;
    } 
    else
    {
        //This means the node was a root
        leftChild->parent = NULL;
        node->parent = leftChild;
        root = leftChild;
    }
}

void AVLTree::rotateLeftRight(Node* node)
{
    rotateLeft(node->left); 
    rotateRight(node);  
}

void AVLTree::rotateRightLeft(Node* node)
{
    rotateRight(node->right); 
    rotateLeft(node); 
}

void AVLTree::updateHeightsAdd(Node* node)
{
    //any insertion will cause all parents of the node we're inserting to increase by one in height 
    Node* nodeParent = node->parent;
    while(nodeParent != NULL)
    {
        nodeParent->height++;
        nodeParent = nodeParent->parent;
    }
}

void AVLTree::balanceTree(Node* newNodeParentParent)
{
    Node* node = deepestNodeThatBreaksAVL(newNodeParentParent);
    //if there is no node that breaks that AVL rule, then just return. We don't need to rotate and rebalance the tree
    if(node == NULL)
        return;
   
    int leftHeight = 0;
    int rightHeight = 0;
    if(node->left != nullptr)
        leftHeight = node->left->height;
    if(node->right != nullptr)
        rightHeight = node->right->height;

    //means its left heavy: so rotate right, or do left right rotate 
    if(leftHeight - rightHeight >= 2 && node->left != nullptr)
    {
        if(node->left->left != nullptr)
        {
            //update heights
            updateHeightsMinus(node);
            node->height -=2;
            rotateRight(node);
        }
        else if (node->left->right != nullptr)
        {
            //update heights
            updateHeightsMinus(node);
            node->height -=2;
            node->left->height -=1;
            node->left->right->height +=1;
            rotateLeftRight(node);  
        }
    }
    //means its right heavy: so rotate left, or do right left rotate 
    if(leftHeight - rightHeight <= -2 && node->right != nullptr)
    {
        if(node->right->right != nullptr)
        {
            //update heights
            updateHeightsMinus(node);
            node->height -=2;
            rotateLeft(node);
        }
        else if(node->right->left != nullptr)
        {
            //update heights
            updateHeightsMinus(node);
            node->height -=2;
            node->right->height -=1;
            node->right->left->height +=1;
            rotateRightLeft(node);
        }
    }
}

Node* AVLTree::deepestNodeThatBreaksAVL(Node* node)
{
    if(node == NULL)
        return NULL;

    int leftHeight = 0;
    int rightHeight = 0;
    if(node->left != nullptr)
        leftHeight = node->left->height;
    if(node->right != nullptr)
        rightHeight = node->right->height;

    if(abs(leftHeight - rightHeight) >= 2)
        return node;
    else
        deepestNodeThatBreaksAVL(node->parent);   
}

bool AVLTree::checkValidNode(string NAME, string ID)
{
    bool validName = true;
    bool validID = true;
    for(int x = 0; x < NAME.length(); x++)
    {
        if(!(isalpha(NAME.at(x)) | NAME.at(x) == ' ')) {
            validName = false;
            break; }
    }
    for(int x = 0; x < ID.length(); x++)
    {
        if(!isdigit(ID.at(x)))
        {
            validID = false;
            break;
        }
    }
	return true;
}

void AVLTree::insert(string NAME, double freq)
{
    if(root == NULL) 
    {
		root = new Node(NAME, freq);
        return;
    }
    
    Node* parent = NULL;
    Node* current = root;
    while(current != NULL)
    {
        parent = current;
        if(current->NAME < NAME)
            current = current->right;
        else if(current->NAME > NAME)
            current = current->left;
        else if(current->NAME == NAME)
        {
            return;
        }
    }

    Node* newNode = new Node(NAME, freq);
    if(parent->NAME < NAME)
        parent->right = newNode;
    else
        parent->left = newNode;
    
    newNode->parent = parent;
    updateHeightsAdd(newNode);

    //if node->parent->parent != NULL, then it's possible then tree needs to be balanced
    //if node->parent->parent == NULL, there's no way there could be an imbalance 
    //This way, insert function actually becomes log(n)!
    if(newNode->parent->parent != NULL)
        balanceTree(newNode->parent->parent);
}

void AVLTree::adjustAllNodeHeights(Node* node)
{
    if(node == NULL)
        return;
    adjustAllNodeHeights(node->left);
    node->height = height(node);
    adjustAllNodeHeights(node->right);
}

int AVLTree::height(Node* node)
{
    if(node == nullptr)
        return 0;
    return 1 + max(height(node->left), height(node->right));
}

Node* AVLTree::search(string NAME)
{
    searchHelper(root, NAME);
}

Node* AVLTree::searchHelper(Node* node, string NAME)
{
    if(node == nullptr)
    {
        return nullptr;
    }
    else if(node->NAME == NAME)
    {
        return node;
    }
    else if(NAME < node->NAME)
    {
        return searchHelper(node->left, NAME);
    }
    else if(NAME > node->NAME)
    {
        return searchHelper(node->right, NAME);
    }
}

void AVLTree::destruct()
{
    if(root != NULL)
        destructHelper(root);
}

void AVLTree::destructHelper(Node* root) 
{
    if (root->left) 
        destructHelper(root->left);
    if (root->right) 
        destructHelper(root->right);
    delete root;
}

void AVLTree::printLevelCount()
{
    if(this->root == nullptr)
    {
        cout << 0 << endl;
        return;
    }
    
    int numLevels = 0;
    queue<Node*> queue;
    queue.push(this->root);
    while(!queue.empty())
    {
        int size = queue.size();
        for(int x = 0; x < size; x++)
        {
            if(queue.front()->left != NULL)
                queue.push(queue.front()->left);
            if(queue.front()->right != NULL)
                queue.push(queue.front()->right);
            queue.pop();
        }
        numLevels++;
    }
    cout << numLevels << endl;
}


void AVLTree::printInorderWords()
{
   vector<Node*> nodes;
   inorderHelperWords(root, nodes);
   for(int x = 0; x < nodes.size(); x++)
   {
        if(x != nodes.size() - 1)
            cout << nodes.at(x)->NAME << ", ";
        else
            cout << nodes.at(x)->NAME << endl;
   }
}

void AVLTree::inorderHelperWords(Node* head, vector<Node*>& nodes)
{
    if(head == NULL)
        return;
    else
    {
        inorderHelperWords(head->left, nodes);
        nodes.push_back(head);
        inorderHelperWords(head->right, nodes);
    }  
}

void AVLTree::printInorderFreqs()
{
   vector<Node*> nodes;
   inorderHelperFreqs(root, nodes);
   for(int x = 0; x < nodes.size(); x++)
   {
        if(x != nodes.size() - 1)
            cout << nodes.at(x)->freq << ", ";
        else
            cout << nodes.at(x)->freq << endl;
   }
}

void AVLTree::inorderHelperFreqs(Node* head, vector<Node*>& nodes)
{
    if(head == NULL)
        return;
    else
    {
        inorderHelperFreqs(head->left, nodes);
        nodes.push_back(head);
        inorderHelperFreqs(head->right, nodes);
    }  
}
#include "AVLTree.h"
#include <queue>
#include <stack>
#include <math.h> 
#include <cctype> 
#include <sstream>

Node::Node(string NAME, int ID, double freq)
{
    this->NAME = NAME;
    this->ID = ID;
    this->height = 1;
	this->freq = freq;
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

void AVLTree::printHeights(Node* node)
{
    if(node == NULL)
        return;
    printHeights(node->left);
    cout << node->height << " " << endl;
    printHeights(node->right);
}

//Printing the parents of all the nodes inorder 
void AVLTree::printParents(Node* node)
{
    if(node == NULL)
        return;
    printParents(node->left);
    if(node->parent != NULL)
        cout << node->parent->ID << " ";
    else
        cout << 0 << " ";
    printParents(node->right);
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
        if(node->parent->ID > node->ID)
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
        if(node->parent->ID > node->ID)
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

	/*
    if(ID.length() == 8 && validName && validID)
        return true;
    else
        return false; 
	*/
}

void AVLTree::insert(string NAME, int ID, double freq)
{
    if(root == NULL) 
    {
		root = new Node(NAME, ID, freq);
        return;
    }
    
    Node* parent = NULL;
    Node* current = root;
	int IDnum = ID;
    while(current != NULL)
    {
        parent = current;
        if(current->ID < IDnum)
            current = current->right;
        else if(current->ID > IDnum)
            current = current->left;
        else if(current->ID == IDnum)
        {
            return;
        }
    }

    Node* newNode = new Node(NAME, ID, freq);
    if(parent->ID < IDnum)
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


void AVLTree::remove(int ID)
{
    if(root == NULL) 
    {
        cout << "unsuccessful" << endl; 
        return; 
    }

    Node* current = root;
    Node* parent = NULL;
    while(current->ID != ID)
    {
        parent = current;
        if(current->ID < ID)
            current = current->right;
        else if(current->ID > ID)
            current = current->left;
        if(current == NULL)
        {
            cout << "unsuccessful" << endl; 
            return;
        }
    }

    current->parent = parent;
    //0 children case 
    if(current->left == NULL && current->right == NULL)
    {
        removeIDHelper0(current);
    }

    //1 child case 
    if(current->left != NULL && current->right == NULL || current->left == NULL && current->right != NULL)
    {
        removeIDHelper1(current);
    }

    //2 children case 
    if(current->left != NULL && current->right != NULL)
    {
        removeIDHelper2(current);
    }
    
    cout << "successful" << endl;

    adjustAllNodeHeights(root);
}

void AVLTree::removeIDHelper0(Node* current)
{
    if(current->parent != NULL)
    {
        if(current->ID < current->parent->ID)
            current->parent->left = NULL;
        else
            current->parent->right = NULL;
    }
    else
    {
        //trying to delete root then, height adjustments here 
        root = NULL;
    }
    delete current; 
}

void AVLTree::removeIDHelper1(Node* current)
{
    Node* onlyChild;
    if(current->left != NULL)
        onlyChild = current->left;
    else
        onlyChild = current->right;
    
    if(current->parent != NULL)
    {
        if(current->ID < current->parent->ID)
            current->parent->left = onlyChild;
        else
            current->parent->right = onlyChild;
        onlyChild->parent = current->parent;
    }
    else
    {
        //means the node we want to delete is the root 
        root = onlyChild;
        onlyChild->parent = NULL;
    }
    delete current;   
}

void AVLTree::removeIDHelper2(Node* current)
{
    Node* inorderSuccessor = current->right;
    Node* inorderSuccessorParent = current;

    //go as far left as possible
    while(inorderSuccessor->left != NULL)
    {
        inorderSuccessorParent = inorderSuccessor;
        inorderSuccessor = inorderSuccessor->left;
    }
    //Copy contents from inorderSuccessor to the node we want to delete 
    current->ID = inorderSuccessor->ID;
    current->NAME = inorderSuccessor->NAME;

    if(inorderSuccessor->right != NULL)
    {
        if(inorderSuccessorParent->ID > inorderSuccessor->right->ID)
            inorderSuccessorParent->left = inorderSuccessor->right;
        else
            inorderSuccessorParent->right = inorderSuccessor->right;
    }
    else
    {
        if(inorderSuccessorParent->ID > inorderSuccessor->ID)
            inorderSuccessorParent->left = NULL;
        else
            inorderSuccessorParent->right = NULL;
    }
    delete inorderSuccessor;
}

//make this faster
Node* AVLTree::search(int ID)
{
    searchHelper(root, ID);
}

Node* AVLTree::searchHelper(Node* node, int ID)
{
    if(node == nullptr)
    {
        return nullptr;
    }
    else if(node->ID == ID)
    {
        return node;
    }
    else if(ID < node->ID)
    {
        return searchHelper(node->left, ID);
    }
    else if(ID > node->ID)
    {
        return searchHelper(node->right, ID);
    }
}

Node* AVLTree::search(string NAME)
{
    bool nameFound = false;
    if(root == nullptr)
    {
      // cout << "unsuccessful" << endl; 
		return nullptr;
    }
    
    stack<Node*> stack;
    stack.push(root);
    while(!stack.empty())
    {
        Node* preorderElement = stack.top();
        stack.pop();
        if(preorderElement->right != NULL)
            stack.push(preorderElement->right);
        if(preorderElement->left != NULL)
        stack.push(preorderElement->left);

        if(preorderElement->NAME == NAME)
        {
            //cout << preorderElement->ID << endl;
            nameFound = true;
			return preorderElement;
        }
    }
	if (nameFound == false)
		return nullptr;
}

void AVLTree::printInorder()
{
   vector<Node*> nodes;
   inorderHelper(root, nodes);
   for(int x = 0; x < nodes.size(); x++)
   {
        if(x != nodes.size() - 1)
            cout << nodes.at(x)->NAME << ", ";
        else
            cout << nodes.at(x)->NAME << endl;
   }
}

vector<int> AVLTree::inorder()
{
    vector<Node*> nodes;
    inorderHelper(root, nodes);
    vector<int> nodeIDs;
    for(int x = 0; x < nodes.size(); x++)
        nodeIDs.push_back(nodes.at(x)->ID);
    return nodeIDs;
}

void AVLTree::printPreorder()
{
    vector<Node*> nodes;
    preorderHelper(root, nodes);
    for(int x = 0; x < nodes.size(); x++)
    {
        if(x != nodes.size() - 1)
            cout << nodes.at(x)->NAME << ", ";
        else
            cout << nodes.at(x)->NAME << endl;
    }
}

vector<int> AVLTree::preorder()
{
    vector<Node*> nodes;
    preorderHelper(root, nodes);
    vector<int> nodeIDs;
    for(int x = 0; x < nodes.size(); x++)
        nodeIDs.push_back(nodes.at(x)->ID);
    return nodeIDs;
}

void AVLTree::printPostorder()
{
    vector<Node*> nodes;
    postorderHelper(root, nodes);
    for(int x = 0; x < nodes.size(); x++)
    {
        if(x != nodes.size() - 1)
            cout << nodes.at(x)->NAME << ", ";
        else
            cout << nodes.at(x)->NAME << endl;
    }
}

void AVLTree::inorderHelper(Node* head, vector<Node*>& nodes)
{
    if(head == NULL)
        return;
    else
    {
        inorderHelper(head->left, nodes);
        nodes.push_back(head);
        inorderHelper(head->right, nodes);
    }  
}

void AVLTree::preorderHelper(Node* head, vector<Node*>& nodes)
{
    if(head == NULL)
        return;
    else
    {
        nodes.push_back(head);
        preorderHelper(head->left, nodes);
        preorderHelper(head->right, nodes);
    }
}

void AVLTree::postorderHelper(Node* head, vector<Node*>& nodes)
{
    if(head == NULL)
        return;
    else
    {
        postorderHelper(head->left, nodes);
        postorderHelper(head->right, nodes);
        nodes.push_back(head);
    }
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

void AVLTree::removeInorder(int N)
{
    if(root == NULL)
    {
        cout << "unsuccessful" << endl;
        return;
    }
    else
    {
        vector<Node*> inorderNodes;
        removeInorderHelper(root, inorderNodes);
        if(N >= inorderNodes.size())
            cout << "unsuccessful" << endl;
        else
        {
            remove(inorderNodes.at(N)->ID);
        }
    }
} 

void AVLTree::removeInorderHelper(Node* node, vector<Node*>& inorderNodes)
{
    if(node == NULL)
        return;

    removeInorderHelper(node->left, inorderNodes);
    inorderNodes.push_back(node);
    removeInorderHelper(node->right, inorderNodes); 
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
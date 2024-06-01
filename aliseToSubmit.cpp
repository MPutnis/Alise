#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class Node {
public:
    int data;
    Node* firstChild;
    Node* nextSibling;
    Node(int d) : data(d), firstChild(nullptr), nextSibling(nullptr) {}
};

// function to count numbers seperated by ' ' in a string
int numCount(string l)
{
    int count = 1;
    for(char c: l)
    {
        if(c == ' ') count++;
    }
    return count;
}

// function to convert string to int array
int* strToIntArr(string l)
{
    // count level elements
    int lcount = numCount(l);
    // make array to hold numbers
    int* level = new int[lcount];
    int currNum = 0; // used to make a number
    int index = 0; // index of level array
    bool inNumber = false; // check if we are making a number
    for(char c: l)
    {
        if( c >= '0' && c <= '9')
        {
            currNum = currNum * 10 + (c - '0'); //multiply current number by 10 and add new digit
            inNumber = true;
        }
        else if(c == ' ' && inNumber) // space reached, insert current number into array
        {
            level[index] = currNum;
            currNum = 0;
            index++;
            inNumber = false;
        }
    }
    // insert last number if string does not end with space
    if(inNumber) level[index++] = currNum;
    return level;
}

// function to build a new TreeNode
Node* buildNode( string l)
{
    int* level = strToIntArr(l);
    int levelCount = numCount(l);
    Node* parent = new Node(level[0]);
    Node* current = parent;
    for(int i = 1; i < levelCount; i++)
    {
        if (i == 1)
        {
            current->firstChild = new Node(level[i]);
            current = current->firstChild;
        }
        else
        {
            current->nextSibling = new Node(level[i]);
            current = current->nextSibling;
        }
    }
    return parent;
}

// function to find a node in the tree
Node* findNode(Node* root, int d)
{
    if( root == nullptr) // tree is empty
        return nullptr;
    if( root->data == d) // root is the node we are looking for
        return root;
    Node* found = findNode(root->firstChild, d); // check children
    if( found != nullptr) // if found in children, return found
        return found;
    return findNode(root->nextSibling, d);
}

// function to delete tree
void deleteTree(Node* root)
{
    if(root == nullptr) return;
    deleteTree(root->firstChild);
    deleteTree(root->nextSibling);
    delete root;
}
// function to mirror tree
Node* mirrorTree(const Node* root) {
    if (root == nullptr) return nullptr;

    // create a new node for mirror tree
    Node* newRoot = new Node(root->data);

    // recursion to mirror the children and siblings and set as firstChild and nextSibling of the new node
    Node* current = root->firstChild;
    Node* prev = nullptr;
    while (current != nullptr) {
        Node* newNode = mirrorTree(current);
        newNode->nextSibling = prev;
        prev = newNode;
        current = current->nextSibling;
    }
    newRoot->firstChild = prev;

    return newRoot;
}

//  function to print tree
void printTree(Node* root)
{
    if(root == nullptr) return;
    
    if ( root->firstChild != nullptr)
    {
        cout << root->data << " ";
        Node* child = root->firstChild;
        while( child != nullptr)
        {
            cout << child->data << " ";
            child = child->nextSibling;
        }
        cout << endl;
    
        child = root->firstChild;
        while( child != nullptr)
        {
            if (child->firstChild != nullptr)
            {
                printTree(child);
            }
            child = child->nextSibling;
        }
    }
    else if ( root->firstChild == nullptr)
    {
        cout << root->data << endl;
    }
}

// function to write tree to file
void writeTree(Node* root, ofstream& fout)
{
    if(root == nullptr) return;
    
    if (root->firstChild != nullptr)
    {
        fout << root->data << " ";
        Node* child = root->firstChild;
        while(child != nullptr)
        {
            fout << child->data << " ";
            child = child->nextSibling;
        }
        fout << endl;
    
        child = root->firstChild;
        while(child != nullptr)
        {
            if(child->firstChild != nullptr) // Only call printTree if child has its own children
            {
                writeTree(child, fout);
            }
            child = child->nextSibling;
        }
    }
    else if (root->firstChild == nullptr)
    {
        fout << root->data << endl;
    }
}

int main()
{
    ifstream fin("alice.in");
    //check if file is open
    if (!fin) 
    {
        cout << "File not found" << endl;
        return 1;
    }
    ofstream fout("alice.out");

    string level;
    std::getline(fin, level);
    Node* root = nullptr;
    Node* freeNodes = nullptr;
    while(fin)
    {
        
        //make new node
        Node* newNode = buildNode(level);
        cout << "new node: " << newNode->data << endl;
        //check if root is empty
        if (root == nullptr)
        {
            root = newNode;
            cout << "root: " << root->data << endl;
            printTree(root);
        }
        // if root is not empty
        else
        {
            // check if root contains parent node
            Node* find = findNode(root, newNode->data);
            // if parent node is found, add children to existing parent
            if (find != nullptr)
            {
                find->firstChild = newNode->firstChild;
            }
        }
        
        if (level == "0") break;

        std::getline(fin, level);
    }

    
    // cout << "tree:\n";
    // printTree(root);
    // cout << "0\n";
    Node* mirror = mirrorTree(root);
    // write mirror tree to file
    writeTree(mirror, fout);
    fout << "0";
    // cout << "mirror:\n";
    // printTree(mirror);
    // cout << "0\n";

    deleteTree(root);
    deleteTree(mirror);
    fout.close();
    return 0;
}
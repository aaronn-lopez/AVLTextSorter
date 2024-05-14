#pragma once

#include "Wordlist_base.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Wordlist : public Wordlist_base
{

    //Use this Node to implement an AVL tree for the word list
    //You can *add* extra variables/methods/features to this struct if you like, but you must
    //keep its name the same, and also keep the word, count, left, and right variables as defined
    struct Node
    {
        string word;
        int count;
        Node *left;
        Node *right;
        Node *parent;
        int height;
    };

    Node *root = nullptr;

    //IMPORTANT: root is the only variable that can be defined in this class
    //It should point to the top node of your AVL tree. When root == nullptr,
    //the tree is empty. No variables other than root are permitted!

int height(Node *N)  
{  
    if (N == nullptr)  

        return -1;  

    return N->height;  
} 

    int getBalance(Node *N)  
    {  
        if (N == nullptr)
        {
            return -1;
        }
        else
        {
            return height(N->left) - height(N->right);
        }    
    }  

    //The next two functions are helper functions for rotations to adjust height 
    void singleRotationAdjH(Node *imb, Node *below)
    {
        if(imb->left == nullptr && imb->right == nullptr)
        {
            imb->height = 0;
        }
        else if(imb->left == nullptr && imb->right != nullptr)
        {
            imb->height = height(imb->right) + 1; 
        }
        else if(imb->left != nullptr && imb->right == nullptr)
        {
            imb->height = height(imb->left) + 1; 
        }
        else if(imb->left != nullptr && imb->right != nullptr)
        {
            imb->height = max(height(imb->left), height(imb->right))+ 1; 
        }
        else
        {
            imb->height = max(height(imb->left), height(imb->right)) + 1;
        }
        below->height = max(height(below->left), height(below->right)) + 1;
    }

    void doubleRotationAdjH(Node *imb, Node *below, Node* base)
    {
        if(imb->left == nullptr && imb->right == nullptr)
        {
            imb->height = 0;
        }
        else if(imb->left == nullptr && imb->right != nullptr)
        {
            imb->height = height(imb->right) + 1; 
        }
        else if(imb->left != nullptr && imb->right == nullptr)
        {
            imb->height = height(imb->left) + 1; 
        }
        else
        {
            imb->height = max(height(imb->left), height(imb->right)) + 1;
        }
        if(below->left == nullptr && below->right == nullptr)
        {
            below->height = 0;
        }
        else if(below->left == nullptr && below->right != nullptr)
        {
            below->height = height(below->right) + 1; 
        }
        else if(below->left != nullptr && below->right == nullptr)
        {
            below->height = height(below->left) + 1; 
        }
        else
        {
            below->height = max(height(below->left), height(below->right)) + 1;
        }
        base->height = max(height(base->left), height(base->right)) + 1;
    }

    //The next 4 functions rotate three nodes, one of which is imbalanced
    //Note: recognize that the "RR and LL" rotations are "backwards" as I created the
    //add_word function in a way that has a > b (ASCII value of a is lesser than b)
    void RightRightRotate(Node *imbalanced)
    {
        Node *imbalancedNode = nullptr;
        Node *nodeBelow = nullptr;
        imbalancedNode = imbalanced;
        nodeBelow = imbalancedNode->left;

        imbalancedNode->left = nodeBelow->right;
        if(nodeBelow->right != nullptr)
        {
            nodeBelow->right->parent = imbalancedNode;
        }
        nodeBelow->right = imbalancedNode;
        if(imbalancedNode->parent != nullptr)
        {
            if(imbalancedNode->parent->right == imbalancedNode)
            {
                imbalancedNode->parent->right = nodeBelow;
            }
            else if(imbalancedNode->parent->left == imbalancedNode)
            {
                imbalancedNode->parent->left = nodeBelow;
            }
        }
        nodeBelow->parent = imbalancedNode->parent;
        imbalancedNode->parent = nodeBelow;
        if(root == imbalanced)
        {
            root = nodeBelow;
        }
        singleRotationAdjH(imbalancedNode, nodeBelow);
    }

    void LeftLeftRotate(Node *imbalanced)
    {
        Node *imbalancedNode = nullptr;
        Node *nodeBelow = nullptr;
        imbalancedNode = imbalanced;
        nodeBelow = imbalancedNode->right;

        imbalancedNode->right = nodeBelow->left;
        if(nodeBelow->left != nullptr)
        {
            nodeBelow->left->parent = imbalancedNode;
        }
        nodeBelow->left = imbalancedNode;
        if(imbalancedNode->parent != nullptr)
        {
            if(imbalancedNode->parent->right == imbalancedNode)
            {
                imbalancedNode->parent->right = nodeBelow;
            }
            else if(imbalancedNode->parent->left == imbalancedNode)
            {
                imbalancedNode->parent->left = nodeBelow;
            }
        }
        nodeBelow->parent = imbalancedNode->parent;
        imbalancedNode->parent = nodeBelow;
        if(root == imbalanced)
        {
            root = nodeBelow;
        }
        singleRotationAdjH(imbalancedNode, nodeBelow);
    }

    void LeftRightRotate(Node *imbalanced)
    {
        Node *imbalancedNode = nullptr;
        Node *nodeBelow = nullptr;
        Node *baseNode = nullptr;
        imbalancedNode = imbalanced;
        nodeBelow = imbalancedNode->left;
        baseNode = imbalancedNode->left->right;

        imbalancedNode->left = baseNode->right;
        nodeBelow->right = baseNode->left;
        if(baseNode->left != nullptr)
        {
            baseNode->left->parent = nodeBelow;
        }
        if(baseNode->right != nullptr)
        {
            baseNode->right->parent = imbalancedNode;
        }
        baseNode->right = imbalancedNode;
        baseNode->left = nodeBelow;
        if(imbalancedNode->parent != nullptr)
        {
            if(imbalancedNode->parent->right == imbalancedNode)
            {
                imbalancedNode->parent->right = baseNode;
            }
            else if(imbalancedNode->parent->left == imbalancedNode)
            {
                imbalancedNode->parent->left = baseNode;
            }
            baseNode->parent = imbalancedNode->parent;
        }
        else
        {
            baseNode->parent = nullptr;
        }
        nodeBelow->parent = baseNode;
        imbalancedNode->parent = baseNode;
        if(root == imbalanced)
        {
            root = baseNode;
        }
        doubleRotationAdjH(imbalancedNode, nodeBelow, baseNode);
    }

    void RightLeftRotate(Node *imbalanced)
    {
        Node *imbalancedNode = nullptr;
        Node *nodeBelow = nullptr;
        Node *baseNode = nullptr;
        imbalancedNode = imbalanced;
        nodeBelow = imbalancedNode->right;
        baseNode = imbalancedNode->right->left;

        imbalancedNode->right = baseNode->left;
        nodeBelow->left = baseNode->right;
        if(baseNode->left != nullptr)
        {
            baseNode->left->parent = imbalancedNode;
        }
        if(baseNode->right != nullptr)
        {
            baseNode->right->parent = nodeBelow;
        }
        baseNode->left = imbalancedNode;
        baseNode->right = nodeBelow;
        if(imbalancedNode->parent != nullptr)
        {
            if(imbalancedNode->parent->right == imbalancedNode)
            {
                imbalancedNode->parent->right = baseNode;
            }
            else if(imbalancedNode->parent->left == imbalancedNode)
            {
                imbalancedNode->parent->left = baseNode;
            }
            baseNode->parent = imbalancedNode->parent;
        }
        else
        {
            baseNode->parent = nullptr;
        }
        nodeBelow->parent = baseNode;
        imbalancedNode->parent = baseNode;
        if(root == imbalanced)
        {
            root = baseNode;
        }
        doubleRotationAdjH(imbalancedNode, nodeBelow, baseNode);
    }

    //Code for the next few functions are closely related
    //removeAll is a helper function that is for the destructor
    void removeAll(Node *temp)
    {
        if(temp != nullptr)
        {
            removeAll(temp->left);
            removeAll(temp->right);
            delete temp;
        }
    }

    int num_different_words(Node *temp) const
    {
        if(temp == nullptr)
        {
            return 0;
        }
        else
        {
            return(1+ num_different_words(temp->left) + num_different_words(temp->right));
        }
    }

    int total_words(Node *temp) const
    {
        if(temp == nullptr)
        {
            return 0;
        }
        else
        {
            return(temp->count + total_words(temp->left) + total_words(temp->right));
        }
    }
    
    //uses parameters to track if all nodes are "in order"
    bool is_sorted(Node *temp, string &less, string &more) const
    {
        if(temp == nullptr)
        {
            return true;
        }
        if(temp->word > less || temp->word < more)
        {
            return false;
        }
        
        return is_sorted(temp->right, less, temp->right->word) && is_sorted(temp->left, temp->left->word, more);
    }
    
    //again uses parameters to track if nodes being compared hold higher value
    string most_frequent(Node *temp, string &most, int &count) const
    {   
        //string word as a placeholder so that we can recursively call this function
        string word;

        if(temp == nullptr) 
        {
            return most;
        }
        if(temp->count > count) 
        {
            most = temp->word;
            count = temp->count;
        }

        //case where the count is equal we select the alphabetically higher word
        else if(temp->count == count && temp->word < most)
        {
            most = temp->word;
            count = temp->count;
        }
        word = most_frequent(temp->left, most, count);
        word = most_frequent(temp->right, most, count);
        return(most + " " + to_string(count));
    }

    int num_singletons(Node *temp) const
    {
        if(temp == nullptr)
        {
            return 0;
        }
        else
        {
            if(temp->count == 1)
            {
                return(1 + num_singletons(temp->left) + num_singletons(temp->right));               
            }
            else
            {
                return(num_singletons(temp->left) + num_singletons(temp->right)); 
            }
        }    
    }
    
    void print_words(Node *temp, int &counter) const
    {   
        if(temp == nullptr)
        {
            return;
        }
        print_words(temp->right, counter);
        counter++;
        cout<< counter << ". {\"" << temp->word;
        cout << "\", " << temp->count << '}' << endl;
        print_words(temp->left, counter);
    }

public:

    //Constructors and desctructors below
    Wordlist()
    {

    }

    Wordlist(string filename)
    {
        ifstream fileInput;
        fileInput.open(filename);
        string word;

        while(fileInput >> word)
        {
            add_word(word);
        }
        fileInput.close();
    }

    ~Wordlist()
    {
        removeAll(root);
    }

    //The remaining functions below are the 8 virtual methods that needed to be implemented
    int get_count(const string &w) const
    {
        Node* temp = root;

        while(temp != nullptr)
        {
            if(w > temp->word)
            {
                temp = temp->left;
            }
            else if(w < temp->word)
            {
                temp = temp->right;
            }
            else if(temp->word == w)
            {
                break;
            }
        }
        if(temp != nullptr)
        {
            return(temp->count);
        }
        else
        {
            return 0;
        }
    }

    int num_different_words() const
    {
        return(num_different_words(root));
    }

    int total_words() const
    {
        return(total_words(root));
    }
    
    //set base min and max values to begin recursively traversing tree(post-order)
    bool is_sorted() const
    {
        string min = root->left->word;
        string max = root->right->word;
        return is_sorted(root, min, max);
    }
    
    //set base word and int to begin recursively traversing tree(similar to above)
    string most_frequent() const
    {
        string firstW = root->word;
        int firstC = 0;
        return(most_frequent(root, firstW, firstC));
    }

    int num_singletons() const
    {
        return(num_singletons(root));
    }
    
    void print_words() const
    {
        int count = 0;
        print_words(root, count);
    }

    //add_word function that adds a word as a leaf (dependant on the word's value)
    //Note: again, pre-warning that I initially created with a > b contrary
    //to the way ASCII values hold for all letters
    void add_word(const string &w)
    {
        Node *temp = root;
        bool leftOrRight = false;
        string imb;
        int balanceFactor = 0;
        bool firstNode = false;

        if(root == nullptr)
        {
            root = new Node{w, 1, nullptr, nullptr, nullptr, 0};
            firstNode = true;
        }
        else
        {
            //start at root and walk down tree until we are at a spot to place the new leaf
            while(temp != nullptr)
            {   
                if(w < temp->word)
                {
                    if(temp->right == nullptr)
                    {
                        leftOrRight = false;
                        break;
                    }
                    else
                    {
                        temp = temp->right;
                    }
                }
                else if(w > temp->word)
                {
                    if(temp->left == nullptr)
                    {
                        leftOrRight = true;
                        break;
                    }
                    else
                    {
                        temp = temp->left;
                    }
                }
                else if(w == temp->word)
                {
                    break;
                }
            }

            //place leaf to the left or right of temp node
            if(leftOrRight == false && w != temp->word)
            {
                temp->right = new Node{w, 1, nullptr, nullptr, temp, 0};
            }
            else if(leftOrRight == true && w != temp->word)
            {
                temp->left = new Node{w, 1, nullptr, nullptr, temp, 0};
            }
            else if(w == temp->word)
            {
                temp->count += 1;

                //set temp to nullptr here as we did not add a node and therefore do not need to incrament height or rotate
                temp = nullptr; 
            }
            
            //here we walk up the tree and incrament the height of parent nodes and simultaneously check for imbalances
            while(temp != nullptr)
            {
                if(firstNode == false)
                {
                    temp->height = 1 + max(height(temp->left), height(temp->right)); 
                }
                balanceFactor = getBalance(temp);
                if(balanceFactor < -1 || balanceFactor > 1)
                {
                    imb = temp->word;
                    if(balanceFactor == 2 && w > temp->left->word)
                    {
                        RightRightRotate(temp);
                    }
                    else if(balanceFactor == -2 && w < temp->right->word)
                    {
                        LeftLeftRotate(temp);
                    }
                    else if(balanceFactor == -2 && w > temp->right->word)
                    {
                        RightLeftRotate(temp);
                    }
                    else if(balanceFactor == 2 && w < temp->left->word)
                    {
                        LeftRightRotate(temp);
                    }
                }
                else
                {
                    temp = temp->parent;
                }
            }
        }
    }
}; // class Wordlist

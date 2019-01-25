#include <iostream>
#include <stack>
#include <ctime>
#include <queue>
using namespace std;

int max(int a, int b){
    if (a > b)
        return a;
    else if (b > a)
        return b;
    else
        return a; }

template <class T>
class AVL;

template <class T>
class node{
    node<T>* left;
    node<T>* right;
    node<T>* parent;
    T data;
public:
    node(T newdata = T(), node<T>* newparent = nullptr, node<T>* newleft = nullptr, node<T>* newright = nullptr){
        data = newdata; parent = newparent; left = newleft; right = newright;
    }
    friend class AVL < T > ;
};

template <class T>
class AVL{
    int countOfnodes = 0;
    node<T>* recursiveCopy(const node<T>* rhs);
    void printInOrder(node<T>* ptr);
    void printLevelOrder(node<T>* ptr);

public:
    node<T>* root;
    
    AVL() :root(nullptr){}
    AVL(const AVL<T>& rhs) :root(nullptr){ *this = rhs; }
    ~AVL(){clear(); }
    bool isEmpty(){ return root == nullptr; }
    bool findInTree(const T& val){ return find(val) != nullptr; }
    void clear(){ while (!isEmpty()) remove(root -> data, root); }
    int size(){ return countOfnodes; }
    
    //
    //
    // FUNCTIONS ADDED FOR AVL TREE IMPLEMENTATION
    //
    //
    node<T>* get_root_ptr(){return root; }
    int height(node<T>* ptr);
    int depth(node<T>* ptr);
    int get_balance(node<T>* ptr);
    void balance(node<T>*& ptr);
    void rotateClockwise(node<T>*& ptr);
    void rotateCounterClockwise(node<T>*& ptr);
    void doubleClockwise(node<T>*& ptr);
    void doubleCounterClockwise(node<T>*& ptr);
    //
    //
    //
    
    AVL<T>& operator=(const AVL<T>& rhs);
    void insert(T& val, node<T>*& root);
    void remove(T& val, node<T>*& ptr);
    node<T>* find(const T&);
    void printInOrder(){ if (root!=nullptr) printInOrder(root); }
    void printLevelOrder(){ if (root!=nullptr) printLevelOrder(root); }

};

template<class T>
void AVL<T>::printInOrder(node<T>* ptr){
    if (ptr->left != nullptr)
        printInOrder(ptr->left);
    cout << ptr->data<<endl;
    if (ptr->right != nullptr)
        printInOrder(ptr->right);
}

template<class T>
void AVL<T>::printLevelOrder(node<T>* ptr){
    queue<node<T>*> q;
    q.push(ptr);
    int tempDepth = depth(ptr);
    while(! q.empty()){
        node<T>* temp = q.front();
        if (depth(temp) > tempDepth){
            cout<<endl<<endl<<"Depth = "<<depth(temp)<< endl<<endl;
            tempDepth++;
        }
        cout << temp -> data<<"  ";
        q.pop();
        if(temp -> left != nullptr)
            q.push(temp -> left);
        if(temp -> right != nullptr)
            q.push(temp -> right);
    }
}

template <class T>
node<T>* AVL<T>::find(const T& val){
    node<T>* temp = root;
    while (temp != nullptr && temp->data != val){
        if (val < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    return temp;
}

template<class T>
int AVL<T> :: height(node<T>* ptr){
    // BASE CASE: height at leaf node (i.e. when ptr = null) = 0
    int tempHeight = -1;
    if (ptr == nullptr)
        return tempHeight;
    else if(ptr != nullptr){     // Return max height from this node down the Tree
        int L_height = height(ptr -> left);
        int R_height = height(ptr -> right);
        int tempMax = max(L_height, R_height);
        tempHeight = tempMax + 1;
    }
    return tempHeight;
}

template<class T>
int AVL<T> :: depth(node<T>* ptr){
    int tempDepth = 0;
    node<T>* tempPtr = ptr;
    while(tempPtr -> parent != nullptr){
        tempDepth++;
        tempPtr = tempPtr -> parent;
    }
    return tempDepth;
}


template<class T>
int AVL<T> :: get_balance(node<T>* ptr){
    // Precondition: Check if ptr == nullptr: if so,
    // Get height from L and R side
    // Return the Difference between L and R heights
    int tempBalance = height(ptr -> left) - height(ptr -> right);
    return tempBalance;
}


template<class T>
void AVL<T> :: rotateClockwise(node<T>*& ptr){
    node<T>* tempPtr = ptr -> left;  // create a TEMP POINTER that points to ptr -> left
    ptr -> left = tempPtr -> right;  // Reassign tempPtr -> right to be the LEFT CHILD of ptr
    tempPtr -> right = ptr;  // Move tempPtr up and make original ptr its RIGHT CHILD
    ptr = tempPtr;
}

template<class T>
void AVL<T> :: rotateCounterClockwise(node<T>*& ptr){
    node<T>* tempPtr = ptr -> right;  // create a TEMP POINTER that points to ptr -> left
    ptr -> right = tempPtr -> left;  // Reassign tempPtr -> right to be the LEFT CHILD of ptr
    tempPtr -> left = ptr;  // Move tempPtr up and make original ptr its RIGHT CHILD
    ptr = tempPtr;
}

template<class T>
void AVL<T> :: doubleClockwise(node<T>*& ptr){
    if((ptr != nullptr) &&(ptr -> left != nullptr))
        rotateCounterClockwise(ptr -> left);
    rotateClockwise(ptr);
}

template<class T>
void AVL<T> :: doubleCounterClockwise(node<T>*& ptr){
    if((ptr != nullptr) &&(ptr -> right != nullptr))
        rotateClockwise(ptr -> right);
    rotateCounterClockwise(ptr);
}


template<class T>
void AVL<T> :: balance(node<T>*& ptr){
    // Takes as parameter the pointer returned by insert and remove functions
    // If an UNBALANCED node is found, determine the type of rotation needed to RE-BALANCE the tree
    
    if(ptr == nullptr)
        return;
    
    int tempBalance;
    tempBalance = get_balance(ptr);
    
    if(tempBalance > 1){
        if(get_balance(ptr -> left) > 0){
            rotateClockwise(ptr); }
        else{
            doubleClockwise(ptr); }
    }
    else if(tempBalance < -1){
        if(get_balance(ptr -> right) < 0){
            rotateCounterClockwise(ptr); }
        else{
            doubleCounterClockwise(ptr); }
    }
}

template <class T>
void AVL<T>::remove(T& val, node<T>*& root){
    if (root == nullptr) //someone asked me to remove a value that isnt in the tree... okay, done!
        return;
    else if (val == root -> data){
        if (root->left == nullptr && root->right == nullptr){ //no children, delete and update parent
            root = nullptr;
            delete root;
            countOfnodes--;
        }
        else if (root->left == nullptr) { //node has a right child
            node<T>* tempRoot = root;
            node<T>* tempChild = root->right;
            node<T>* tempParent = root->parent;
            if (tempParent != nullptr){
                if (tempRoot == tempParent->left){
                    tempParent->left = tempChild;
                    tempRoot = nullptr;
                    delete tempRoot;}
                else{
                    tempParent->right = tempChild;
                    tempRoot = nullptr;
                    delete tempRoot;}
            }
            else{
                root = tempChild;
                tempRoot = nullptr;
                delete tempRoot; }
            countOfnodes--;
        }
        else if (root->right == nullptr) { //node has a left child
            node<T>* tempRoot = root;
            node<T>* tempChild = root->left;
            node<T>* tempParent = root->parent;
            if (tempParent != nullptr){
                if (tempRoot == tempParent->left){
                    tempParent->left = tempChild;
                    tempRoot = nullptr;
                    delete tempRoot;}
                else{
                    tempParent->right = tempChild;
                    tempRoot = nullptr;
                    delete tempRoot;}
            }
            else{
                root = tempChild;
                tempRoot = nullptr;
                delete tempRoot; }
            countOfnodes--;
        }
        else{ //the node has two children!!! - Bad
            //Replace the data with the min of the right subtree
            node<T>* temp = root->right;
            while (temp->left != nullptr)
                temp = temp->left;
            root->data = temp->data;
            temp = nullptr;
            delete temp; //Recursion at its finest....ahhh!
        }
    }
    else if (val < root -> data)
        remove(val, root -> left);
    else if(val > root -> data)
        remove(val, root -> right);
    balance(root);
}

template <class T>
void AVL<T>::insert(T& val, node<T>*& root){
    if (root == nullptr){//New node is the first on the tree
        root = new node<T>(val);
        countOfnodes++;
    }
    else if(val < root -> data){
        insert(val, root -> left);
    }
    else{
        insert(val, root -> right);
    }
    balance(root);
}

template <class T>
node<T>* AVL<T>::recursiveCopy(const node<T>* rhs){
    if (rhs == nullptr)
        return nullptr;
    node<T>* temp = new node<T>(rhs->data);
    temp->left = recursiveCopy(rhs->left);
    temp->right = recursiveCopy(rhs->right);
    if (temp->left!=nullptr)
        temp->left->parent = temp;
    if (temp->right)
        temp->right->parent = temp;
    return temp;
}

template <class T>
AVL<T>& AVL<T>::operator=(const AVL<T>& rhs){
    if (this == &rhs)
        return *this;
    clear();
    root = recursiveCopy(rhs.root);
    countOfnodes = rhs.countOfnodes;
    return *this;
}

int main(){
    int tempVal;
    AVL < int > t1;
    srand(time(NULL));
    for (int i = 0; i < 100; i++){
        tempVal = rand() % 1000;
        t1.insert(tempVal, t1.root); }
    
    AVL<int> t2 = t1;
    
    for (int i = 0; i < 20; i++){
        tempVal = rand() % 1000;
        t2.remove(tempVal, t2.root); }
    
    t2.printLevelOrder();
    cout<< endl<<endl;
    
    cout<<"end of program."<<endl;
    
    return 0;
    
}


#include <iostream>
#include <bits/stdc++.h>
#include <time.h>
#include <chrono>
#include <array>
#include <vector>
#include <algorithm>
using namespace std;
using namespace std::chrono;

enum Color {RED, BLACK};
//NODE STRUCTURE

template<class T>
struct Node
{
    T data;
    bool color;
    Node<T> *left, *right, *parent;

    // Constructor
    Node(T data)
    {
       this->data = data;
       left = right = parent = NULL;
       this->color = RED;
    }

    // check if node is left child of parent
  bool isOnLeft() { return this == parent->left; }

    // returns pointer to uncle
  Node<T> *uncle() {
    // If no parent or grandparent, then no uncle
    if (parent == NULL or parent->parent == NULL)
      return NULL;

    if (parent->isOnLeft())
      // uncle on right
      return parent->parent->right;
    else
      // uncle on left
      return parent->parent->left;
  }
    // returns pointer to sibling
  Node<T> *sibling(){
    // sibling null if no parent
    if (parent == NULL)
      return NULL;

    if (isOnLeft())
      return parent->right;

    return parent->left;
  }

  bool hasRedChild() {
    return (left != NULL and left->color == RED) or
           (right != NULL and right->color == RED);
  }

  void moveDown(Node<T> *nParent) {
    if (parent != NULL) {
      if (isOnLeft()) {
        parent->left = nParent;
      } else {
        parent->right = nParent;
      }
    }
    nParent->parent = parent;
    parent = nParent;
  }
};

//Template Class
template <class T> class RBTree{
    Node<T>* root;
protected:
    void rotateLeft(Node<T> *&, Node<T> *&);
    void rotateRight(Node<T> *&, Node<T> *&);
    void fixViolation(Node<T> *&, Node<T> *&);

public:
    //Constructor
    RBTree(){
        root = NULL;
    }

    //Scramble and Print
    void scramble_array(T arr[],int size); //Size must be INT
    void inorderHelper(Node<T>* root);
    void inorder();
    void levelOrder();
    void printPostorderHelper(Node<T>* root);
    void postorder();
    //Insert
    void insert(T n);
    Node<T>* BSTInsert(Node<T>* root, Node<T> *pt);

    //Search and levels
    bool search(T x, bool found);
    bool BSTSearch(Node<T>* root, T node, bool found);
    Node<T>* searchNode(T x);

    Node<T>* BSTSearchNode(Node<T>* n, T x);
    int MaxDepth(Node<T>* node); //Method must be Int
    int ObtainDepth(T number);//Method must be Int

    //Delete
    void deleteVal(T n);
    void deleteNode(Node<T> *v);//Must recieve Node
    void fixDoubleBlack(Node<T> *x);
    void leftRotate(Node<T>* x);
    void rightRotate(Node<T>* x);
    void swapColors(Node<T>* x1, Node<T>* x2);
    void swapValues(Node<T>* u, Node<T>* v);
    void fixRedRed(Node<T>* x);
    Node<T>* successor(Node<T>* x);
    Node<T>* BSTreplace(Node<T>* x);
    Node<T>* GetRoot(){ return root; }

    //Must implement (implemented in Class method)
    T obtenerNivel(T data);
    T obtenerNivelTool(struct Node<T> *Node, T data, int level);
};

//Scramble the array--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void RBTree<T>::scramble_array(T arr[],int size){
    srand(time(NULL));//Random Seed
    for(int i = 0; i<size;i++){
        arr[i] = rand()%(size*100);
    }
}

//Print in Order-----------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void RBTree<T>::inorder(){
    inorderHelper(root);
}

template <class T>
void RBTree<T>::postorder(){
    printPostorderHelper(root);
}

template <class T>
void RBTree<T>::inorderHelper(Node<T>* root){
    if (root == NULL){
        return;
    }

    inorderHelper(root->left);
    cout << root->data << "  ";
    inorderHelper(root->right);
}

template <class T>
void RBTree<T>::printPostorderHelper(Node<T>* root) {
    if (root == NULL)
        return;

    // first recur on left subtree
    printPostorderHelper(root->left);

    // then recur on right subtree
    printPostorderHelper(root->right);

    // now deal with the node
    cout << root->data << " ";
}

//INSERT NODE----------------------------------------------------------------------------------------------------------------------------------------------------------------
// Function to insert a new node with given data
template <class T>
void RBTree<T>::insert(T data){
    Node<T> *pt = new Node<T>(data);

    // Do a normal BST insert
    root = BSTInsert(root, pt);

    // fix Red Black Tree violations
    fixViolation(root, pt);
}

//BSTINSERT METHOD
template <class T>
Node<T>* RBTree<T>::BSTInsert(Node<T>* root, Node<T> *pt){
    /* If the tree is empty, return a new node */
    if (root == NULL)
       return pt; //newNode(key)

    /* Otherwise, recur down the tree */
    if (pt->data < root->data) //key < node-> key
    {
        root->left  = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else if (pt->data > root->data)
    {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }

    /* return the (unchanged) node pointer */
    return root;
}

// This function fixes violations caused by BST insertion
template <class T>
void RBTree<T>::fixViolation(Node<T> *&root, Node<T> *&pt){
    Node<T> *parent_pt = NULL;
    Node<T> *grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != BLACK) &&
           (pt->parent->color == RED))
    {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
            Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left)
        {

            Node<T> *uncle_pt = grand_parent_pt->right;

            /* Case : 1
               The uncle of pt is also red
               Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }

            else
            {
                /* Case : 2
                   pt is right child of its parent
                   Left-rotation required */
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is left child of its parent
                   Right-rotation required */
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        /* Case : B
           Parent of pt is right child of Grand-parent of pt */
        else
        {
            Node<T> *uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is right child of its parent
                   Left-rotation required */
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
    root->color = BLACK;
}

//Rotate Left
template <class T>
void RBTree<T>::rotateLeft(Node<T> *&root, Node<T> *&pt){
    Node<T> *pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

//Rotate Right
template <class T>
void RBTree<T>::rotateRight(Node<T> *&root, Node<T> *&pt){
    Node<T> *pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

//DELETE NODE----------------------------------------------------------------------------------------------------------------------------------------------------------------
// utility function that deletes the node with given value
template <class T>
void RBTree<T>::deleteVal(T n){
    if (root == NULL)
      // Tree is empty
      return;

    Node<T> *v = searchNode(n), *u;

    if (v->data != n) {
      cout << "No node found to delete with value:" << n << endl;
      return;
    }

    deleteNode(v);
}

//Deletes the given node
template <class T>
void RBTree<T>::deleteNode(Node<T> *v){
    Node<T> *u = BSTreplace(v);

    // True when u and v are both black
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    Node<T> *parent = v->parent;

    if (u == NULL) {
      // u is NULL therefore v is leaf
      if (v == root) {
        // v is root, making root null
        root = NULL;
      } else {
        if (uvBlack) {
          // u and v both black
          // v is leaf, fix double black at v
          fixDoubleBlack(v);
        } else {
          // u or v is red
          if (v->sibling() != NULL)
            // sibling is not null, make it red"
            v->sibling()->color = RED;
        }

        // delete v from the tree
        if (v->isOnLeft()) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }

    if (v->left == NULL or v->right == NULL) {
      // v has 1 child
      if (v == root) {
        // v is root, assign the value of u to v, and delete u
        v->data = u->data;
        v->left = v->right = NULL;
        delete u;
      } else {
        // Detach v from tree and move u up
        if (v->isOnLeft()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          // u and v both black, fix double black at u
          fixDoubleBlack(u);
        } else {
          // u or v red, color u black
          u->color = BLACK;
        }
      }
      return;
    }

    // v has 2 children, swap values with successor and recurse
    swapValues(u, v);
    deleteNode(u);
}

//Sucesor
template <class T>
Node<T>* RBTree<T>::successor(Node<T> *x){
    Node<T> *temp = x;

    while (temp->left != NULL)
      temp = temp->left;

    return temp;
}

//Replace
template <class T>
Node<T>* RBTree<T>::BSTreplace(Node<T> *x){
    // when node have 2 children
    if (x->left != NULL and x->right != NULL)
      return successor(x->right);

    // when leaf
    if (x->left == NULL and x->right == NULL)
      return NULL;

    // when single child
    if (x->left != NULL)
      return x->left;
    else
      return x->right;
}

//Fix Double Black
template <class T>
void RBTree<T>::fixDoubleBlack(Node<T> *x){
    if (x == root)
      // Reached root
      return;

    Node<T> *sibling = x->sibling(), *parent = x->parent;
    if (sibling == NULL) {
      // No sibiling, double black pushed up
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == RED) {
        // Sibling red
        parent->color = RED;
        sibling->color = BLACK;
        if (sibling->isOnLeft()) {
          // left case
          rightRotate(parent);
        } else {
          // right case
          leftRotate(parent);
        }
        fixDoubleBlack(x);
      } else {
        // Sibling black
        if (sibling->hasRedChild()) {
          // at least 1 red children
          if (sibling->left != NULL and sibling->left->color == RED) {
            if (sibling->isOnLeft()) {
              // left left
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rightRotate(parent);
            } else {
              // right left
              sibling->left->color = parent->color;
              rightRotate(sibling);
              leftRotate(parent);
            }
          } else {
            if (sibling->isOnLeft()) {
              // left right
              sibling->right->color = parent->color;
              leftRotate(sibling);
              rightRotate(parent);
            } else {
              // right right
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              leftRotate(parent);
            }
          }
          parent->color = BLACK;
        } else {
          // 2 black children
          sibling->color = RED;
          if (parent->color == BLACK)
            fixDoubleBlack(parent);
          else
            parent->color = BLACK;
        }
      }
    }
}

//Swap Vaues
template <class T>
void RBTree<T>::swapValues(Node<T> *u, Node<T> *v){
    int temp;
    temp = u->data;
    u->data = v->data;
    v->data = temp;
}

template <class T>
void RBTree<T>::leftRotate(Node<T>* x) {
    // new parent will be node's right child
    Node<T>* nParent = x->right;

    // update root if current node is root
    if (x == root)
      root = nParent;

    x->moveDown(nParent);

    // connect x with new parent's left element
    x->right = nParent->left;
    // connect new parent's left element with node
    // if it is not null
    if (nParent->left != NULL)
      nParent->left->parent = x;

    // connect new parent with x
    nParent->left = x;
}

template <class T>
void RBTree<T>::rightRotate(Node<T> *x) {
    // new parent will be node's left child
    Node<T>* nParent = x->left;

    // update root if current node is root
    if (x == root)
      root = nParent;

    x->moveDown(nParent);

    // connect x with new parent's right element
    x->left = nParent->right;
    // connect new parent's right element with node
    // if it is not null
    if (nParent->right != NULL)
      nParent->right->parent = x;

    // connect new parent with x
    nParent->right = x;
}

template <class T>
void RBTree<T>::swapColors(Node<T>* x1, Node<T>* x2) {
    bool temp;
    temp = x1->color;
    x1->color = x2->color;
    x2->color = temp;
}

// fix red red at given node
template <class T>
void RBTree<T>::fixRedRed(Node<T>* x){
    // if x is root color it black and return
    if (x == root) {
      x->color = BLACK;
      return;
    }

    // initialize parent, grandparent, uncle
    Node<T>* parent = x->parent, *grandparent = parent->parent,
         *uncle = x->uncle();

    if (parent->color != BLACK) {
      if (uncle != NULL && uncle->color == RED) {
        // uncle red, perform recoloring and recurse
        parent->color = BLACK;
        uncle->color = BLACK;
        grandparent->color = RED;
        fixRedRed(grandparent);
      } else {
        // Else perform LR, LL, RL, RR
        if (parent->isOnLeft()) {
          if (x->isOnLeft()) {
            // for left right
            swapColors(parent, grandparent);
          } else {
            leftRotate(parent);
            swapColors(x, grandparent);
          }
          // for left left and left right
          rightRotate(grandparent);
        } else {
          if (x->isOnLeft()) {
            // for right left
            rightRotate(parent);
            swapColors(x, grandparent);
          } else {
            swapColors(parent, grandparent);
          }

          // for right right and right left
          leftRotate(grandparent);
        }
      }
    }
}
//SEARCH----------------------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
Node<T>* RBTree<T>::searchNode(T x){
    BSTSearchNode(root, x);
}

template <class T>
bool RBTree<T>::search(T data, bool found){
    BSTSearch(root, data, found);
}

template <class T>
bool RBTree<T>::BSTSearch(Node<T>* root, T node, bool found){

    if(root == NULL){
        found = false;
        return found;
    }

    if(node == root->data){
        found = true;
        return found;
    }

    if(root->data < node){
        return BSTSearch(root->right, node, found);
    }

    return BSTSearch(root->left, node, found);

}

template <class T>
Node<T>* RBTree<T>::BSTSearchNode(Node<T>* root, T node){

    if(root == NULL){
        return 0;
    }

    if(node == root->data){
        return root;
    }

    if(root->data < node){
        return BSTSearchNode(root->right, node);
    }

    return BSTSearchNode(root->left, node);

}

//LEVEL, DEPTH AND HEIGHT----------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
T RBTree<T>::obtenerNivel(T data){
    return obtenerNivelTool(root, data, 1);
}

//Clase para obtener el NIVEL de un nodo.
template <class T>
T RBTree<T>::obtenerNivelTool(struct Node<T> *Node, T data, int level){
    //Si el nodo es nulo.
    if(Node == NULL){
        return 0;
    }

    if(Node -> data == data){
        return level;
    }

    int bajarNivel = obtenerNivelTool(Node -> left, data, level+1);

    if(bajarNivel != 0){
        return bajarNivel;
    }

    bajarNivel = obtenerNivelTool(Node -> right, data, level+1);
    return bajarNivel;
}

//Obtener Height
template <class T>
int RBTree<T>:: RBTree::MaxDepth(Node<T>* node){
    int depth = 0;
    if (node == NULL){
        return 0;
    }

    else{
        int LeftD = MaxDepth(node->left);
        int RightD = MaxDepth(node -> right);

        if(LeftD > RightD){
            return(LeftD+1);
            depth++;
        }
        else{
            return(RightD+1);
            depth++;
        }
    }

}

//MAIN----------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(){
    //Declarar variable
    RBTree <int> tree;
    int menu, option;
    bool menuWhile =true, found;
    Node<int>* node;
    //Crear array de 100,000 elementos.
    int size = 100000;
    int arr [size];
    //Meter elementos aleatorios.
    tree.scramble_array(arr, size);
    //Insertar los elementos al arbol.
    for(int i=0;i<size-9;i++){
     tree.insert(arr[i]);
    };

    while(menuWhile){
    cout<<"--------Red-Black Tree Main Menu-------- \n 1. Print Inorder \n 2. Print Postorder \n 3. Search Element \n 4. Insert Element \n 5. Eliminar Elemento \n 6. Obtain Node Level \n 7. Obtain Node Depth \n 8. Obtain Node Height \n 0. Leave Menu \n";
    cin>>menu;

    if(menu ==0){
        menuWhile=false;
    }

    else if(menu == 1){
        cout << "Inorder Traversal of Created Tree\n";
        tree.inorder();
        cout<<endl;
    }

    else if(menu == 2){
        cout << "Postorder Traversal of Created Tree\n";
        tree.postorder();
        cout<<endl;
    }

    else if(menu == 3){
        cout<<"Buscar en el arbol por el elemento: "<<endl;
        cin>>option;
        bool found;
        auto start = high_resolution_clock::now();
        if(tree.search(option,found)){
            cout<<option<<" founded!"<<endl;
        }
        else{
            cout<<option<<" not founded!"<<endl;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time of execution "<< duration.count() <<" microseconds"<< endl;
        found=true;
    }

    else if(menu == 4){
        cout<<"Insertar el elemento: "<<endl;
        cin>>option;
        tree.insert(option);
    }

    else if(menu == 5){
        cout<<"Eliminar el elemento: "<<endl;
        cin>>option;
        tree.deleteVal(option);
    }

    else if(menu == 6){
        cout<< "Obtener nivel del nodo...:"<<endl;
        cin>>option;
        cout<<"El nivel del nodo "<<option<<" es : ";
        if(tree.obtenerNivel(option)!=0){
            cout<<tree.obtenerNivel(option)<<endl;
        }
        else{
            cout<<"Node not found"<<endl;
        }
    }

    else if(menu == 7){
        cout<< "Obtener profundidad del nodo...:"<<endl;
        cin>>option;
        cout<<"La profundidad del nodo "<<option<<" es: ";
        if((tree.obtenerNivel(option)-1)!=-1){
            cout<<tree.obtenerNivel(option)-1<<endl;
        }
        else{
            cout<<"Node not found"<<endl;
        }
    }

    else if(menu == 8){
        cout<< "Obtener altura del nodo...:"<<endl;
        cin>>option;
        node = tree.searchNode(option);
        cout<<"La altura del nodo "<<option<<" es: ";
        if(tree.MaxDepth(node)!=0){
            cout<<tree.MaxDepth(node)<<endl;
        }
        else{
            cout<<"Node not found"<<endl;
        }
    }

    else{
        cout<<"Wrong input, try again."<<endl;
    }

    }//While
}

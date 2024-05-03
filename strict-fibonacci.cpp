#include <bits/stdc++.h>
using namespace std;
ifstream f("heap.in");
ofstream g("heap.out");

///STRICT FIBONACCI
template <class V> class FibonacciHeap;

const double INF_fibonacci_heap = 2000000001;

template <class V> struct node {
   node<V>* left;
   node<V>* right;
   node<V>* child;
   node<V>* parent;
   V val;
   bool marked;
   int degree;
};

template <class V> class FibonacciHeap{
private:
   node<V>* maxNode;
   node<V>* rootList;

   node<V>* constructNode(V value){
       auto* newNode = new node<V>;
       newNode->left = newNode;
       newNode->right = newNode;
       newNode->child = nullptr;
       newNode->parent = nullptr;
       newNode->degree = 0;
       newNode->val = value;
       newNode->marked = false;
       return newNode;
   }

   void mergeWithRoot(node<V>* mergedNode){
       if (rootList == nullptr)
           rootList = mergedNode;
       else {
           mergedNode->right = rootList;
           mergedNode->left = rootList->left;
           rootList->left->right = mergedNode;
           rootList->left = mergedNode;
       }
   }

   void removeFromRoot(node<V>* removedNode){
       if (removedNode == rootList)
           rootList = removedNode->right;
       removedNode->left->right = removedNode->right;
       removedNode->right->left = removedNode->left;
   }

   void removeFromChildren(node<V>* removedChild, node<V>* parent){
       if (parent->child == parent->child->right)
           parent->child = nullptr;
       else if (parent->child == removedChild) {
           parent->child = removedChild->right;
           removedChild->right->parent = parent;
       }
       removedChild->left->right = removedChild->right;
       removedChild->right->left = removedChild->left;
   }

   void mergeWithChild(node<V>* newChild, node<V>* parent){
       if (parent->child == nullptr)
           parent->child = newChild;
       else{
           newChild->right = parent->child->right;
           newChild->left = parent->child;
           parent->child->right->left = newChild;
           parent->child->right = newChild;
       }
   }

   void heapLink(node<V>* child, node<V>* parent){
       removeFromRoot(child);
       child->left = child->right = child;
       parent->degree++;
       mergeWithChild(child, parent);
       child->parent = parent;
   }

   void cleanUp(){
       std::vector< node<V>* > degreeTable(64, nullptr);
       std::vector< node<V>* > rootNodes = {rootList};

       node<V>* p = rootList->right;
       while (p != rootList) {
           rootNodes.push_back(p);
           p = p->right;
       }

       for (auto rootNode : rootNodes){
           int deg = rootNode->degree;
           while(degreeTable[deg] != nullptr){
               node<V>* degNode = degreeTable[deg];

               if(rootNode->val < degNode->val)
                   std::swap(rootNode, degNode);

               heapLink(degNode, rootNode);
               degreeTable[deg] = nullptr;
               deg++;
           }
           degreeTable[deg] = rootNode;
       }
       for(int i = 0; i < 64; i++)
           if (degreeTable[i] != nullptr)
               if( degreeTable[i]->val > maxNode->val)
                   maxNode = degreeTable[i];
   }

public:
   FibonacciHeap(){
       maxNode = nullptr;
       rootList = nullptr;
   }

   ~FibonacciHeap() = default;

   void insert(V insertedValue){
       node<V>* insertedNode = constructNode(insertedValue);
       mergeWithRoot(insertedNode);
       if (maxNode == nullptr || maxNode->val < insertedValue)
           maxNode = insertedNode;
   }

   void merge(FibonacciHeap<V>* other) {
       if (rootList == nullptr){
           rootList = other->rootList;
           maxNode = other->maxNode;
       }
       else if(other->rootList != nullptr) {
           node<V>* last = other->rootList->left;
           other->rootList->left = rootList->left;
           rootList->left->right = other->rootList;
           rootList->left = last;
           rootList->left->right = rootList;

           if (maxNode->val < other->maxNode->val)
               maxNode = other->maxNode;
       }
   }

   V getMaximum(){
       return maxNode->val;
   }

   V extractMax(){
       node<V>* z = maxNode;
       V maxVal = 0;

       if (z != nullptr){
           if (z->child != nullptr) {
               node<V>* p = rootList->right;
               std::vector<node<V> *> children = {};
               node<V> *currentChild = z->child;
               do{
                   auto temp = currentChild;
                   children.push_back(temp);
                   currentChild = currentChild->right;
               }while(currentChild != z->child);

               for (auto child: children) {
                   mergeWithRoot(child);
                   child->parent = nullptr;
               }
           }

           removeFromRoot(z);

           if (z == z->right){
               maxNode = nullptr;
               rootList = nullptr;
           }
           else{
               maxNode = z->right;
               cleanUp();
           }

           maxVal = z->val;
           delete z;
       }
       return maxVal;
   }
};

using namespace std;

int main() {
   int N, Q;
   vector<FibonacciHeap<int>*> heapArray;
   f >> N >> Q;
   for(int i = 0; i < N + 1; ++i)
       heapArray.push_back(new FibonacciHeap<int>());
   for(int i = 0; i < Q; ++i)
   {
       int operation;
       f >> operation;
       if (operation == 1){
           int set, val;
           f >> set >> val;
           heapArray[set]->insert(val);
       }
       else if (operation == 2){
           int set;
           f >> set;
           g << heapArray[set]->extractMax() << "\n";
       }
       else if (operation == 3){
           int set1, set2;
           f >> set1 >> set2;
           heapArray[set1]->merge(heapArray[set2]);
           delete heapArray[set2];
           heapArray[set2] = new FibonacciHeap<int>();
       }
   }
   return 0;
}

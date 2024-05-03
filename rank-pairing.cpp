#include <bits/stdc++.h>
using namespace std;
ifstream fin("heap.in");
ofstream fout("heap.out");

struct Node
{
    int val;
    Node* copil;
    Node* frate;
    Node(int x = 0, Node* son = NULL, Node* frate_ = NULL) : val(x), copil(son), frate(frate_) {}
};

Node* getRoot(Node* root)
{
    return root;
}

void setRoot(Node** root, Node* point)
{
    *root = point;
}

Node* mergeNodes(Node* h1, Node* h2)
{
    if (h1 == NULL)
        return h2;
    if (h2 == NULL)
        return h1;
    if (h1->val < h2->val)
        swap(h1, h2);
    h2->frate = h1->copil;
    h1->copil = h2;
    return h1;
}

Node* mergeSons(Node* h)
{
    if (h == NULL || h->frate == NULL)
        return h;
    Node* one = h;
    Node* two = h->frate;
    Node* three = h->frate->frate;
    one->frate = two->frate = NULL;
    return mergeNodes(mergeNodes(one, two), mergeSons(three));
}

Node* createHeap(int key)
{
    return new Node(key);
}

int findMax(Node* root)
{
    return root->val;
}

void mergeHeaps(Node** root1, Node* root2)
{
    if (*root1 == NULL)
    {
        *root1 = root2;
        return;
    }
    else if (root2 == NULL)
        return;
    if ((*root1)->val < root2->val)
    {
        swap(*root1, root2);
    }
    root2->frate = (*root1)->copil;
    (*root1)->copil = root2;
}

void push(Node** root, int key)
{
    mergeHeaps(root, createHeap(key));
}

void pop(Node** root)
{
    Node* temp = *root;
    *root = mergeSons(temp->copil);
    delete temp;
}

void mergeSet(Node** root1, Node** root2)
{
    mergeHeaps(root1, *root2);
    *root2 = NULL;
}

void decKey(Node* h, int amount)
{
    Node* kid = h->copil;
    h->copil = NULL;
    kid->val -= amount;
    mergeHeaps(&h, kid);
}

int main()
{
    int n, nr_operatii, op;
    fin >> n >> nr_operatii;
    vector<Node*> h(n, NULL);
    for (int i = 0; i < nr_operatii; i++)
    {
        fin >> op;
        ///Tipul 1
        ///Se adauga la heapul id valoarea val
        if (op == 1)
        {
            int id, val;
            fin >> id >> val;
            auto start = chrono::high_resolution_clock::now();
            push(&h[id], val);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed_seconds  = end - start;
            fout << elapsed_seconds.count() << " secunde" << '\n';
        }
        ///Tipul 2
        ///Se alege maximul din heapul id si se sterge
        else if (op == 2)
        {
            int id;
            fin >> id;
            fout << findMax(h[id]) << "\n";
            auto start = chrono::high_resolution_clock::now();
            pop(&h[id]);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed_seconds  = end - start;
            fout << elapsed_seconds.count() << " secunde" << '\n';
        }
        ///Tipul 3
        ///Se imbina 2 heapuri
        else if (op == 3)
        {
            int heap1, heap2;
            fin >> heap1 >> heap2;
            auto start = chrono::high_resolution_clock::now();
            mergeSet(&h[heap1], &h[heap2]);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed_seconds  = end - start;
            fout << elapsed_seconds.count() << " secunde" << '\n';
        }
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
ifstream fin("heap.in");
ofstream fout("heap.out");

// Structura unui nod pentru arborele skew-binomial
struct nod {
    int valoare, grad;
    nod *copil, *frate, *parinte;
    nod(int valoare_=0, nod* copil_=NULL, nod* frate_=NULL, nod* parinte_=NULL):
        valoare(valoare_), copil(copil_), frate(frate_), parinte(parinte_) {}
};

// Adaugă un arbore în lista de arbori
void adauga_arbore(nod* x, list<nod*> &v) {
    v.push_front(x);
}

// Găsește nodul cu valoarea maximă dintr-un heap de arbori
list<nod*>::iterator gaseste_nod_maxim(list<nod*> &v) {
    list<nod*>::iterator max_it = v.begin();
    for (list<nod*>::iterator it = v.begin(); it != v.end(); it++) {
        if ((*it)->valoare > (*max_it)->valoare)
            max_it = it;
    }
    return max_it;
}

// Șterge rădăcina unui arbore și adaugă copiii săi în heapul de arbori
void sterge_radacina(nod* arbore, list<nod*> &v) {
    if (arbore->grad == 0) {
        delete arbore;
    }
    else {
        nod* temp = arbore;
        arbore->copil->parinte = NULL;
        temp = temp->copil;
        adauga_arbore(arbore->copil, v);
        while (temp->frate != NULL) {
            temp->frate->parinte = NULL;
            adauga_arbore(temp->frate, v);
            temp = temp->frate;
        }
    }
    delete arbore;
}

// Îmbină doi arbori skew-binomiali
nod* imbinare_simpla(nod* arbore1, nod* arbore2) {
    if (arbore1->valoare < arbore2->valoare)
        swap(arbore1, arbore2);
    arbore2->frate = arbore1->copil;
    arbore2->parinte = arbore1;
    arbore1->copil = arbore2;
    arbore1->grad++;
    return arbore1;
}

// Redimensionează arborii pentru a reduce numărul de coliziuni
void redimensioneaza_arbori(list<nod*> &v) {
    if (v.size() <= 1) return;
    list<nod*>::iterator prev, curent, urmator, temp;
    prev = curent = v.begin();
    curent++;
    urmator = curent;
    urmator++;
    while (curent != v.end()) {
        while ((urmator == v.end() || (*urmator)->grad > (*curent)->grad) &&
               curent != v.end() && (*prev)->grad == (*curent)->grad) {
            *curent = imbinare_simpla(*curent, *prev);
            temp = prev;
            if (prev == v.begin()) {
                prev++;
                curent++;
                if (urmator != v.end())
                    urmator++;
            }
            else prev--;
            v.erase(temp);
        }
        prev++;
        if (curent != v.end()) curent++;
        if (urmator != v.end()) urmator++;
    }
}

// Obține valoarea maximă dintr-un heap de arbori
int valoare_maxima(list<nod*> &v) {
    list<nod*>::iterator it = gaseste_nod_maxim(v);
    return (*it)->valoare;
}

// Merge două heapuri de arbori
void merge_heapuri(list<nod*> &v1, list<nod*> &v2, list<nod*> &v_noua) {
    list<nod*>::iterator i(v1.begin()), j(v2.begin());
    while (i != v1.end() && j != v2.end()) {
        if ((*i)->grad < (*j)->grad) {
            adauga_arbore(*i, v_noua);
            i++;
        }
        else {
            adauga_arbore(*j, v_noua);
            j++;
        }
    }
    while (i != v1.end()) {
        adauga_arbore(*i, v_noua);
        i++;
    }
    while (j != v2.end()) {
        adauga_arbore(*j, v_noua);
        j++;
    }
    v2.clear();
    redimensioneaza_arbori(v_noua);
}

// Adaugă un element în heapul de arbori
void adauga(int x, list<nod*> &v) {
    nod* n = new nod(x);
    list<nod*>::iterator it = v.begin(), it2 = (++it);
    it = v.begin();
    if ((*it)->grad != (*it2)->grad)
        v.push_front(n);
    else {
        int k1 = (*it)->valoare, k2 = (*it2)->valoare;
        if (x > k1 && x > k2) {
            (*it)->frate = (*it2);
            n->copil = (*it);
            (*it)->parinte = n;
            v.push_front(n);
        }
        else {
            nod *n1 = (*it), *n2 = (*it2);
            if (k1 < k2)
                swap(n1, n2);
            n1->copil = n;
            n->frate = n2;
            n->parinte = n1;
            v.push_front(n1);
        }
    }
}

// Șterge elementul maxim din heapul de arbori
void sterge(list<nod*> &v) {
    list<nod*>::iterator x = gaseste_nod_maxim(v);
    list<nod*> v_noua;
    sterge_radacina((*x), v_noua);
    v.erase(x);
    merge_heapuri(v, v_noua, v);
}

int main()
{
    int n, nr_operatii, op;
    fin >> n >> nr_operatii;
    vector<list<nod*>> h(n); ///Initializare
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
            adauga(val, h[id]);
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
            fout << valoare_maxima(h[id]) << "\n";
            auto start = chrono::high_resolution_clock::now();
            sterge(h[id]);
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
            merge_heapuri(h[heap1], h[heap2], h[heap1]);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed_seconds  = end - start;
            fout << elapsed_seconds.count() << " secunde" << '\n';
        }
    }
    return 0;
}

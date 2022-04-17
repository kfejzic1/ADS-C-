#include "DvostrukaLista.h"
#include <iostream>
#include <cmath>
#include <cstddef>

using namespace std;

template <typename tip>
tip dajMaksimum(const Lista<tip> &n){

    tip max{};
    
    Iterator<tip> it(dynamic_cast<const DvostrukaLista<tip>&>(n));
    while(it.trenutniCvor() != it.kraj()){
        if(max < it.trenutni())
            max = it.trenutni();
        
        it.sljedeci();
    }

    return max;
}

void testDodajIspred(Lista<int> *l){
    if(l->brojElemenata() == 0) //Ako je lista prazna potrebno dodati jos jedan element za ispravnost testa
        l->dodajIspred(5);
    l->pocetak();
    l->dodajIspred(1);
    l->prethodni();
    if(l->trenutni() == 1)
        std::cout << "OK\n";
    else
        std::cout << "NOK\n";

    delete l;
}

void testDodajIza(Lista<int>* l){
    if(l->brojElemenata() == 0)
        l->dodajIza(5);
    l->pocetak();
    l->dodajIza(1);
    l->sljedeci();
    if(l->trenutni() == 1)
        std::cout << "OK\n";
    else
        std::cout << "NOK\n";

    delete l;
}

void testBrojElemenata(Lista<int> *l){
    for(int i(1); i<5; i++)
        l->dodajIspred(i);
    if(l->brojElemenata() == 4)
        std::cout << "OK\n";
    else 
        std::cout << "NOK\n";
}

void testTrenutni(Lista<int> *l){
    for(int i(5); i<10; i++)
        l->dodajIspred(i);
    
    if(l->trenutni() == 5)
        std::cout << "OK\n";
    else
        std::cout << "NOK\n";

    delete l;
}

void testPrethodni(Lista<int> *l){
    for(int i(10); i<20; i++)
        l->dodajIspred(i);
    
    if(l->prethodni()){
        if(l->prethodni()){
            if(l->prethodni()){
                if(l->trenutni() == 17)
                    std::cout << "OK\n";
                else
                    std::cout << "NOK\n";
            }
        }
    }

    delete l;
}

void testSljedeci(Lista<int> *l){
    for(int i(10); i<20; i++)
        l->dodajIza(i);
    
    if(l->sljedeci()){
        if(l->sljedeci()){
            if(l->sljedeci()){
                if(l->trenutni() == 17)
                    std::cout << "OK\n";
                else
                    std::cout << "NOK\n";
            }
        }
    }

    delete l;
}

void testPocetak(Lista<int> *l){
    for(int i(5); i<10; i++)
        l->dodajIspred(i);
    l->pocetak();
    if(l->trenutni() == 6)
        std::cout << "OK\n";
    else 
        std::cout << "NOK\n";

    delete l;
}

void testKraj(Lista<int> *l){
    for(int i(5); i<10; i++)
        l->dodajIza(i);
    l->kraj();
    if(l->trenutni() == 6)
        std::cout << "OK\n";
    else 
        std::cout << "NOK\n";

    delete l;
}

void testObrisi(Lista<int> *l){
    for(int i(1); i<10; i++)
        l->dodajIza(i);

    l->obrisi();

    if(l->brojElemenata() == 8 && l->trenutni() == 9)
        std::cout << "OK\n";
    else 
        std::cout << "NOK\n";

    delete l;
}

void testOperatorPristupa(Lista<int> *l){
    for(int i(1); i<5; i++)
        l->dodajIza(i);
    
    if((*l)[0] == 1 && (*l)[1] == 4 && (*l)[2] == 3 && (*l)[3] == 2)
        std::cout << "OK\n";
    else
        std::cout << "NOK\n";

    delete l;
}

void testDajMaksimum(Lista<int> *l){
    for(int i(1); i<50; i++)
        l->dodajIspred(i);

    if(dajMaksimum(*l) == 49)
        std::cout << "OK\n";
    else
        std::cout << "NOK\n";

    delete l;
}

int main() {
    DvostrukaLista<int> *dvostruka = new DvostrukaLista<int>;
    testDodajIspred(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testDodajIza(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testBrojElemenata(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testTrenutni(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testPrethodni(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testSljedeci(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testPocetak(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testKraj(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testObrisi(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testOperatorPristupa(dvostruka);

    dvostruka = new DvostrukaLista<int>;
    testDajMaksimum(dvostruka);

    return 0;
}

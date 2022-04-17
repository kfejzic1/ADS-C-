#include <iostream>

#include "JednostrukaLista.h"
#include "NizLista.h"

using namespace std;

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
        
    delete l;
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

int main() {
    JednostrukaLista<int> *jednostruka = new JednostrukaLista<int>;
    NizLista<int> *nizLista = new NizLista<int>;
    testDodajIspred(jednostruka);
    testDodajIspred(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testDodajIza(jednostruka);
    testDodajIza(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testBrojElemenata(jednostruka);
    testBrojElemenata(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testTrenutni(jednostruka);
    testTrenutni(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testPrethodni(jednostruka);
    testPrethodni(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testSljedeci(jednostruka);
    testSljedeci(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testPocetak(jednostruka);
    testPocetak(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testKraj(jednostruka);
    testKraj(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testObrisi(jednostruka);
    testObrisi(nizLista);

    jednostruka = new JednostrukaLista<int>;
    nizLista = new NizLista<int>;
    testOperatorPristupa(jednostruka);
    testOperatorPristupa(nizLista);

    return 0;
}

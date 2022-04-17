#include "DvostraniRed.h"
#include <iostream>

void popuniDvostraniRed(DvostraniRed<int> *dr, bool vrh=true){
    if(vrh){
        for(int i(5); i<=10; i++)
            dr->staviNaVrh(i);
    }
    else{
        for(int i(5); i<=10; i++)
            dr->staviNaCelo(i);
    }
}

void testStaviNaVrh(DvostraniRed<int> *dr){
    popuniDvostraniRed(dr);
    if(dr->dajString() == "5 6 7 8 9 10 ")
        std::cout << "OK\n";
    else 
        std::cout << "NOK\n";
    delete dr;
}

void testStaviNaCelo(DvostraniRed<int> *dr){
    popuniDvostraniRed(dr, false);
    if(dr->dajString() == "10 9 8 7 6 5 ")
        std::cout << "OK\n";
    else
        std::cout << "NOK\n";
    delete dr;
}

void testSkiniSaCela(DvostraniRed<int> *dr){
    popuniDvostraniRed(dr);
    
    int niz[6] = {dr->skiniSaCela(), dr->skiniSaCela(), dr->skiniSaCela(), dr->skiniSaCela(), dr->skiniSaCela(), dr->skiniSaCela()};
    int k=5;
    for(int i(0); i<5; i++){
        if(niz[i]!=k){
            delete dr;
            std::cout << "NOK\n";
            return;
        }
        k++;
    }

    try{
        dr->skiniSaCela();
    }catch(...){
        std:cout << "OK\n";
    }

    delete dr;
}

void testSkiniSaVrha(DvostraniRed<int> *dr){
    popuniDvostraniRed(dr);

    int niz[6] = {dr->skiniSaVrha(), dr->skiniSaVrha(), dr->skiniSaVrha(), dr->skiniSaVrha(), dr->skiniSaVrha(), dr->skiniSaVrha()};
    int k=10;
    for(int i(0); i<5; i++){
        if(niz[i]!=k){
            delete dr;
            std::cout << "NOK\n";
            return;
        }
        k--;
    }

    try{
        dr->skiniSaCela();
    }catch(...){
        std:cout << "OK\n";
    }

    delete dr;
}

void testBrisi(DvostraniRed<int> *dr){
    popuniDvostraniRed(dr);
    dr->brisi();

    if(dr->brojElemenata() == 0)
        std::cout << "OK\n";
    else 
        std::cout << "NOK\n";
    
    delete dr;
}

void testCelo(DvostraniRed<int> *dr){
    popuniDvostraniRed(dr, false);

    if(dr->celo() == 10){
        dr->brisi();
        try {
            dr->celo();
        } catch (...) {
            std::cout << "OK\n";
        }
    }else
        std::cout << "NOK\n";
    
    delete dr;
}

void testVrh(DvostraniRed<int> *dr){
    popuniDvostraniRed(dr);

    if(dr->vrh() == 10){
        dr->brisi();
        try {
            dr->vrh();
        } catch (...) {
            std::cout << "OK\n";
        }
    }else
        std::cout << "NOK\n";
    
    delete dr;
}

int main() {
    DvostraniRed<int> *dr = new DvostraniRed<int>;
    testStaviNaVrh(dr);

    dr = new DvostraniRed<int>;
    testStaviNaCelo(dr);

    dr = new DvostraniRed<int>;
    testSkiniSaVrha(dr);

    dr = new DvostraniRed<int>;
    testSkiniSaCela(dr);

    dr = new DvostraniRed<int>;
    testBrisi(dr);

    dr = new DvostraniRed<int>;
    testCelo(dr);

    dr = new DvostraniRed<int>;
    testVrh(dr);


    return 0;
}

#include "Stek.h"
#include <iostream>

using namespace std;

void ubaciElementeUStek(Stek<int> *s) {
  for (int i(1); i <= 10; i++)
    s->stavi(i);

    //Ubacuje elemente:
    // 10 9 8 7 6 6 5 4 3 2 1
    // Vrh je 10 - zadnji unesen
}

void testBrojElemenata(Stek<int> *s){
    ubaciElementeUStek(s);

    if(s->brojElemenata() == 10){
        std::cout << "OK\n";
        return;
    }
    std::cout << "NOK\n";

    delete s;
}

void testStavi(Stek<int> *s){
    ubaciElementeUStek(s);

    if(s->brojElemenata() == 10 && s->vrh() == 10){
        std::cout << "OK\n";
        return ;
    }   
    std::cout << "NOK\n";

    delete s;
}

void testBrisi(Stek<int> *s){
    ubaciElementeUStek(s);

    s->brisi();
    if(s->brojElemenata() == 0){
        try{
            s->vrh(); //Stavljen dodatni uslov jer je moguce samo promijeniti broj elemenata
        }catch(...){
            std::cout << "OK\n";
            return;
        }
    }
    std::cout << "NOK\n";
    
    delete s;
}

void testSkini(Stek<int> *s){
    ubaciElementeUStek(s);

    if(s->skini() == 10){
        if(s->brojElemenata() == 9 && s->vrh() == 9){
            s->brisi();

            try{
                s->skini();
            }catch(...){
                std::cout << "OK\n";
                return;
            }
        }
    }
    std::cout << "NOK\n";

    delete s;
}

void testVrh(Stek<int> *s){
    ubaciElementeUStek(s);

    if(s->vrh() == 10 && s->brojElemenata() == 10){
        s->skini();
        s->skini();
        s->skini();
        if(s->vrh() == 7){  //Testira tacnost funkcije
            s->brisi();
            try{    //Testira izuzetak
                s->vrh();
            }catch(...){
                std::cout << "OK\n";
                return;
            }
        }
    }
    std::cout << "NOK\n";

    delete s;
}

int main() {
    Stek<int> *s = new Stek<int>;
    testBrojElemenata(s);

    s = new Stek<int>;
    testStavi(s);

    s = new Stek<int>;
    testBrisi(s);

    s = new Stek<int>;
    testSkini(s);

    s = new Stek<int>;
    testVrh(s);

  return 0;
}

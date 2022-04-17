#include <iostream>
#include <vector>
#include "Stek.h"

using namespace std;

int binarnaPretraga(vector<int> v, int lijevaParticija, int desnaParticija, int element){
    if(desnaParticija >= lijevaParticija){
        int sredina = lijevaParticija + (desnaParticija-lijevaParticija)/2;

        if(v.at(sredina) == element)
            return sredina;
        
        if(v.at(sredina) > element) //Element se nalazi u lijevoj particiji
            return binarnaPretraga(v, lijevaParticija, sredina-1, element);
        
        return binarnaPretraga(v, sredina+1, desnaParticija, element);  //Element se nalazi u desnoj particiji
    }

    return -1;
}

void pretraga(Stek<vector<int>> &s, int trazeni){
    Stek<vector<int>> tempStack;
    bool pronadjen = false;

    int brojIzbacenih = 0;
    int velicina = s.brojElemenata();
    while(s.brojElemenata() != 0){
        tempStack.stavi(s.vrh());

        vector<int> vektor = s.vrh();

        int indeks = binarnaPretraga(vektor, 0, vektor.size()-1, trazeni);
        if(indeks != -1){
            pronadjen = true;
            std::cout << indeks << " " << velicina - brojIzbacenih - 1;
        }
            
        s.skini();
        brojIzbacenih++;
    }

    while(tempStack.brojElemenata() != 0){
        s.stavi(tempStack.skini());
    }

    if(!pronadjen)
        std::cout << "Nema elementa\n";
}

void test1(){
    Stek<vector<int>> s;
    vector<int> a = {1,2,3,4};
    vector<int> b = {0,2,4,6,8,10};
    vector<int> c = {1,3,5,7,9,11};

    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    
    pretraga(s, 0);
    //Treba ispisati 0 1 - OK
}

void test2(){
    Stek<vector<int>> s;
    vector<int> a = {1,2,3,4,5,6};
    vector<int> b = {0,2,4,6,8,10,12,14,16};
    vector<int> c = {1,3,5,7,9,11,13,15};
    vector<int> d = {100, 200, 300, 400, 500};
    
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);

    pretraga(s, 7);
    //Treba ispisati 3 2 - OK
}

void test3(){
    Stek<vector<int>> s;
    vector<int> a = {1,2,3,4,5,6,7,8,9,10};
    vector<int> b = {0,2,4,6,8,10,12,14,16,18,20,22,24};
    vector<int> c = {1,3,5,7,9,11,13,15,17,19,21};
    vector<int> d = {100, 200, 300, 400, 500, 600, 700, 800, 900};
    
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);

    pretraga(s, 50);
    //Treba ispisati "Nema elementa" - OK
}

void test4(){
    Stek<vector<int>> s;
    vector<int> a = {1,2,3,4,5,6,7,8,9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    vector<int> b = {0,2,4,6,8,10,12,14,16,18,20,22,24, 26, 28, 30};
    vector<int> c = {1,3,5,7,9,11,13,15,17,19,21, 23, 25, 27, 29, 31};
    vector<int> d = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300};
    vector<int> e = {-100,-50,-49,-40,0};
    vector<int> f = {99, 100, 101, 102, 103, 104, 105, 106, 107};
    
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    s.stavi(e);
    s.stavi(f);

    pretraga(s, 103);
    //Treba ispisati 4 5 - OK
}

int main() {
    test1();
    std::cout << std::endl;
    test2();
    std::cout << std::endl;
    test3();
    test4();
    return 0;
}

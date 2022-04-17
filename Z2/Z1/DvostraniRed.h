#include <iostream>
#include <string>

using namespace std;

template <typename tip>
class DvostraniRed{
private:
    struct Cvor{
        tip el;
        Cvor *sljedeci;
        Cvor *prethodni;
        static Cvor* dajCvor(tip element){
            Cvor* novi = new Cvor;
            novi->el = element;
            novi->prethodni = novi->sljedeci = nullptr;

            return novi;
        }
    };
    Cvor *pocetak;  //Pokazivac na pocetak reda
    Cvor *kraj;     //Pokazivac na kraj reda
    int vel;
    void obrisiMemoriju();
    void kopiraj(const DvostraniRed<tip> &r);
public:
    DvostraniRed() : pocetak(nullptr), kraj(nullptr), vel(0){};
    DvostraniRed(const DvostraniRed<tip> &r) : pocetak(nullptr), kraj(nullptr), vel(0) {kopiraj(r);};
    ~DvostraniRed() {obrisiMemoriju();};
    DvostraniRed& operator= (const DvostraniRed<tip> &r) {if(&r!=this){obrisiMemoriju(); kopiraj(r);} return *this;};
    void brisi() {obrisiMemoriju();};
    void staviNaVrh(const tip& element);
    void staviNaCelo(const tip& element);
    tip skiniSaCela();
    tip skiniSaVrha();
    tip& celo() {if(vel==0) throw "Red je prazan!\n"; return pocetak->el;};
    tip& vrh() {if(vel==0) throw "Red je prazan!\n"; return kraj->el;};
    int brojElemenata(){return vel;};
    std::string dajString() const;
};

template <typename tip>
void DvostraniRed<tip>::kopiraj(const DvostraniRed<tip> &r){
    Cvor* p(r.pocetak);
    while(p!=nullptr){  //Provjeriti da li staviti p!=nullptr ili p!=r.kraj
        staviNaVrh(p->el);
        p = p->sljedeci;
    }
}

template <typename tip>
void DvostraniRed<tip>::obrisiMemoriju(){
    while(vel!=0)
        skiniSaVrha();
}

template <typename tip>
void DvostraniRed<tip>::staviNaCelo(const tip &element){
    Cvor* novi = Cvor::dajCvor(element);
    if(vel == 0)  //Dodaje se prvi element
        pocetak = kraj = novi;
    else{
        novi->sljedeci = pocetak;
        pocetak->prethodni = novi;
        pocetak = novi;
    }
    vel++;
}

template <typename tip>
void DvostraniRed<tip>::staviNaVrh(const tip &element){
    Cvor* novi = Cvor::dajCvor(element);
    if(vel == 0)  //Dodaje se prvi element
        pocetak = kraj = novi;
    else{
        novi->prethodni = kraj;
        kraj->sljedeci = novi;
        kraj = novi;
    }
    vel++;
}

//CELO - POCETAK
//VRH - KRAJ

template <typename tip>
tip DvostraniRed<tip>::skiniSaCela(){
    if(vel == 0)
        throw "Red je prazan!\n";
    
    tip povratni_el(pocetak->el);

    Cvor *p = pocetak;
    pocetak = pocetak->sljedeci;

    if(vel == 1)    //U redu se nalazi samo jedan element
        pocetak = kraj = nullptr;
    else
        pocetak->prethodni = nullptr;
    
    delete p;
    p = nullptr;

    vel--;
    
    return povratni_el;
}

template <typename tip>
tip DvostraniRed<tip>::skiniSaVrha(){
    if(vel == 0)
        throw "Red je prazan!\n";
    
    tip povratni_el(kraj->el);

    Cvor *p = kraj;
    kraj = kraj->prethodni;

    if(vel == 1) //U redu se nalazi samo jedan element
        pocetak = kraj = nullptr;
    else
        kraj->sljedeci = nullptr;
    
    delete p;
    p = nullptr;

    vel--;
    
    return povratni_el;
}



template <typename tip>
std::string DvostraniRed<tip>::dajString() const{
    Cvor *p(pocetak);

    std::string s;
    while(p!=nullptr){
        s += (std::to_string(p->el) + " ");
        p = p->sljedeci;
    }

    return s;
}

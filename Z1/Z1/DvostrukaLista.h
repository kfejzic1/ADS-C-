#include "Lista.h"
#include <cmath>
#include <cstddef>
#include <iostream>

using namespace std;

template<typename tip>
class Iterator;

template<typename tip>
class DvostrukaLista : public Lista<tip>
{
private:
    struct Cvor{
        tip el;
        Cvor *sljedeci;
        Cvor *prethodni;
        Cvor (const tip &element, Cvor* prevp, Cvor* nextp=nullptr) : el(element), prethodni(prevp), sljedeci(nextp){
            if(prethodni != nullptr) prethodni->sljedeci = this;
            if(sljedeci != nullptr) sljedeci->prethodni = this;
        };
        Cvor(Cvor* prevp=nullptr, Cvor *nextp=nullptr) : prethodni(prevp), sljedeci(nextp){
            if(prethodni != nullptr) prethodni->sljedeci = this;
            if(sljedeci != nullptr) sljedeci->prethodni = this;
        };
    };
    int duzina;
    int indeks;
    Cvor *poc;
    Cvor *zadnji;
    Cvor *tekuci;           //Pokazivac na prethodnika trenutnog
    void ocistiMemoriju();
    void inicijaliziraj();
    void kopiraj(const DvostrukaLista<tip> &l);
    void daLiJeListaPrazna() const{
        if(duzina==0) 
            throw "Greska, lista je prazna!\n";
    }
public:
    DvostrukaLista(){inicijaliziraj();};
    DvostrukaLista(const DvostrukaLista &l) {kopiraj(l);};
    ~DvostrukaLista() {ocistiMemoriju();};
    DvostrukaLista<tip>& operator=(const DvostrukaLista &l);
    int brojElemenata() const override {return duzina;};
    tip& trenutni() override;
    const tip trenutni() const override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const tip &el) override;
    void dodajIza(const tip &el) override;
    tip& operator[](int i) override;
    const tip operator[](int i) const override;
    void ispisi() const override;
    int dajIndeks() const {return indeks;}
    friend class Iterator<tip>;
};

template <typename tip>
void DvostrukaLista<tip>::ocistiMemoriju(){
    while(poc != zadnji){
        tekuci = poc;
        poc = poc->sljedeci;
        delete tekuci;
    }
    delete zadnji;
    zadnji = nullptr; tekuci = nullptr;
}

template <typename tip>
void DvostrukaLista<tip>::inicijaliziraj(){
    tekuci = poc = new Cvor(nullptr, nullptr);
    poc->sljedeci = zadnji = new Cvor(poc, nullptr);
    duzina = 0; indeks = -1;
}

template <typename tip>
void DvostrukaLista<tip>::kopiraj(const DvostrukaLista<tip> &l){
    inicijaliziraj();

    Cvor *temp = l.poc->sljedeci;
    Cvor *tekuci_temp;
    bool prviPut = false;
    do {
        new Cvor(temp->el, zadnji->prethodni, zadnji);

        if (temp->el == l.trenutni())
            tekuci = zadnji->prethodni->prethodni;

        temp = temp->sljedeci;
    } while (temp != l.zadnji);

    indeks = l.indeks;
    duzina = l.duzina;
}

template <typename tip>
DvostrukaLista<tip>& DvostrukaLista<tip>::operator=(const DvostrukaLista<tip> &l){
    if(&l!=this){
        ocistiMemoriju();
        kopiraj(l);
    }
    return *this;
}

template <typename tip>
tip& DvostrukaLista<tip>::trenutni(){
    daLiJeListaPrazna(); 
    if(tekuci==zadnji && duzina!=0)
        return tekuci->prethodni->el;
    return tekuci->sljedeci->el;
}

template <typename tip>
const tip DvostrukaLista<tip>::trenutni() const{
    daLiJeListaPrazna(); 
    if(tekuci==zadnji && duzina!=0)
        return tekuci->prethodni->el;
    return tekuci->sljedeci->el;
}

template <typename tip>
void DvostrukaLista<tip>::pocetak(){
    this->daLiJeListaPrazna(); 
    tekuci = poc; 
    indeks = 0;
}

template <typename tip>
void DvostrukaLista<tip>::kraj(){
    this->daLiJeListaPrazna(); 
    tekuci=zadnji; 
    indeks = duzina-1;
}

template <typename tip>
bool DvostrukaLista<tip>::prethodni(){
    daLiJeListaPrazna();
    if(tekuci != poc){
        tekuci = tekuci->prethodni;
        indeks--;
        return true;
    }
    return false;
}

template <typename tip>
bool DvostrukaLista<tip>::sljedeci(){
    daLiJeListaPrazna();
    if(tekuci->sljedeci != zadnji->prethodni){
        tekuci = tekuci->sljedeci;
        indeks++;
        return true;
    }
    return false;
}

template <typename tip>
void DvostrukaLista<tip>::obrisi(){
    daLiJeListaPrazna();
    
    Cvor* temp = tekuci->sljedeci;      //Trenutni element, potrebno ga je obrisati
    temp->sljedeci->prethodni = tekuci;
    tekuci->sljedeci = temp->sljedeci;
    delete temp;

    if(tekuci->sljedeci == zadnji)
        prethodni();

    duzina--;
}

template <typename tip>
void DvostrukaLista<tip>::dodajIspred(const tip &el){
    new Cvor(el, tekuci, tekuci->sljedeci);

    if(duzina!=0)
        sljedeci();
    else
        indeks++;

    duzina++;
}

template <typename tip>
void DvostrukaLista<tip>::dodajIza(const tip &el){
    if(duzina==0){
        new Cvor(el, tekuci, tekuci->sljedeci);
        indeks++;
    }else{
        new Cvor(el, tekuci->sljedeci, tekuci->sljedeci->sljedeci);
    }

    duzina++;
}

template <typename tip>
tip& DvostrukaLista<tip>::operator[](int i){
    if((i<0) || (i>=duzina))
        throw "Indeks van opsega!\n";

    int ld = indeks;
    int dd = duzina-indeks-1;
    int k = 0;
    Cvor* p = tekuci->sljedeci;

    if(i<=abs(i-ld) && i<=(ld+dd)/2){
        k = i;
        p = poc->sljedeci;
    }
    else if(abs(i-ld) < ld+dd-i)
        k = i-ld;
    else{
        k = i-ld-dd;
        p = zadnji->prethodni;
    }
    
    if(k<0)
        for(int i=0; i<abs(k); i++)
            p = p->prethodni;
    else
        for(int i=0; i<k; i++)
            p = p->sljedeci;

    return p->el;
}

template <typename tip>
const tip DvostrukaLista<tip>::operator[](int i) const{
   if((i<0) || (i>=duzina))
        throw "Indeks van opsega!\n";

    int ld = indeks;
    int dd = duzina-indeks-1;
    int k = 0;
    Cvor* p = tekuci;

    if(i<=abs(i-ld) && i<=(ld+dd)/2){
        k = i;
        p = poc;
    }
    else if(abs(i-ld) <= ld+dd-i)
        k = i-ld;
    else{
        k = i-ld-dd-1;
        p = zadnji;
    }
    
    if(k<0)
        for(int i=0; i<abs(k); i++)
            p = p->prethodni;
    else
        for(int i=0; i<k; i++)
            p = p->sljedeci;

    return p->sljedeci->el;
}

template <typename tip>
void DvostrukaLista<tip>::ispisi() const{
    daLiJeListaPrazna();

    Cvor* temp = poc->sljedeci;
    while(temp!=zadnji){
        std::cout << temp->el << " ";
        temp = temp->sljedeci;
    }
    std::cout << std::endl;
}

template <typename tip>
class Iterator{
private:
    const DvostrukaLista<tip> *l;
    typename DvostrukaLista<tip>::Cvor* trenutniLista;
public:
    Iterator(const DvostrukaLista<tip> &parametarLista) : l(&parametarLista), trenutniLista(parametarLista.poc->sljedeci){};
    tip trenutni() const {return trenutniLista->el;};
    void prethodni() {l->prethodni();};
    void sljedeci() {trenutniLista = trenutniLista->sljedeci;};
    typename DvostrukaLista<tip>::Cvor* pocetak() const {return l->poc;};
    typename DvostrukaLista<tip>::Cvor* kraj() const {return l->zadnji;};
    typename DvostrukaLista<tip>::Cvor* trenutniCvor() const{return trenutniLista;};
};

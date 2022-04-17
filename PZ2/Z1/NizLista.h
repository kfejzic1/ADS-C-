#include<iostream>
#include <locale>
#include "Lista.h"

template <typename tip>
class NizLista : public Lista<tip>{
protected:
    tip** el;
    int duzina;
    int kapacitet;
    int tekuci;
    void daLiJeListaPrazna(){if(duzina==0) throw "Lista je prazna!\n";}
    void ocistiMemoriju() {
        for(int i=0; i<this->duzina; i++){
            delete el[i];
            el[i] = nullptr;
        }
        delete[] el; 
        el=nullptr;
    }
public:
    NizLista(int vel = 5): kapacitet(vel), duzina(0), tekuci(0){el = new tip*[kapacitet]{};};
    NizLista(const NizLista<tip> &l): tekuci(l.tekuci), kapacitet(l.kapacitet), duzina(l.duzina), el(new tip*[l.kapacitet]{}){
        for(int i=0; i<l.duzina; i++)
            el[i] = new tip(*l.el[i]);
    };
    NizLista(NizLista<tip> &&l) : kapacitet(l.kapacitet), tekuci(l.tekuci), duzina(l.duzina), el(l.el){l.el = nullptr;};
    ~NizLista(){ocistiMemoriju();};
    int brojElemenata() const override {return duzina;};
    const tip trenutni() const override {return *el[tekuci];};
    tip& trenutni() override {daLiJeListaPrazna(); return *el[tekuci];};
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override {daLiJeListaPrazna(); tekuci = 0;};
    void kraj() override {daLiJeListaPrazna(); tekuci = duzina-1;};
    void obrisi() override;
    void dodajIspred(const tip& temp) override;
    void dodajIza(const tip& temp) override;
    const tip operator[](int i) const override;
    tip &operator [](int i) override;
    NizLista<tip> &operator =(const NizLista<tip> &l);
    NizLista<tip> &operator =(NizLista<tip> &&l);
    int dajTekuci () const {return tekuci;};
    int dajDuzinu () const {return duzina;};
    void ispisi() const override {
        for(int i=0; i<duzina; i++)
            std::cout << "Element (" << i << "): " << *el[i] << std::endl;
    }
};

template <typename tip>
NizLista<tip>& NizLista<tip>::operator=(const NizLista<tip> &l){
    tip** temp = new tip*[l.kapacitet]{};
    
    for(int i=0; i<l.duzina; i++)
        temp[i] = new tip(*l.el[i]);
    ocistiMemoriju();
    duzina = l.duzina;
    kapacitet = l.kapacitet;
    tekuci = l.tekuci;
    
    el = temp;

    return *this;
}

template <typename tip>
NizLista<tip>& NizLista<tip>::operator=(NizLista<tip> &&l){
    if(&l != this){
        ocistiMemoriju();
        el = l.el;
        duzina = l.duzina;
        kapacitet = l.kapacitet;
        tekuci = l.tekuci;
        l.el = nullptr;
    }
}

template <typename tip>
bool NizLista<tip>::prethodni(){
    daLiJeListaPrazna();
    if(tekuci == 0)
        return false;
    tekuci--;
    return true;
}

template <typename tip>
bool NizLista<tip>::sljedeci(){
    daLiJeListaPrazna();
    if(tekuci == duzina-1)
        return false;
    tekuci++;
    return true;
}

template <typename tip>
void NizLista<tip>::obrisi(){
    daLiJeListaPrazna();

    delete el[tekuci];
    duzina--;

    if(tekuci == duzina)
        tekuci--;
    else{
        for(int i = tekuci; i<duzina; i++)
            el[i] = el[i+1];
    }

    el[duzina] = nullptr;

    if(duzina == 0)
        tekuci = 0;
}

template <typename tip>
void NizLista<tip>::dodajIspred(const tip& temp){
    if(duzina == 0){
        el[0] = new tip(temp);
    }else if(duzina < kapacitet){
        for(int i = duzina; i > tekuci; i--)
            el[i] = el[i-1];
        el[tekuci++] = new tip(temp);
    }else{
        kapacitet*=2;
        tip** pom = new tip*[kapacitet]{};
        for(int i = 0; i < tekuci; i++)
            pom[i] = el[i];

        pom[tekuci++] = new tip(temp);

        for(int i = tekuci; i < duzina+1; i++)
            pom[i] = el[i-1];

        delete[] el;
        el = pom;
    }
    duzina++;
}


template <typename tip>
void NizLista<tip>::dodajIza(const tip& temp){
    if(duzina == 0){
        el[0] = new tip(temp);
    }else if(duzina < kapacitet){
        for(int i = duzina; i > tekuci; i--)
            el[i] = el[i-1];
        el[tekuci+1] = new tip(temp);
    }else{
        kapacitet*=2;
        tip** pom = new tip*[kapacitet]{};
        for(int i = 0; i <= tekuci; i++)
            pom[i] = el[i];

        pom[tekuci+1] = new tip(temp);

        for(int i = tekuci+2; i < duzina+1; i++)
            pom[i] = el[i-1];

        delete[] el;
        el = pom;
    }
    duzina++;
}

template <typename tip>
tip& NizLista<tip>::operator[](int i){
    if(i < 0 || i >= duzina)
        throw "Nesipravan indeks!";
    return *(el[i]);
}

template <typename tip>
const tip NizLista<tip>::operator[](int i) const{
    if(i < 0 || i >= duzina)
        throw "Nesipravan indeks!";
    return *(el[i]);
}

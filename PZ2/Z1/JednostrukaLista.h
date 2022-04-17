#include "Lista.h"
#include <iostream>

template <typename tip>
class JednostrukaLista : public Lista<tip>{
private:
    struct Cvor{
        tip el;
        Cvor *sljedeci;
        Cvor (const tip &el, Cvor *sljedeci) : el(el), sljedeci(sljedeci){};
    };
    int duzina;
    Cvor *poc;
    Cvor *end;
    Cvor *tekuci;
    void kopiraj(const JednostrukaLista<tip> &l);
    void obrisiMemoriju();
    void daLiJeListaPrazna() {if(duzina == 0) throw "Lista je prazna!\n";}
public:
    JednostrukaLista() : poc(nullptr), end(nullptr), tekuci(nullptr), duzina(0){};
    JednostrukaLista(const JednostrukaLista &l) : duzina(l.duzina){kopiraj(l);};
    JednostrukaLista(JednostrukaLista &&l) : duzina(l.duzina), poc(l.poc), end(l.end), tekuci(l.tekuci){l.poc = l.end = l.tekuci = nullptr; l.duzina = 0;};
    ~JednostrukaLista(){obrisiMemoriju();};
    JednostrukaLista<tip> &operator=(const JednostrukaLista &l){obrisiMemoriju(); kopiraj(l);};
    JednostrukaLista<tip> &operator=(JednostrukaLista &&l);
    int brojElemenata() const override {return duzina;};
    tip &trenutni() override {return tekuci->el;};
    const tip trenutni() const override {return tekuci->el;};
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override {daLiJeListaPrazna(); tekuci = poc;};
    void kraj() override {daLiJeListaPrazna(); tekuci = end;};
    void obrisi() override;
    void dodajIspred(const tip &el) override;
    void dodajIza(const tip &el) override;
    tip &operator[](int i) override;
    const tip operator[](int i) const override;
    void ispisi() const override{
        int i = 0;
        Cvor* it = poc;
        while (it != nullptr){
            std::cout << "Element (" << i++ << "): " << it->el << std::endl;
            it = it->sljedeci;
        }
    }
    tip dajTekuci()const {return tekuci->el;};
    tip dajPrvi() const {return poc->el;};
    tip dajZadnji() const {return end->el;};
    int dajDuzinu() const {return duzina;}
};

template <typename tip>
void JednostrukaLista<tip>::kopiraj(const JednostrukaLista<tip> &l){
    Cvor* it = l.poc;
    poc = end = new Cvor(l.poc->el, nullptr);
    it = it->sljedeci;

    do{
        //Dodajemo elemente na kraj liste
        end->sljedeci = new Cvor(it->el, nullptr);

        if(end == l.tekuci)     //Kopiramo tekuci element
            tekuci = end;

        end = end->sljedeci;    //Prebacujemo na iduci element
        it = it->sljedeci;
    }while(it != l.end);
    duzina = l.duzina;
}

template <typename tip>
void JednostrukaLista<tip>::obrisiMemoriju(){
    Cvor* it = poc;
    while(it != nullptr){
        tekuci = it;
        it = it->sljedeci;
        delete tekuci;
    }
}

template <typename tip>
JednostrukaLista<tip>& JednostrukaLista<tip>::operator=(JednostrukaLista &&l){
    if(&l != this){
        obrisiMemoriju();
        poc = l.poc;
        end = l.end;
        tekuci = l.tekuci;
        duzina = l.duzina;

        l.poc = l.end = l.tekuci = nullptr;
        l.duzina = 0;
    }
}

template <typename tip>
bool JednostrukaLista<tip>::prethodni(){
    daLiJeListaPrazna();
    if(poc == tekuci)
        return false;
    
    Cvor *it = poc;

    while(it != nullptr){
        if(it->sljedeci == tekuci){
            tekuci = it;
            break;
        }
        it = it->sljedeci;
    }

    return true;
}

template <typename tip>
bool JednostrukaLista<tip>::sljedeci(){
    daLiJeListaPrazna();
    if(tekuci->sljedeci == nullptr)
        return false;

    tekuci = tekuci->sljedeci;
    return true;
}

template <typename tip>
void JednostrukaLista<tip>::obrisi(){
    daLiJeListaPrazna();

    if(tekuci == poc){
        //Prvi element
        poc = tekuci->sljedeci;
        delete tekuci;
        tekuci = poc;
    }else{
        Cvor* temp = tekuci;
        prethodni();
        tekuci->sljedeci = temp->sljedeci;

        if(end == temp)
            end = tekuci;
        else
            tekuci = tekuci->sljedeci;
        delete temp;
    }

    duzina--;
    if(duzina == 0)
        end = nullptr;
}

template <typename tip>
void JednostrukaLista<tip>::dodajIspred(const tip &el){
    Cvor* temp = new Cvor(el, nullptr);
    if(duzina == 0){
        poc = temp;
        tekuci = temp;
        end = temp;
    }else{
        if(tekuci == poc){
            poc = temp;
            poc->sljedeci = tekuci;
        }else{
            Cvor *it = tekuci;
            prethodni();
            tekuci->sljedeci = temp;
            temp->sljedeci = it;

            //Vracamo tekuci
            tekuci = tekuci->sljedeci;
            tekuci = tekuci->sljedeci;
        }
    }
    duzina++;
}

template <typename tip>
void JednostrukaLista<tip>::dodajIza(const tip&el){
    Cvor* temp = new Cvor(el, nullptr);
    if(duzina == 0){
        poc = temp;
        tekuci = temp;
        end = temp;
    }else{
        temp->sljedeci = tekuci->sljedeci;
        tekuci->sljedeci = temp;

        if(temp->sljedeci == nullptr)
            end = temp;
    }
    duzina++;
}

template <typename tip>
tip& JednostrukaLista<tip>::operator[](int i){
    if(i < 0 || i>=duzina)
        throw "Neispravan koeficijent!";
    Cvor* it = poc;
    int j = 0;
    while (it != nullptr){
        if(i == j)
            return it->el;
        j++;
        it = it->sljedeci;
    }
}

template <typename tip>
const tip JednostrukaLista<tip>::operator[](int i) const{
    if(i < 0 || i>=duzina)
        throw "Neispravan koeficijent!";
    Cvor* it = poc;
    int j = 0;
    while (it != nullptr){
        if(i == j)
            return it->el;
        j++;
        it = it->sljedeci;
    }
}

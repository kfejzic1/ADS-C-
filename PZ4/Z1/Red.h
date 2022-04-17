#include <iostream>
#include <string>

template <typename tip>
class Red{
private:
    struct Cvor{
        tip el;
        Cvor *sljedeci;
        Cvor(const tip &el, Cvor* sljedeci): el(el), sljedeci(sljedeci){};
    };
    Cvor *pocetak;  //Pokazivac na pocetak reda
    Cvor *kraj;     //Pokazivac na kraj reda
    int vel;
    void obrisiMemoriju();
    void kopiraj(const Red<tip> &r);
public:
    Red() : pocetak(nullptr), kraj(nullptr), vel(0){};
    Red(const Red<tip> &r) : pocetak(nullptr), kraj(nullptr), vel(0) {kopiraj(r);};
    ~Red() {obrisiMemoriju();};
    Red& operator= (const Red<tip> &r) {if(&r!=this){obrisiMemoriju(); kopiraj(r);} return *this;};
    void brisi() {obrisiMemoriju();};
    void stavi(const tip& element);
    tip skini();
    tip& celo() {if(vel==0) throw "Red je prazan!\n"; return pocetak->el;};
    int brojElemenata(){return vel;};
    std::string dajString() const;
};

template <typename tip>
void Red<tip>::kopiraj(const Red<tip> &r){
    Cvor* p(r.pocetak);
    while(p!=nullptr){
        stavi(p->el);
        p = p->sljedeci;
    }
}

template <typename tip>
void Red<tip>::obrisiMemoriju(){
    while(vel!=0)
        skini();
}

template <typename tip>
void Red<tip>::stavi(const tip &element){
    Cvor* novi(new Cvor(element, nullptr));
    if(pocetak == nullptr)  //Dodaje se prvi element
        pocetak = kraj = novi;
    else{
        kraj->sljedeci = novi;
        kraj = kraj->sljedeci;
    }
    vel++;
}

template <typename tip>
tip Red<tip>::skini(){
    if(vel == 0)
        throw "Red je prazan!\n";
    
    tip povratni_el(pocetak->el);

    Cvor *p = pocetak;
    if(pocetak == kraj) //U redu se nalazi samo jedan element
        pocetak = kraj = nullptr;
    else
        pocetak = pocetak->sljedeci;
    
    delete p;
    vel--;
    
    return povratni_el;
}

template <typename tip>
std::string Red<tip>::dajString() const{
    Cvor *p(pocetak);

    std::string s;
    while(p!=nullptr){
        s += (std::to_string(p->el) + " ");
        p = p->sljedeci;
    }

    return s;
}

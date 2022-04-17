#include <ctime>
#include <iostream>
using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
public:
    Mapa(){};
    virtual TipVrijednosti& operator[] (const TipKljuca &kljuc) = 0;
    virtual TipVrijednosti operator[] (const TipKljuca &kljuc) const = 0;
    virtual int brojElemenata() const=0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &x) = 0;
    virtual ~Mapa(){};
};


template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
private:    
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        int balans = 0;     //Dodan balans
        Cvor *lijevo, *desno, *roditelj;
        Cvor () { lijevo = desno = roditelj = nullptr; }
        Cvor (const TipKljuca &kljuc, const TipVrijednosti &vrijednost, Cvor *l = nullptr, Cvor *d = nullptr, Cvor *r = nullptr){
            this->kljuc = kljuc;
            this->vrijednost = vrijednost;
            lijevo = l;
            desno = d;
            roditelj = r;
        }
    };

    Cvor *korijen;
    int brojEl;
    
    void kopirajStablo(Cvor *&c, Cvor *c2, Cvor *root){ //DA
        if(c2 != nullptr){
            c = new Cvor(c2->kljuc, c2->vrijednost, nullptr, nullptr, root);
            kopirajStablo(c->lijevo, c2->lijevo, c);
            kopirajStablo(c->desno, c2->desno, c);
            brojEl++;
        }
    }
    void obrisiStablo(Cvor *root) { //DA
        if(root == nullptr) return;
        obrisiStablo(root->lijevo);
        obrisiStablo(root->desno);
        delete root;
    }
public:
    AVLStabloMapa() : korijen(nullptr), brojEl(0) {};
    AVLStabloMapa(const AVLStabloMapa &bsm) : korijen(nullptr), brojEl(0) {
        kopirajStablo(korijen, bsm.korijen, nullptr);
    }
    ~AVLStabloMapa() {obrisiStablo(korijen);}
    AVLStabloMapa& operator=(const AVLStabloMapa &bsm);         //DA
    TipVrijednosti operator[] (const TipKljuca &kljuc) const;   //DA
    TipVrijednosti& operator[] (const TipKljuca &kljuc);        //DA
    int brojElemenata() const {return brojEl;}                  //DA
    void obrisi() { obrisiStablo(korijen); korijen = nullptr; brojEl = 0; } //DA
    void obrisi (const TipKljuca &x);                           //DA
    Cvor* trazi (Cvor *c, const TipKljuca &x) const {           //DA
        if(c == nullptr || c->kljuc == x)
            return c;
        else if(x < c->kljuc)
            return trazi(c->lijevo, x);
        else
            return trazi(c->desno, x);
    }
    Cvor* dodaj (Cvor *&c, const TipKljuca &k, Cvor *root) {    //DA
        if(c == nullptr) {
            brojEl++;
            c = new Cvor(k, TipVrijednosti(), nullptr, nullptr, root);
            return c;
        } 
        else if(c->kljuc > k)
            return dodaj(c->lijevo, k, c);
        else
            return dodaj(c->desno, k, c);
    }
    void umetni(Cvor *&c);                                      //DA
    void rotacijaDesno(Cvor *&c);                               //DA
    void rotacijaLijevo(Cvor *&c);                              //DA
    void updateBalans(Cvor *&c);                                //DA
};

template<typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>& AVLStabloMapa<TipKljuca, TipVrijednosti>::operator=(const AVLStabloMapa &bsm) {   //DA
    if(this == &bsm)
        return *this;
    obrisiStablo(korijen);
    korijen = nullptr;
    brojEl = 0;
    kopirajStablo(korijen, bsm.korijen, nullptr);
    return *this;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti AVLStabloMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &kljuc) const { //DA
    Cvor* rez = trazi(this->korijen, kljuc);
    if(rez != nullptr)
        return rez->vrijednost;
    TipVrijednosti temp{};
    return temp;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& AVLStabloMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &kljuc) {  //DA
    Cvor* rez = trazi(this->korijen, kljuc);
    if(rez != nullptr)
        return rez->vrijednost;
    Cvor *temp = new Cvor(kljuc, TipVrijednosti{});
    this->umetni(temp);
    return temp->vrijednost;
}

template<typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &x) { //DA
    Cvor *p = korijen, *q = nullptr, *tmp = nullptr, *rp = nullptr, *pp = nullptr;
    while(p != nullptr && p->kljuc != x){
        q = p;
        if(x < p->kljuc)
            p = p->lijevo;
        else 
            p = p->desno;
    }
    if(p == nullptr)
        throw std::logic_error("Nije pronadjen element x");
    if(p->lijevo == nullptr)
        rp = p->desno;
    else if(p->desno == nullptr)
        rp = p->lijevo;
    else{
        pp = p;
        rp = p->lijevo;
        tmp = rp->desno;
        while(tmp!=nullptr){
            pp = rp;
            rp = tmp;
            tmp = rp->desno;
        }
        if(pp != p){
            pp->desno = rp->lijevo;
            rp->lijevo = p->lijevo;
            pp->roditelj = rp;
            rp->roditelj = q;
            if(pp->desno)
                pp->desno->roditelj = pp;
        }
        rp->desno = p->desno;
        p->desno->roditelj = rp;
    }
    if(q == nullptr){
        korijen = rp;
        if(korijen)
            korijen->roditelj = nullptr;
    }
    else if(p == q->lijevo){
        q->lijevo = rp;
        if(rp)
            rp->roditelj = q;
    }
    else{
        q->desno = rp;
        if(rp)
            rp->roditelj = q;
    }
    if(p->roditelj && p->roditelj->lijevo == p)
        p->roditelj->balans--;
    else if(p->roditelj)
        p->roditelj->balans++;
    delete p;
    brojEl--;
}

template<typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::umetni(Cvor *&c) {   //DA
    Cvor *temp = nullptr;
    Cvor *root = korijen;
    while(root!=nullptr) { //Prolazak kroz stablo
        temp = root;
        if(c->kljuc < root->kljuc)
            root = root->lijevo;
        else
            root = root->desno;
    }

    bool hasSibling = false;
    if(temp == nullptr) {
        korijen = c;
        brojEl++;
        hasSibling = true;
    } else {
        if(c->kljuc < temp->kljuc) {
            temp->lijevo = c;
            c->roditelj = temp;
            brojEl++;
            if(temp->desno) {
                hasSibling = true;
                temp->balans = 0;
            }
        } else {
            temp->desno = c;
            c->roditelj = temp;
            brojEl++;
            if(temp->lijevo) {
                hasSibling = true;
                temp->balans = 0;
            }
        }
    }
    if(!hasSibling)
        updateBalans(c);
}

template<typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::rotacijaDesno(Cvor *&c) {    //DA
    if(c->roditelj && c->roditelj->lijevo == c)
        c->roditelj->lijevo = c->lijevo;
    else if(c->roditelj)
        c->roditelj->desno = c->lijevo;
    
    Cvor *temp = c->lijevo;
    bool exists = false;

    if(c->lijevo->desno) {
        c->lijevo = temp->desno;
        temp->desno->roditelj = c;
        temp->desno = nullptr;
        exists = true;
    }
    temp->roditelj = c->roditelj;

    if(c->roditelj)
        c->roditelj = temp;
    else {
        c->roditelj = temp;
        korijen = temp;
    }

    temp->desno = c;
    temp->balans = 0;
    c->balans = 0;
    
    if(!exists)
        c->lijevo = nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::rotacijaLijevo(Cvor *&c) {   //DA
    if(c->roditelj && c->roditelj->desno == c)
        c->roditelj->desno = c->desno;
    else if(c->roditelj)
        c->roditelj->lijevo = c->desno;
    
    Cvor *temp = c->desno;
    bool exists = false;

    if(c->desno->lijevo) {
        c->desno = temp->lijevo;
        temp->lijevo->roditelj = c;
        temp->lijevo = nullptr;
        exists = true;
    }
    temp->roditelj = c->roditelj;

    if(c->roditelj)
        c->roditelj = temp;
    else {
        c->roditelj = temp;
        korijen = temp;
    }

    temp->lijevo = c;
    temp->balans = 0;
    c->balans = 0;
    
    if(!exists)
        c->desno = nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::updateBalans(Cvor *&c) { //DA
    if(c->roditelj == nullptr)
        return;
    if(!(c->lijevo && c->desno && c->balans == 0)) {
        if(c->roditelj->lijevo == c)
            c->roditelj->balans++;
        else
            c->roditelj->balans--;
    }

    bool pom = true;
    Cvor *c1 = c->roditelj;
    Cvor *c2 = c;

    if(c->balans > 0 && c->roditelj->balans < -1) {
        rotacijaDesno(c2);
        rotacijaLijevo(c1);
        pom = false;
    } else if(c->balans < 0 && c->roditelj->balans > 1) {
        rotacijaLijevo(c2);
        rotacijaDesno(c1);
        pom = false;
    } else if(c->roditelj->balans < -1) {
        rotacijaLijevo(c1);
        pom = false;
    } else if(c->roditelj->balans > 1) {
        rotacijaDesno(c1);
        pom = false;
    }

    if(pom)
        updateBalans(c->roditelj);
}

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
private:    
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;
        Cvor () { lijevo = desno = roditelj = nullptr; }
        Cvor (const TipKljuca &kljuc, const TipVrijednosti &vrijednost, Cvor *l = nullptr, Cvor *d = nullptr, Cvor *r = nullptr){
            this->kljuc = kljuc;
            this->vrijednost = vrijednost;
            lijevo = l;
            desno = d;
            roditelj = r;
        }
    };

    Cvor *korijen;
    int brojEl;
    
    void kopirajStablo(Cvor *&c, Cvor *c2, Cvor *root){
        if(c2 != nullptr){
            c = new Cvor(c2->kljuc, c2->vrijednost, nullptr, nullptr, root);
            kopirajStablo(c->lijevo, c2->lijevo, c);
            kopirajStablo(c->desno, c2->desno, c);
            brojEl++;
        }
    }
    void obrisiStablo(Cvor *root) {
        if(root == nullptr) return;
        obrisiStablo(root->lijevo);
        obrisiStablo(root->desno);
        delete root;
    }
public:
    BinStabloMapa() : korijen(nullptr), brojEl(0) {};
    BinStabloMapa(const BinStabloMapa &bsm) : korijen(nullptr), brojEl(0) {
        kopirajStablo(korijen, bsm.korijen, nullptr);
    }
    ~BinStabloMapa() {obrisi();}
    BinStabloMapa& operator=(const BinStabloMapa &bsm);
    TipVrijednosti operator[] (const TipKljuca &kljuc) const;
    TipVrijednosti& operator[] (const TipKljuca &kljuc);
    bool operator>(const BinStabloMapa &bsm) const;
    bool operator<(const BinStabloMapa &bsm) const;
    int brojElemenata() const {return brojEl;}
    void obrisi() { obrisiStablo(korijen); korijen = nullptr; brojEl = 0; }
    void obrisi (const TipKljuca &x);
    Cvor* trazi (Cvor *c, const TipKljuca &x) const {
        if(c == nullptr || c->kljuc == x)
            return c;
        else if(x < c->kljuc)
            return trazi(c->lijevo, x);
        else
            return trazi(c->desno, x);
    }
    Cvor* dodaj (Cvor *&c, const TipKljuca &k, Cvor *root) {
        if(c == nullptr) {
            brojEl++;
            c = new Cvor(k, TipVrijednosti(), nullptr, nullptr, root);
            return c;
        } 
        else if(c->kljuc > k)
            return dodaj(c->lijevo, k, c);
        else
            return dodaj(c->desno, k, c);
    }
};

template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>& BinStabloMapa<TipKljuca, TipVrijednosti>::operator=(const BinStabloMapa &bsm) {
    if(this == &bsm)
        return *this;
    this->obrisi();        //Moguca greska
    kopirajStablo(korijen, bsm.korijen, nullptr);
    return *this;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &kljuc) const {
    auto rez = trazi(this->korijen, kljuc);
    if(rez != nullptr)
        return rez->vrijednost;
    TipVrijednosti temp{};
    return temp;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &kljuc) {
    auto rez = trazi(this->korijen, kljuc);
    if(rez != nullptr)
        return rez->vrijednost;
    rez = dodaj(korijen, kljuc, nullptr);
    return rez->vrijednost;
}

template<typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &x) {
    Cvor *p = korijen, *q = nullptr, *tmp = nullptr, *rp = nullptr, *pp = nullptr;
        while(p != nullptr && p->kljuc != x){
            q = p;
            if(x < p->kljuc)
                p = p->lijevo;
            else 
                p = p->desno;
        }
        if(p == nullptr)
            throw std::logic_error("Nije pronadjen element x");
        if(p->lijevo == nullptr)
            rp = p->desno;
        else if(p->desno == nullptr)
            rp = p->lijevo;
        else{
            pp = p;
            rp = p->lijevo;
            tmp = rp->desno;
            while(tmp!=nullptr){
                pp = rp;
                rp = tmp;
                tmp = rp->desno;
            }
            if(pp != p){
                pp->desno = rp->lijevo;
                rp->lijevo = p->lijevo;
                pp->roditelj = rp;
                rp->roditelj = q;
                if(pp->desno)
                    pp->desno->roditelj = pp;
            }
            rp->desno = p->desno;
        }
        if(q == nullptr)
            korijen = rp;
        else if(p == q->lijevo)
            q->lijevo = rp;
        else
            q->desno = rp;
        delete p;
        brojEl--;
}


int main() {
    BinStabloMapa<int, int> bsm;
    AVLStabloMapa<int, int> avl;
    int n = 10;

    clock_t t1 = clock(); //Dodavanje u BinStabloMapa
    for(int i=0; i<3000; i++)
        bsm[n++] = i*2;
    clock_t t1_kraj = clock();

    clock_t t2 = clock(); //Dodavanje u NizMapa
    n=10;
    for(int i=0; i<3000; i++)
        avl[n++] = i*2;
    clock_t t2_kraj = clock();

    int vrijemeBinStabloMapa = (t1_kraj-t1)/(CLOCKS_PER_SEC/1000);
    cout << "Vrijeme izvršenja dodavanja elemenata za BinStabloMapa: " << vrijemeBinStabloMapa << " milisekundi.\n";
    int vrijemeAVLMapa = (t2_kraj-t2)/(CLOCKS_PER_SEC/1000);
    cout << "Vrijeme izvršenja dodavanja elemenata za AVLStabloMapa: " << vrijemeAVLMapa << " milisekundi.\n\n";

    //Vec nakon ovog testa primijetna je razlika za unos elemenata u BinStabloMapa i AVLStabloMapa. 
    //Unos u AVLStablo je dosta brže.
    
    t1 = clock();
    bsm[5515] = 1345;   //Pristup elementima BinStabloMapa
    t1_kraj = clock();

    t2 = clock();
    avl[5515] = 1345;    //Pristup elementima AVLStabloMapa
    t2_kraj = clock();

    vrijemeBinStabloMapa = (t1_kraj-t1)/(CLOCKS_PER_SEC/1000);
    cout << "Vrijeme izvršenja pristupa elementima za BinStabloMapa: " << vrijemeBinStabloMapa << " milisekundi.\n";
    vrijemeAVLMapa = (t2_kraj-t2)/(CLOCKS_PER_SEC/1000);
    cout << "Vrijeme izvršenja pristupa elementima za NizMapa: " << vrijemeAVLMapa << " milisekundi.\n";

    //Nije primijetna razlika u vremenima pristupa u ovom slučaju.
    //Inače je vrijeme pristupa elementima AVLStabla dosta brže od pristupa elementima BinStabla.
    return 0;
}

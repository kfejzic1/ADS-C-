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

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti>{
private:
    pair<TipKljuca, TipVrijednosti> **mapa;
    int brojEl;
    int velicina;

    void ocistiMemoriju() {
        for(int i=0; i<velicina; i++) {
            delete mapa[i];
            mapa[i] = nullptr;
        }
        delete[] mapa;
        mapa = nullptr;
    }

    void prosiriMemoriju() {
        pair<TipKljuca, TipVrijednosti> **novaMapa = new pair<TipKljuca, TipVrijednosti>*[velicina*2] {};
        for(int i=0; i<brojEl; i++)
            novaMapa[i] = new pair<TipKljuca, TipVrijednosti>(*mapa[i]);
        ocistiMemoriju();
        velicina*=2;
        mapa = novaMapa;
    }

    int dajIndeks(TipKljuca kljuc) const {
        for(int i=0; i<brojEl; i++) {
            if(mapa[i] != nullptr && mapa[i]->first == kljuc) {
                return i;
            }
        }
        return -1;
    }
public:
    ~NizMapa() override {ocistiMemoriju();}
    NizMapa() : brojEl(0), velicina(50), mapa(new pair<TipKljuca, TipVrijednosti>*[50]{}) {};
    NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &m) : brojEl(m.brojEl), velicina(m.velicina) {
        mapa = new pair<TipKljuca, TipVrijednosti>*[m.velicina]{};
        for(int i=0; i<m.brojEl; i++)
            mapa[i] = new pair<TipKljuca, TipVrijednosti>(*m.mapa[i]);
    }
    NizMapa(NizMapa<TipKljuca, TipVrijednosti> &&m) : velicina(m.velicina), brojEl(m.brojEl), mapa(m.mapa) {
        m.mapa = nullptr;
    }
    NizMapa &operator=(const NizMapa &m);
    NizMapa &operator=(NizMapa &&m);
    int brojElemenata() const override {return brojEl;};
    void obrisi() override {ocistiMemoriju(); velicina = 50; brojEl = 0; mapa = new pair<TipKljuca, TipVrijednosti>*[velicina]{};}
    void obrisi(const TipKljuca &kljuc) override;
    TipVrijednosti &operator[](const TipKljuca &kljuc) override;
    TipVrijednosti operator[](const TipKljuca &kljuc) const override;
};

template<typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
    int i = dajIndeks(kljuc);
    if(i!=-1) {
        delete mapa[i];
        mapa[i] = nullptr;
        brojEl--;
    }
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& NizMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &kljuc) {
    int i = dajIndeks(kljuc);
    if(i==-1) {
        if(brojEl>=velicina)
            prosiriMemoriju();
        
        int j;
        for(j=0; j<brojEl; j++) {
            if(j != brojEl && mapa[j] == nullptr) {
                break;
            }
        }
        
        mapa[j] = new pair<TipKljuca, TipVrijednosti>(make_pair(kljuc, TipVrijednosti{}));
        i = j;
        brojEl++;
    }
    return mapa[i]->second;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti NizMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &kljuc) const {
    int i = dajIndeks(kljuc);
    if(i==-1)
        return TipVrijednosti{};    //Postoji greška
    return mapa[i]->second;
}

template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>& NizMapa<TipKljuca, TipVrijednosti>::operator=(const NizMapa<TipKljuca, TipVrijednosti> &m) {
    pair<TipKljuca, TipVrijednosti> **temp = new pair<TipKljuca, TipVrijednosti> *[m.velicina]{};

    for(int i=0; i<m.brojEl; i++) 
        temp[i] = new pair<TipKljuca, TipVrijednosti>(*m.mapa[i]);
    
    ocistiMemoriju();
    velicina = m.velicina;
    brojEl = m.brojEl;
    mapa = temp;
    return *this;
} 

template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>& NizMapa<TipKljuca, TipVrijednosti>::operator=(NizMapa<TipKljuca, TipVrijednosti> &&m) {
    if(&m!=this) {
        ocistiMemoriju();
        mapa = m.mapa;
        velicina = m.velicina;
        brojEl = m.brojEl;
        m.mapa=nullptr;
    }
    return *this;
}

int main() {
    //Testni program

    BinStabloMapa<int, int> bsm;
    NizMapa<int, int> nm;
    int n = 10;

    clock_t t1 = clock(); //Dodavanje u BinStabloMapa
    for(int i=0; i<3000; i++)
        bsm[n++] = i*2;
    clock_t t1_kraj = clock();

    clock_t t2 = clock(); //Dodavanje u NizMapa
    n=10;
    for(int i=0; i<3000; i++)
        nm[n++] = i*2;
    clock_t t2_kraj = clock();

    int vrijemeBinStabloMapa = (t1_kraj-t1)/(CLOCKS_PER_SEC/1000);
    cout << "Vrijeme izvršenja dodavanja elemenata za BinStabloMapa: " << vrijemeBinStabloMapa << " milisekundi.\n";
    int vrijemeNizMapa = (t2_kraj-t2)/(CLOCKS_PER_SEC/1000);
    cout << "Vrijeme izvršenja dodavanja elemenata za NizMapa: " << vrijemeNizMapa << " milisekundi.\n\n";

    //Vrijeme izvršenja BinStabla je nešto veće od izvršenja NizMape zato što je relativno mali broj elemenata unesen.
    //Nakon unosa mnogo više elemenata, posle određenog vremena, ubacivanje u BinStablo postaje brže od ubacivanja u NizMapu.

    
    t1 = clock();
    bsm[5515] = 1345;   //Pristup elementima BinStabloMapa
    t1_kraj = clock();

    t2 = clock();
    nm[5515] = 1345;    //Pristup elementima NizMapa
    t2_kraj = clock();

    vrijemeBinStabloMapa = (t1_kraj-t1)/(CLOCKS_PER_SEC/1000);
    cout << "Vrijeme izvršenja pristupa elementima za BinStabloMapa: " << vrijemeBinStabloMapa << " milisekundi.\n";
    vrijemeNizMapa = (t2_kraj-t2)/(CLOCKS_PER_SEC/1000);
    cout << "Vrijeme izvršenja pristupa elementima za NizMapa: " << vrijemeNizMapa << " milisekundi.\n";

    //Nije primijetna razlika u vremenima pristupa u ovom slučaju.
    //Inače je vrijeme pristupa elementima BinStabla dosta brže od pristupa elementima NizMape.
    
    return 0;
}

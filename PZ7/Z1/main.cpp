#include <iostream>
#include <utility>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
public:
    Mapa(){};
    virtual ~Mapa(){};
    virtual TipVrijednosti &operator[](TipKljuca kljuc) = 0;
    virtual TipVrijednosti operator[](TipKljuca kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
};

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
    TipVrijednosti &operator[](TipKljuca kljuc);
    TipVrijednosti operator[](TipKljuca kljuc) const;
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
TipVrijednosti& NizMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) {
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
TipVrijednosti NizMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) const {
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
    std::cout << "Pripremna Zadaca Za Vjezbu 7, Zadatak 1";
    return 0;
}

#include <iostream>
#include <vector>

using namespace std;

template<typename TipOznake>
class Cvor;
template<typename TipOznake>
class Grana;
template<typename TipOznake>
class GranaIterator;

template<typename TipOznake>
class UsmjereniGraf {
public:
    UsmjereniGraf() = default;
    virtual ~UsmjereniGraf(){};
    virtual int dajBrojCvorova() = 0;
    virtual void postaviBrojCvorova(int brojCvorova) = 0;
    virtual void dodajGranu(int polazniCvor, int dolazniCvor, double tezinaGrane) = 0;
    virtual void obrisiGranu(int polazniCvor, int dolazniCvor) = 0;
    virtual void postaviTezinuGrane(int polazniCvor, int dolazniCvor, double tezinaGrane) = 0;
    virtual double dajTezinuGrane(int polazniCvor, int dolazniCvor) = 0;
    virtual bool postojiGrana(int polazniCvor, int dolazniCvor) const = 0;
    virtual void postaviOznakuCvora(int brojCvora, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int brojCvora) const = 0;
    virtual void postaviOznakuGrane(int polazniCvor, int dolazniCvor, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) const = 0;
    virtual Cvor<TipOznake> dajCvor(int brojCvora) = 0;
    virtual Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;   //Pocetni iterator
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;     //Posljednji iterator
};

template<typename TipOznake>
class Cvor {
private:
    TipOznake oznaka;
    int redniBroj;
    UsmjereniGraf<TipOznake> *graf;
    friend class Grana<TipOznake>;
public:
    Cvor(TipOznake oznaka, int redniBroj, UsmjereniGraf<TipOznake> *graf) : oznaka(oznaka), redniBroj(redniBroj), graf(graf){};
    Cvor() : oznaka(TipOznake{}), redniBroj(0), graf(nullptr) {};
    TipOznake dajOznaku() const {return oznaka;}
    void postaviOznaku(TipOznake novaOznaka) {graf->postaviOznakuCvora(redniBroj, novaOznaka);} //Obratiti paznju
    void postaviOznakuCvora(TipOznake novaOznaka) {oznaka = novaOznaka;}
    int dajRedniBroj() const {return redniBroj;}
};

template<typename TipOznake>
class Grana {
private:
    double tezina;
    TipOznake oznaka;
    Cvor<TipOznake> *polazniCvor, *dolazniCvor;
public:
    Grana(double tezina, TipOznake oznaka, Cvor<TipOznake> *polazniCvor, Cvor<TipOznake> *dolazniCvor) : tezina(tezina), oznaka(oznaka), polazniCvor(polazniCvor), dolazniCvor(dolazniCvor){};
    Grana() : tezina(0), oznaka(TipOznake{}), polazniCvor(nullptr), dolazniCvor(nullptr) {};
    double dajTezinu() const {return tezina;}
    void postaviTezinu(double novaTezina) {polazniCvor->graf->postaviTezinuGrane(polazniCvor->dajRedniBroj(), dolazniCvor->dajRedniBroj(), novaTezina);}
    void postaviTezinuGrane(double novaTezina) {tezina = novaTezina;} //Mozda je moguce metodu staviti u private i postaviti klase za friends
    TipOznake dajOznaku() const {return oznaka;}
    void postaviOznaku(TipOznake novaOznaka) {polazniCvor->graf->postaviOznakuGrane(polazniCvor->dajRedniBroj(), dolazniCvor->dajRedniBroj(), novaOznaka);}
    void postaviOznakuGrane(TipOznake novaOznaka) {oznaka = novaOznaka;}
    Cvor<TipOznake> dajPolazniCvor() {return *polazniCvor;}
    Cvor<TipOznake> dajDolazniCvor() {return *dolazniCvor;}
};

template<typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {
private:
    vector<vector<Grana<TipOznake>*>> matrica;
    vector<Cvor<TipOznake>*> cvorovi;
    int posjeceni = 0;
    void daLiPostojiCvor(int brojCvora) const{
        if(cvorovi[brojCvora] == nullptr)
            throw("Cvor ne postoji!");
    }
public:
    MatricaGraf(int brojCvorova);
    ~MatricaGraf();
    int dajBrojCvorova() override {return cvorovi.size();}
    void postaviBrojCvorova (int brojCvorova) override;
    void dodajGranu(int polazniCvor, int dolazniCvor, double tezinaGrane) override {
        if(matrica[polazniCvor][dolazniCvor] != nullptr || polazniCvor>cvorovi.size() || dolazniCvor>cvorovi.size())
            throw("Neispravni polazni/dolazni indeks cvora!");
        matrica[polazniCvor][dolazniCvor] = new Grana<TipOznake>(tezinaGrane, TipOznake{}, cvorovi[polazniCvor], cvorovi[dolazniCvor]);
    }
    void obrisiGranu(int polazniCvor, int dolazniCvor) override {
        if(!postojiGrana(polazniCvor, dolazniCvor))
            throw("Grana ne postoji!");
        delete matrica[polazniCvor][dolazniCvor];
        matrica[polazniCvor][dolazniCvor] = nullptr;
    }
    void postaviTezinuGrane(int polazniCvor, int dolazniCvor, double tezinaGrane) override {
        if(!postojiGrana(polazniCvor, dolazniCvor))
            throw("Grana ne postoji!");
        matrica[polazniCvor][dolazniCvor]->postaviTezinuGrane(tezinaGrane);
    }
    double dajTezinuGrane(int polazniCvor, int dolazniCvor) override {
        if(!postojiGrana(polazniCvor, dolazniCvor))
            throw("Grana ne postoji!");
        return matrica[polazniCvor][dolazniCvor]->dajTezinu();
    }
    bool postojiGrana(int polazniCvor, int dolazniCvor) const override {
        if(polazniCvor>cvorovi.size() || dolazniCvor>cvorovi.size())
            return false;
        return matrica[polazniCvor][dolazniCvor];   //Ako ne postoji u matrici je upisana 0 tako da vraća false
    }
    void postaviOznakuCvora(int brojCvora, TipOznake oznaka) override {
        daLiPostojiCvor(brojCvora);
        cvorovi[brojCvora]->postaviOznakuCvora(oznaka);
    }
    TipOznake dajOznakuCvora(int brojCvora) const override{
        daLiPostojiCvor(brojCvora);
        return cvorovi[brojCvora]->dajOznaku();
    }
    void postaviOznakuGrane(int polazniCvor, int dolazniCvor, TipOznake oznaka) override {
        if(!postojiGrana(polazniCvor, dolazniCvor))
            throw("Grana ne postoji!");
        matrica[polazniCvor][dolazniCvor]->postaviOznakuGrane(oznaka);
    }
    TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) const override {
        if(!postojiGrana(polazniCvor, dolazniCvor))
            throw("Grana ne postoji!");
        return matrica[polazniCvor][dolazniCvor]->dajOznaku();
    }
    Cvor<TipOznake> dajCvor(int brojCvora) override {
        daLiPostojiCvor(brojCvora);
        return *cvorovi[brojCvora];
    }
    Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor) override {
        if(!postojiGrana(polazniCvor, dolazniCvor))
            throw("Grana ne postoji!");
        return *matrica[polazniCvor][dolazniCvor];
    }
    GranaIterator<TipOznake> dajGranePocetak() override;
    GranaIterator<TipOznake> dajGraneKraj() override;
    friend class GranaIterator<TipOznake>;
};

template<typename TipOznake>
MatricaGraf<TipOznake>::MatricaGraf(int brojCvorova) {
    matrica.resize(brojCvorova);
    
    for(int i=0; i<brojCvorova; i++)
        matrica[i].resize(brojCvorova, nullptr);
    cvorovi.resize(brojCvorova, nullptr);
    for(int i=0; i<brojCvorova; i++) 
        cvorovi[i] = new Cvor<TipOznake>(TipOznake{}, i, this);
}

template<typename TipOznake>
MatricaGraf<TipOznake>::~MatricaGraf() {
    for(int i=0; i<matrica.size(); i++) {
        for(int j=0; j<matrica.size(); j++) {
            delete matrica[i][j];
        }
    }

    for(int i=0; i<cvorovi.size(); i++)
        delete cvorovi[i];
}

template<typename TipOznake>
void MatricaGraf<TipOznake>::postaviBrojCvorova(int brojCvorova) {
    if(brojCvorova<cvorovi.size())
        throw ("Broj cvorova nije moguce smanjiti!");
    
    matrica.resize(brojCvorova);
    for(int i=0; i<brojCvorova; i++)
        matrica[i].resize(brojCvorova, nullptr);
    int temp = cvorovi.size();
    cvorovi.resize(brojCvorova, nullptr);
    for(int i=temp; i<brojCvorova; i++) //Dinamicki alociraj nove cvorove
        cvorovi[i] = new Cvor<TipOznake>(TipOznake{}, i, this);
}

template<typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::dajGranePocetak() {
    for(int i=0; i<matrica.size(); i++) {
        for(int j=0; j<matrica.size(); j++) {
            if(matrica[i][j] != nullptr)
                return GranaIterator<TipOznake>(this, i, j);
        }
    }
    return GranaIterator<TipOznake>(this, 0, 0);
}

template<typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::dajGraneKraj() {
    for(int i=matrica.size()-1; i>=0; i--) { //Mozda treba dodati znak jednakosti i>=0
        for(int j=matrica.size()-1; j>=0; j--) {
            if(matrica[i][j] != nullptr) {
                if(j<matrica.size()-1)
                    return GranaIterator<TipOznake>(this, i, j+1);
                else
                    return GranaIterator<TipOznake>(this, i+1, 0);
            }
        }
    }

    return GranaIterator<TipOznake>(this, matrica.size()-1, matrica.size()-1);
}

template <typename TipOznake>
class GranaIterator {
private:
    MatricaGraf<TipOznake> *matrica;
    int polazniCvor, dolazniCvor;
public:
    GranaIterator(MatricaGraf<TipOznake> *matrica, int polazniCvor, int dolazniCvor) : matrica(matrica), polazniCvor(polazniCvor), dolazniCvor(dolazniCvor) {};
    Grana<TipOznake> operator*() {
        if(matrica->matrica[polazniCvor][dolazniCvor])
            return *(matrica->matrica[polazniCvor][dolazniCvor]);
        throw("Greška!");
    }
    bool operator==(const GranaIterator &iter) const { return (matrica == iter.matrica && polazniCvor == iter.polazniCvor && dolazniCvor == iter.dolazniCvor); }
    bool operator!=(const GranaIterator &iter) const {return (!(matrica == iter.matrica && polazniCvor == iter.polazniCvor && dolazniCvor == iter.dolazniCvor));};
    GranaIterator& operator++();
    GranaIterator operator++(int);
};

template<typename TipOznake>
GranaIterator<TipOznake>& GranaIterator<TipOznake>::operator++() {
    if(polazniCvor == matrica->matrica.size()-1 && dolazniCvor == matrica->matrica.size()-1)
        throw("Kraj!\nPrekoračenje dimenzije");

    bool stop = false;
    GranaIterator<TipOznake> graneKraj = matrica->dajGraneKraj();
    int pol = graneKraj.polazniCvor, dol = graneKraj.dolazniCvor;

    for(int i=0; i<matrica->matrica.size(); i++) {
        for(int j=0; j<matrica->matrica.size(); j++) {
            if(i==pol && j==dol) {
                polazniCvor = pol;
                dolazniCvor = dol;
                return *this;
            }
            else if(((i==polazniCvor && j>dolazniCvor) || i>polazniCvor) && matrica->matrica[i][j]!=nullptr) {
                polazniCvor = i;
                dolazniCvor = j;
                stop = true;
                break;
            }
        }
        if(stop)
            break;
    }
    return *this;
}

template<typename TipOznake>
GranaIterator<TipOznake> GranaIterator<TipOznake>::operator++(int) {
    GranaIterator<TipOznake> old(matrica, polazniCvor, dolazniCvor);
    
    if(polazniCvor == matrica->matrica.size()-1 && dolazniCvor == matrica->matrica.size()-1)
        throw("Kraj!\nPrekoračenje dimenzije");
    
    bool stop = false;
    GranaIterator<TipOznake> graneKraj = matrica->dajGraneKraj();
    int pol = graneKraj.polazniCvor, dol = graneKraj.dolazniCvor;

    for(int i=0; i<matrica->matrica.size(); i++) {
        for(int j=0; j<matrica->matrica.size(); j++) {
            if(i==pol && j==dol) {
                old.polazniCvor = pol;
                old.dolazniCvor = dol;
                return old;
            }
            else if(((i==polazniCvor && j>dolazniCvor) || i>polazniCvor) && matrica->matrica[i][j] != nullptr) {
                polazniCvor = i;
                dolazniCvor = j;
                stop = true;
                break;
            }
        }
        if(stop)
            break;
    }

    return old;
}

int main() {
     try {
        UsmjereniGraf<bool> *g = new MatricaGraf<bool>(3);
        g->dodajGranu(0, 1, 2.5);
        g->dodajGranu(1, 0, 1.2);
        g->dodajGranu(1, 2, 0.1);
        g->dodajGranu(0, 0, 3.14);
        for (GranaIterator<bool> iter = g->dajGranePocetak();
        iter != g->dajGraneKraj(); ++iter)
        cout << (*iter).dajPolazniCvor().dajRedniBroj() << " "
        << (*iter).dajDolazniCvor().dajRedniBroj() << " "
        << (*iter).dajTezinu() << endl;
        delete g;
    } catch (const char izuzetak[]) {
        cout << izuzetak << endl;
    }
 return 0;

}

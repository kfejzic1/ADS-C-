#include <iostream>
#include <utility>
#include <vector>
#include <string>

using namespace std;

template <typename TipOznake> class Cvor;
template <typename TipOznake> class Grana;
template <typename TipOznake> class GranaIterator;
template <typename TipOznake> class ListaGraf;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
public:
    Mapa(){};
    virtual TipVrijednosti& operator[] (const TipKljuca kljuc) = 0;
    virtual TipVrijednosti operator[] (const TipKljuca kljuc) const = 0;
    virtual int brojElemenata() const=0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &x) = 0;
    virtual ~Mapa(){};
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
private:
    int brojEl;
    int vel;
    vector<vector<pair<TipKljuca, TipVrijednosti>>> map;
    unsigned int (*h)(TipKljuca, unsigned int);
    void indeks(TipKljuca kljuc, int &prviIndeks, int &drugiIndeks) const {
        unsigned int hash = h(kljuc, vel);
        int ind = hash%vel;
        if(map[ind].size()==0)
            return;
        
        for(int i=0; i<map[ind].size(); i++) {
            if(map[ind][i].first == kljuc) {
                prviIndeks = ind;
                drugiIndeks = i;
                return;
            }
        }
    }
    void prosiri() {
        vector<vector<pair<TipKljuca, TipVrijednosti>>> newMap(vel*2);
        for(int i=0; i<vel; i++) {
            for(int j=0; j<map[i].size(); j++) {
                unsigned int hash = h(map[i][j].first, vel*2);
                int indeks = hash%(vel*2);
                newMap[indeks].push_back(map[i][j]);
            }
        }
        map = newMap;
        vel*=2;
    }
public:
    HashMapaLan() : map(vector<vector<pair<TipKljuca, TipVrijednosti>>>(vel)), brojEl(0), vel(500), h(nullptr) {};
    ~HashMapaLan() {};
    HashMapaLan(const HashMapaLan<TipKljuca, TipVrijednosti> &hashMapa) : vel(hashMapa.vel), brojEl(hashMapa.brojEl), h(hashMapa.h), map(hashMapa.map) {};
    HashMapaLan<TipKljuca, TipVrijednosti>& operator=(const HashMapaLan<TipKljuca, TipVrijednosti> &hashMapa);
    TipVrijednosti operator[](const TipKljuca key) const;
    TipVrijednosti& operator[](const TipKljuca key);
    int brojElemenata() const {return brojEl;};
    void obrisi();
    void obrisi(const TipKljuca &key);
    void definisiHashFunkciju(unsigned int (*fja)(TipKljuca, unsigned int)) {h = fja;};
};

template <typename TipKljuca, typename TipVrijednosti>
HashMapaLan<TipKljuca, TipVrijednosti>& HashMapaLan<TipKljuca, TipVrijednosti>::operator=(const HashMapaLan<TipKljuca, TipVrijednosti> &hashMapa) {
    vector<vector<pair<TipKljuca, TipVrijednosti>>> newMap(hashMapa.map);
    vel = hashMapa.vel;
    brojEl = hashMapa.brojEl;
    map = newMap;
    h = hashMapa.h;

    return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti HashMapaLan<TipKljuca, TipVrijednosti>::operator[](const TipKljuca key) const{
    if(h == nullptr)
        throw ("Hash funkcija nije definisana!");
    int prviIndeks = -1, drugiIndeks = -1;
    indeks(key, prviIndeks, drugiIndeks);

    if(prviIndeks!=-1 && drugiIndeks!=-1) 
        return map[prviIndeks][drugiIndeks].second;
    
    return TipVrijednosti{};
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& HashMapaLan<TipKljuca, TipVrijednosti>::operator[](const TipKljuca key) {
    if(h != nullptr) {
        int prviIndeks = -1, drugiIndeks = -1, insertIndeks = -1;
        indeks(key, prviIndeks, drugiIndeks);

        if(prviIndeks == -1 || drugiIndeks == -1) {
            if(brojEl + vel/10 >= vel) //Potrebno prosirivanje
                prosiri();
            
            unsigned int hash = h(key, vel);
            prviIndeks = hash%vel;
            if(map[prviIndeks].size()!=0) {
                for(int i=0; i<map[prviIndeks].size()-1; i++) {
                    if(map[prviIndeks][i].first <key && map[prviIndeks][i+1].first > key) {
                        map[prviIndeks].insert(map[prviIndeks].begin()+i+1, make_pair(key, TipVrijednosti()));
                        insertIndeks = i+1;
                        break;
                    }
                }
            }
            
            if(insertIndeks == -1) {
                map[prviIndeks].push_back(make_pair(key,TipVrijednosti()));
                insertIndeks = map[prviIndeks].size()-1;
            }
            brojEl++;
            return map[prviIndeks][insertIndeks].second;
        } else {
            return map[prviIndeks][drugiIndeks].second;
        }
    } else {
        throw ("Hash fja nije definisana");
    }
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapaLan<TipKljuca, TipVrijednosti>::obrisi() {
    brojEl = 0;
    vel = 500;
    map.clear();
    map.resize(vel);
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapaLan<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &key) {
    int prviIndeks = -1, drugiIndeks = -1;
    indeks(key,prviIndeks,drugiIndeks);

    if(prviIndeks!=-1 && drugiIndeks!=-1) {
        map[prviIndeks].erase(map[prviIndeks].begin()+drugiIndeks);
        brojEl--;
    }
}

template <typename TipOznake> class UsmjereniGraf {
public:
  UsmjereniGraf() = default;
  virtual ~UsmjereniGraf(){};
  virtual int dajBrojCvorova() = 0;
  virtual void postaviBrojCvorova(int brojCvorova) = 0;
  virtual void dodajGranu(int polazniCvor, int dolazniCvor,
                          double tezinaGrane) = 0;
  virtual void obrisiGranu(int polazniCvor, int dolazniCvor) = 0;
  virtual void postaviTezinuGrane(int polazniCvor, int dolazniCvor,
                                  double tezinaGrane) = 0;
  virtual double dajTezinuGrane(int polazniCvor, int dolazniCvor) = 0;
  virtual bool postojiGrana(int polazniCvor, int dolazniCvor) const = 0;
  virtual void postaviOznakuCvora(int brojCvora, TipOznake oznaka) = 0;
  virtual TipOznake dajOznakuCvora(int brojCvora) const = 0;
  virtual void postaviOznakuGrane(int polazniCvor, int dolazniCvor,
                                  TipOznake oznaka) = 0;
  virtual TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) const = 0;
  virtual Cvor<TipOznake> dajCvor(int brojCvora) = 0;
  virtual Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor) = 0;
  virtual GranaIterator<TipOznake> dajGranePocetak() = 0; // Pocetni
  virtual GranaIterator<TipOznake> dajGraneKraj() = 0;    // Posljednji
};

template <typename TipOznake> class Cvor {
private:
  TipOznake oznaka;
  int redniBroj;
  UsmjereniGraf<TipOznake> *graf;
  vector<Grana<TipOznake> *> susjedniCvorovi;
  friend class Grana<TipOznake>;

public:
  Cvor(TipOznake oznaka, int redniBroj, UsmjereniGraf<TipOznake> *graf,
       vector<Grana<TipOznake> *> susjedniCvorovi)
      : oznaka(oznaka), redniBroj(redniBroj), graf(graf),
        susjedniCvorovi(susjedniCvorovi){};
  Cvor(TipOznake oznaka, int redniBroj, UsmjereniGraf<TipOznake> *graf)
      : oznaka(oznaka), redniBroj(redniBroj), graf(graf){};
  Cvor() : oznaka(TipOznake{}), redniBroj(0), graf(nullptr){};
  TipOznake dajOznaku() const { return oznaka; }
  void postaviOznaku(TipOznake novaOznaka) {
    graf->postaviOznakuCvora(redniBroj, novaOznaka);
  }
  void postaviOznakuCvora(TipOznake novaOznaka) { oznaka = novaOznaka; }
  int dajRedniBroj() const { return redniBroj; }
  bool daLiPostojiSusjed(int dolazni) const {
    for (int i = 0; i < susjedniCvorovi.size(); i++) {
      if (susjedniCvorovi[i]->dajDolazniCvor().dajRedniBroj() == dolazni)
        return true;
    }
    return false;
  }
  void dodajSusjedniCvor(Cvor *dolazni, double tezinaGrane) {
    susjedniCvorovi.push_back(
        new Grana<TipOznake>(tezinaGrane, TipOznake{}, this, dolazni));
  }
  Grana<TipOznake> *&dajGranuCvora(int dolazni) {
    for (int i = 0; i < susjedniCvorovi.size(); i++) {
      if (susjedniCvorovi[i]->dajDolazniCvor().dajRedniBroj() == dolazni)
        return susjedniCvorovi[i];
    }
    throw ("Grana ne postoji!");
  }
  friend class ListaGraf<TipOznake>;
  friend class GranaIterator<TipOznake>;
  template <typename TipOznakice>
  friend void dfs(UsmjereniGraf<TipOznakice> *g, vector<Cvor<bool>> &obilazak, Cvor<TipOznakice> pocetniCvor);
  template <typename TipOznakice>
  friend void bfs(UsmjereniGraf<TipOznakice> *g, vector<Cvor<bool>> &obilazak, Cvor<TipOznakice> pocetniCvor, int sljedeci);
};

template <typename TipOznake> class Grana {
private:
  double tezina;
  TipOznake oznaka;
  Cvor<TipOznake> *polazniCvor, *dolazniCvor;

public:
  Grana(double tezina, TipOznake oznaka, Cvor<TipOznake> *polazniCvor,
        Cvor<TipOznake> *dolazniCvor)
      : tezina(tezina), oznaka(oznaka), polazniCvor(polazniCvor),
        dolazniCvor(dolazniCvor){};
  Grana()
      : tezina(0), oznaka(TipOznake{}), polazniCvor(nullptr),
        dolazniCvor(nullptr){};
  double dajTezinu() const { return tezina; }
  void postaviTezinu(double novaTezina) {
    polazniCvor->graf->postaviTezinuGrane(
        polazniCvor->dajRedniBroj(), dolazniCvor->dajRedniBroj(), novaTezina);
  }
  void postaviTezinuGrane(double novaTezina) { tezina = novaTezina; }
  TipOznake dajOznaku() const { return oznaka; }
  void postaviOznaku(TipOznake novaOznaka) {
    polazniCvor->graf->postaviOznakuGrane(
        polazniCvor->dajRedniBroj(), dolazniCvor->dajRedniBroj(), novaOznaka);
  }
  void postaviOznakuGrane(TipOznake novaOznaka) { oznaka = novaOznaka; }
  Cvor<TipOznake> dajPolazniCvor() { return *polazniCvor; }
  Cvor<TipOznake> dajDolazniCvor() { return *dolazniCvor; }
};

template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake> {
private:
  // Moguce je u cvorovima drzati odgovarajuce grane
  vector<Cvor<TipOznake> *> cvorovi;
  int brojPosjecenih = 0;
  void daLiPostojiCvor(int brojCvora) const {
    if (cvorovi[brojCvora] == nullptr)
      throw("Cvor ne postoji!");
  }

public:
  ListaGraf(int brojCvorova);
  ~ListaGraf();
  int dajBrojCvorova() override { return cvorovi.size(); }
  void postaviBrojCvorova(int brojCvorova) override;
  void dodajGranu(int polazniCvor, int dolazniCvor,
                  double tezinaGrane) override {
    cvorovi[polazniCvor]->dodajSusjedniCvor(cvorovi[dolazniCvor], tezinaGrane);
  }
  void obrisiGranu(int polazniCvor, int dolazniCvor) override {
    if (!postojiGrana(polazniCvor, dolazniCvor))
      throw("Grana ne postoji!");

    for (int i = 0; i < cvorovi[polazniCvor]->susjedniCvorovi.size(); i++) {
      auto it = cvorovi[polazniCvor]->susjedniCvorovi.begin();
      if (cvorovi[polazniCvor]
              ->susjedniCvorovi[i]
              ->dajDolazniCvor()
              .dajRedniBroj() == dolazniCvor) {
        delete cvorovi[polazniCvor]->susjedniCvorovi[i];
        cvorovi[polazniCvor]->susjedniCvorovi.erase(it + i);
      }
    }
  }
  void postaviTezinuGrane(int polazniCvor, int dolazniCvor,
                          double tezinaGrane) override {
    if (!postojiGrana(polazniCvor, dolazniCvor))
      throw("Grana ne postoji!");
    cvorovi[polazniCvor]
        ->dajGranuCvora(dolazniCvor)
        ->postaviTezinuGrane(tezinaGrane);
  }
  double dajTezinuGrane(int polazniCvor, int dolazniCvor) override {
    if (!postojiGrana(polazniCvor, dolazniCvor))
      throw("Grana ne postoji!");
    return cvorovi[polazniCvor]->dajGranuCvora(dolazniCvor)->dajTezinu();
  }
  bool postojiGrana(int polazniCvor, int dolazniCvor) const override {
    if (polazniCvor > cvorovi.size() || dolazniCvor > cvorovi.size())
      return false;
    return cvorovi[polazniCvor]->daLiPostojiSusjed(
        dolazniCvor); // Ako ne postoji u matrici je upisana 0 tako da vraća
                      // false
  }
  void postaviOznakuCvora(int brojCvora, TipOznake oznaka) override {
    daLiPostojiCvor(brojCvora);
    cvorovi[brojCvora]->postaviOznakuCvora(oznaka);
  }
  TipOznake dajOznakuCvora(int brojCvora) const override {
    daLiPostojiCvor(brojCvora);
    return cvorovi[brojCvora]->dajOznaku();
  }
  void postaviOznakuGrane(int polazniCvor, int dolazniCvor,
                          TipOznake oznaka) override {
    if (!postojiGrana(polazniCvor, dolazniCvor))
      throw("Grana ne postoji!");
    cvorovi[polazniCvor]
        ->dajGranuCvora(dolazniCvor)
        ->postaviOznakuGrane(oznaka);
  }
  TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) const override {
    if (!postojiGrana(polazniCvor, dolazniCvor))
      throw("Grana ne postoji!");
    return cvorovi[polazniCvor]->dajGranuCvora(dolazniCvor)->dajOznaku();
  }
  Cvor<TipOznake> dajCvor(int brojCvora) override {
    daLiPostojiCvor(brojCvora);
    return *cvorovi[brojCvora];
  }
  Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor) override {
    if (!postojiGrana(polazniCvor, dolazniCvor))
      throw("Grana ne postoji!");
    return *cvorovi[polazniCvor]->dajGranuCvora(dolazniCvor);
  }
  GranaIterator<TipOznake> dajGranePocetak() override;
  GranaIterator<TipOznake> dajGraneKraj() override;
  friend class GranaIterator<TipOznake>;

  template <typename TipOznakice>
  friend void dfs(UsmjereniGraf<TipOznakice> *g, vector<Cvor<bool>> &obilazak,
                  Cvor<TipOznakice> pocetniCvor);
  template <typename TipOznakice>
  friend void bfs(UsmjereniGraf<TipOznakice> *g, vector<Cvor<bool>> &obilazak, Cvor<TipOznakice> pocetniCvor, int sljedeci);
};

template <typename TipOznake> ListaGraf<TipOznake>::ListaGraf(int brojCvorova) {
  cvorovi.resize(brojCvorova, nullptr);
  for (int i = 0; i < brojCvorova; i++)
    cvorovi[i] = new Cvor<TipOznake>(TipOznake{}, i, this);
}

template <typename TipOznake> ListaGraf<TipOznake>::~ListaGraf() {
  for (int i = 0; i < cvorovi.size(); i++) {
    for (int j = 0; j < cvorovi[i]->susjedniCvorovi.size(); j++) {
      delete cvorovi[i]->susjedniCvorovi[j];
    }
  }

  for (int i = 0; i < cvorovi.size(); i++)
    delete cvorovi[i];
}

template <typename TipOznake>
void ListaGraf<TipOznake>::postaviBrojCvorova(int brojCvorova) {
  if (brojCvorova < cvorovi.size())
    throw("Broj cvorova nije moguce smanjiti!");

  int temp = cvorovi.size();
  cvorovi.resize(brojCvorova, nullptr);

  for (int i = temp; i < brojCvorova; i++) // Dinamicki alociraj nove cvorove
    cvorovi[i] = new Cvor<TipOznake>(TipOznake{}, i, this);
}

template <typename TipOznake>
GranaIterator<TipOznake> ListaGraf<TipOznake>::dajGranePocetak() {
  for (int i = 0; i < cvorovi.size(); i++) {
    for (int j = 0; j < cvorovi[i]->susjedniCvorovi.size(); j++) {
      return GranaIterator<TipOznake>(this, i, j);
    }
  }
  return GranaIterator<TipOznake>(this, 0, 0);
}

template <typename TipOznake>
GranaIterator<TipOznake> ListaGraf<TipOznake>::dajGraneKraj() {
  for (int i = cvorovi.size() - 1; i > 0; i--) {
    if (cvorovi[i]->susjedniCvorovi.size() != 0) {
      for (int j = cvorovi[i]->susjedniCvorovi.size() - 1; j > 0; j--) {
        if (j <
            cvorovi[i]->susjedniCvorovi.size() -
                1) // mozda treba postaviti cvorovi[i]->susjedniCvorovi.size()
          return GranaIterator<TipOznake>(this, i, j + 1);
        else
          return GranaIterator<TipOznake>(this, i + 1, 0);
      }
    }
  }
  return GranaIterator<TipOznake>(
      this, cvorovi.size() - 1,
      cvorovi[cvorovi.size() - 1]->susjedniCvorovi.size() - 1);
}

template <typename TipOznake> class GranaIterator {
private:
  ListaGraf<TipOznake> *matrica;
  int polazniCvor, dolazniCvor;

public:
  GranaIterator(ListaGraf<TipOznake> *matrica, int polazniCvor, int dolazniCvor)
      : matrica(matrica), polazniCvor(polazniCvor), dolazniCvor(dolazniCvor){};
  Grana<TipOznake> operator*() {
    return matrica->dajGranu(polazniCvor, dolazniCvor);
  }
  bool operator==(const GranaIterator &iter) const {
    return (matrica == iter.matrica && polazniCvor == iter.polazniCvor &&
            dolazniCvor == iter.dolazniCvor);
  }
  bool operator!=(const GranaIterator &iter) const {
    return (!(matrica == iter.matrica && polazniCvor == iter.polazniCvor &&
              dolazniCvor == iter.dolazniCvor));
  };
  GranaIterator &operator++();
  GranaIterator operator++(int);
  int dajPolazniCvor() { return polazniCvor; }
  int dajDolazniCvor() { return dolazniCvor; }
};

template <typename TipOznake>
GranaIterator<TipOznake> &GranaIterator<TipOznake>::operator++() {
  if (polazniCvor == matrica->cvorovi.size() &&
      dolazniCvor == matrica->cvorovi.size())
    throw("Prekoracenje!");
  bool stop = false;
  GranaIterator<TipOznake> graneKraj =
      matrica->dajGraneKraj(); // Mozda greska zbog dajGraneKraj
  int pol = graneKraj.polazniCvor,
      dol = graneKraj.dolazniCvor; // Greska zbog poziva

  for (int i = 0; i < matrica->cvorovi.size(); i++) {
    for (int j = 0; j < matrica->cvorovi.size(); j++) {
      if (i == pol && j == dol) { // Mozda greska, treba uzimati redni broj
                                  // odgovarajućih cvorova umjesto i j
        polazniCvor = pol;
        dolazniCvor = dol;
        return *this;
      } else if (((i == polazniCvor && j > dolazniCvor) || i > polazniCvor) &&
                 matrica->postojiGrana(i, j)) {
        polazniCvor = i;
        dolazniCvor = j;
        stop = true;
        break;
      }
    }
    if (stop)
      break;
  }
  return *this;
}

template <typename TipOznake>
GranaIterator<TipOznake> GranaIterator<TipOznake>::operator++(int) {
  GranaIterator<TipOznake> iter(matrica, polazniCvor, dolazniCvor);
  ++(*this);
  return iter;
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *g, vector<Cvor<bool>> &obilazak, Cvor<TipOznake> pocetniCvor) {
  ListaGraf<TipOznake> *graf = dynamic_cast<ListaGraf<TipOznake> *>(g);
  auto posjecen = TipOznake(1);

  graf->cvorovi[pocetniCvor.dajRedniBroj()]->postaviOznaku(posjecen);
  obilazak.push_back(pocetniCvor);
  graf->brojPosjecenih++;   //Ukljucen pocetniCvor

  if(pocetniCvor.susjedniCvorovi.size() != 0) {
      for(int i=0; i<graf->cvorovi[pocetniCvor.dajRedniBroj()]->susjedniCvorovi.size(); i++) {
          if(graf->cvorovi[pocetniCvor.dajRedniBroj()]->susjedniCvorovi[i]->dajDolazniCvor().dajOznaku() != posjecen) {
              dfs(g, obilazak, graf->cvorovi[pocetniCvor.dajRedniBroj()]->susjedniCvorovi[i]->dajDolazniCvor());
          }
      }
  }

  if(graf->brojPosjecenih < graf->dajBrojCvorova()) {
      for(int i=0; i<graf->dajBrojCvorova(); i++) {
          for(int j=0; j<graf->cvorovi[i]->susjedniCvorovi.size(); j++) {
              if(graf->cvorovi[i]->susjedniCvorovi[j]->dajDolazniCvor().dajOznaku() != posjecen) {
                  if(graf->cvorovi[i]->dajOznaku() != posjecen) {
                      graf->cvorovi[i]->postaviOznaku(posjecen);
                      obilazak.push_back(*graf->cvorovi[i]);
                  }
                  dfs(g, obilazak, graf->cvorovi[i]->susjedniCvorovi[j]->dajDolazniCvor());
              }
          }
      }
  }
  if(graf->brojPosjecenih == graf->dajBrojCvorova())
    graf->brojPosjecenih = 0;
  
}

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *g, vector<Cvor<bool>> &obilazak, Cvor<TipOznake> pocetniCvor, int sljedeci = 0) {
    ListaGraf<TipOznake> *graf = dynamic_cast<ListaGraf<TipOznake> *>(g);
    if(graf->brojPosjecenih == graf->dajBrojCvorova()) {
        graf->brojPosjecenih = 0;
        return;
    }
    auto posjecen = TipOznake(1);

    if(graf->cvorovi[pocetniCvor.dajRedniBroj()]->dajOznaku() != posjecen) {
        graf->cvorovi[pocetniCvor.dajRedniBroj()]->postaviOznaku(posjecen);
        obilazak.push_back(pocetniCvor);
        graf->brojPosjecenih++;
    }

    for(int i=0; i<graf->cvorovi[pocetniCvor.dajRedniBroj()]->susjedniCvorovi.size(); i++) {
        if(graf->cvorovi[pocetniCvor.dajRedniBroj()]->susjedniCvorovi[i]->dajDolazniCvor().dajOznaku() != posjecen) {
            obilazak.push_back(graf->cvorovi[pocetniCvor.dajRedniBroj()]->susjedniCvorovi[i]->dajDolazniCvor());
            graf->cvorovi[pocetniCvor.dajRedniBroj()]->susjedniCvorovi[i]->dajDolazniCvor().postaviOznaku(posjecen);
            graf->brojPosjecenih++;
        }
    }
    bfs(g, obilazak, obilazak[++sljedeci], sljedeci);
}

int main() {

    UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > bfs_obilazak;
    bfs(g, bfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < bfs_obilazak.size(); i++)
      cout << bfs_obilazak[i].dajRedniBroj() << ",";
  delete g;
  
  return 0;
}

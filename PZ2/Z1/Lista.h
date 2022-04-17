#pragma once

template <typename tip> class Lista {
public:
  Lista(){};
  virtual ~Lista(){};
  virtual int brojElemenata() const = 0;
  virtual tip &trenutni() = 0;
  virtual const tip trenutni() const = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const tip &el) = 0;
  virtual void dodajIza(const tip &el) = 0;
  virtual tip &operator[](int i) = 0;
  virtual const tip operator[](int i) const = 0;
  virtual void ispisi() const = 0;
};

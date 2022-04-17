#include <iostream>
using namespace std;

template <typename tip> class Stek {
  struct Cvor {
    tip el;
    Cvor *sljedeci;
    Cvor(const tip &el, Cvor *sljedeci) : el(el), sljedeci(sljedeci){};
  };
  int vel;
  Cvor *top;
  void kopiraj(const Stek &s);
  bool daLiJePrazan() { return top == nullptr; }

public:
  Stek() : top(nullptr), vel(0){};
  Stek(const Stek<tip> &s) { kopiraj(s); };
  ~Stek() { brisi(); };
  Stek &operator=(const Stek &s);
  void brisi();
  void stavi(const tip &el) { top = new Cvor(el, top); vel++;};
  tip skini();
  tip &vrh() {
    if (daLiJePrazan())
      throw "\nStek je prazan!\n";
    return top->el;
  };
  int brojElemenata() { return vel; };
  void ispisi() const;
};

template <typename tip> void Stek<tip>::kopiraj(const Stek &s) {
  Cvor *q, *p(s.top);
  top = nullptr;
  vel = s.vel;

  while (p != nullptr) {
    Cvor *novi(new Cvor(p->el, nullptr));
    if (top == nullptr) // Prvi element
      top = novi;
    else
      q->sljedeci = novi;
    p = p->sljedeci;
    q = novi;
  }
}

template <typename tip> Stek<tip> &Stek<tip>::operator=(const Stek &s) {
  if(&s==this)
    return *this;
    brisi();
    kopiraj(s);
    return *this;
}

template <typename tip> tip Stek<tip>::skini() {
  if (daLiJePrazan())
    throw "\nStek je prazan!\n";
  tip el(top->el);
  Cvor *p = top->sljedeci;
  delete top;
  top = p;
  vel--;
  return el;
}

template <typename tip>
void Stek<tip>::brisi(){
    while(!daLiJePrazan())
        skini();
}

template <typename tip> void Stek<tip>::ispisi() const {
  Cvor *temp(top);
  while (temp != nullptr) {
    std::cout << temp->el << std::endl;
    temp = temp->sljedeci;
  }
  std::cout << std::endl;
}

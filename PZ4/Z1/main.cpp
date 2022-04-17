#include "Red.h"
#include <iostream>

using namespace std;

void popuniRed(Red<int> *r) {
  for (int i = 1; i <= 5; i++)
    r->stavi(i);
}

void testStavi(Red<int> *r) {
  popuniRed(r);

  if (r->dajString() == "1 2 3 4 5 ")
    std::cout << "OK\n";
  else
    std::cout << "NOK\n";
  delete r;
}

void testBrojElemenata(Red<int> *r) {
  popuniRed(r);
  if (r->brojElemenata() == 5) {
    r->brisi();
    if (r->brojElemenata() == 0)
      std::cout << "OK\n";
    else
      std::cout << "NOK\n";
  } else
    std::cout << "NOK\n";
  delete r;
}

void testBrisi(Red<int> *r) {
  popuniRed(r);
  r->brisi();
  if (r->brojElemenata() == 0)
    std::cout << "OK\n";
  else
    std::cout << "NOK\n";
  delete r;
}

void testSkini(Red<int> *r) {
  popuniRed(r);
  if (r->skini() == 1 && r->brojElemenata() == 4) {
    r->skini();
    r->skini();
    r->skini();
    r->skini();
    try {
      r->skini();
    } catch (...) {
      std::cout << "OK\n";
    }
  } else
    std::cout << "NOK\n";
  delete r;
}

void testCelo(Red<int> *r) {
  popuniRed(r);

  if (r->celo() == 1) {
    r->brisi();
    try {
      r->celo();
    } catch (...) {
      std::cout << "OK\n";
    }
  } else
    std::cout << "NOK\n";
  delete r;
}

int main() {
  Red<int> *r = new Red<int>;
  testStavi(r);

  r = new Red<int>;
  testBrojElemenata(r);

  r = new Red<int>;
  testBrisi(r);

  r = new Red<int>;
  testSkini(r);

  r = new Red<int>;
  testCelo(r);

  return 0;
}

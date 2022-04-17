#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

template <typename Tip> void zamijeni(Tip &a, Tip &b) {
  Tip temp = a;
  a = b;
  b = temp;
}

template <typename Tip> void selection_sort(Tip *niz, int vel) {
  for (int i = 0; i < vel - 1; i++) {
    int mini = i;
    for (int j = i + 1; j < vel; j++) {
      if (niz[j] < niz[mini])
        mini = j;
    }
    zamijeni(niz[mini], niz[i]);
  }
}

template <typename Tip> void bubble_sort(Tip *niz, int vel) {
  for (int i = vel - 1; i > 0; i--) {
    for (int j = 1; j <= i; j++) {
      if (niz[j - 1] > niz[j]) {
        zamijeni<Tip>(niz[j - 1], niz[j]);
      }
    }
  }
}

template <typename Tip>
void merge(Tip *niz, const int l, const int p, const int q, const int u) {
  int i = 0, j = q - l, k = l;
  Tip *n = new Tip[u - l + 1];
  for (int m = 0; m <= u - l; m++)
    n[m] = niz[l + m];
  while (i <= p - l && j <= u - l) {
    if (n[i] < n[j])
      niz[k] = n[i++];
    else
      niz[k] = n[j++];
    k++;
  }
  while (i <= p - l)
    niz[k++] = n[i++];
  while (j <= u - l)
    niz[k++] = n[j++];
  delete[] n;
}

template <typename Tip> void MergeSort(Tip *niz, const int l, const int u) {
  if (u > l) {
    int p = (l + u - 1) / 2;
    int q = p + 1;
    MergeSort<Tip>(niz, l, p);
    MergeSort<Tip>(niz, q, u);
    merge(niz, l, p, q, u);
  }
}

template <typename Tip> void merge_sort(Tip *niz, int vel) {
  MergeSort(niz, 0, vel - 1);
}

template <typename Tip>
int particija(Tip *niz, const int prvi, const int zadnji) {
  Tip pivot = niz[prvi];
  int p = prvi + 1;
  while (p <= zadnji) {
    if (niz[p] <= pivot)
      p++;
    else
      break;
  }
  for (int i = p + 1; i <= zadnji; i++) {
    if (niz[i] < pivot) {
      zamijeni(niz[p], niz[i]);
      p++;
    }
  }
  zamijeni(niz[prvi], niz[p - 1]);
  return p - 1;
}

template <typename Tip>
void QuickSort(Tip *niz, const int prvi, const int zadnji) {
  if (prvi < zadnji) {
    int j = particija(niz, prvi, zadnji);
    QuickSort(niz, prvi, j - 1);
    QuickSort(niz, j + 1, zadnji);
  }
}

template <typename Tip> void quick_sort(Tip *niz, int vel) {
  QuickSort(niz, 0, vel - 1);
}

void ucitaj(string filename, int *&niz, int &vel) {
  ifstream ulaz;
  ulaz.open(filename.c_str());

  int i = 0;
  if (ulaz.is_open()) {
    int temp;
    while (ulaz >> temp) {
      i++;
    }
  }
  ulaz.close();

  vel = i;
  niz = new int[vel];
  i = 0;

  ulaz.open(filename.c_str());
  if (ulaz.is_open()) {
    while (ulaz >> niz[i++])
      ;
  }
}

bool DaLiJeNizSortiran(int *niz, int vel) {
  for (int i = 0; i < vel - 1; i++) {
    if (niz[i] > niz[i + 1])
      return false;
  }
  return true;
}

void ZapisiUDatoteku(string filename, int *niz, int vel) {
  ofstream izlaz(filename);
  if (izlaz.is_open()) {
    for (int i = 0; i < vel; i++)
      izlaz << niz[i] << " ";

    izlaz.close();
    delete[] niz;
  }
}

void generisi(string filename, int vel) {
  ofstream izlaz(filename);
  if (izlaz.is_open()) {
    for (int i = 0; i < vel; i++)
      izlaz << rand() << " ";

    izlaz.close();
  }
}

int main() {
  int *niz;
  int vel;
  generisi("ulaz.txt", 100);
  ucitaj("ulaz.txt", niz, vel);
  clock_t pocetak = clock();

  int opcija;
  cout << "Odaberite algoritam za sortiranje:\n1-Bubble Sort\n2-Selection "
          "Sort\n3-Quick Sort\n4-Merge Sort\n";
  cin >> opcija;

  switch (opcija) {
  case 1:
    bubble_sort(niz, vel);
    break;
  case 2:
    selection_sort(niz, vel);
    break;
  case 3:
    quick_sort(niz, vel);
    break;
  case 4:
    merge_sort(niz, vel);
    break;
  }
  clock_t kraj = clock();

  cout << "Vrijeme sortiranja je: "
       << (kraj - pocetak) / (CLOCKS_PER_SEC / 1000) << " milisekundi.\n";

  if (DaLiJeNizSortiran(niz, vel)) {
    ZapisiUDatoteku("izlaz.txt", niz, vel);
    cout << "Sortirani niz: ";
    for (int i = 0; i < vel; i++)
      cout << niz[i] << " ";
    cout << endl;
  } else
    cout << "Niz nije sortiran";

  return 0;
}

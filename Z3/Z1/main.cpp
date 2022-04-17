#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void zamijeni(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
}

int dajMax(vector<int> a) {
    int max = a.at(0);
    for(int i=0; i<a.size(); i++) {
        if(a.at(i)>max)
            max = a.at(i);
    }
    return max;
}

void countSort(vector<int> &a, int pozicija) {
    vector<int> izlaz(a.size());
    int brojac[10] = {0};    //Brojac

    //Povecaj brojac za svaki element
    for(int i=0; i<a.size(); i++)
        brojac[(a.at(i)/pozicija)%10]++;

    //Saberi susjedne elemente brojaca
    for(int i=1; i<10; i++)
        brojac[i]+=brojac[i-1];

    //Sortiraj elemente
    for(int i=a.size()-1; i>=0; i--) {
        izlaz.at(brojac[(a.at(i)/pozicija)%10]-1) = a.at(i);
        brojac[(a.at(i)/pozicija)%10]--;
    }

    for(int i=0; i<a.size(); i++)
        a.at(i) = izlaz.at(i);
}

void radixSort(vector<int> &a) {
    int max = dajMax(a);

    for(int pozicija = 1; max/pozicija>0; pozicija*=10)
        countSort(a, pozicija);
}

bool jeLiList(int velicina, int i) {
    return (i>=velicina/2) && (i<velicina);
}

int roditelj(int i) {return (i-1)/2;}

int lijevoDijete(int i) {return 2*i+1;}

int desnoDijete(int i) {return 2*i+2;}

void popraviDolje(vector<int> &el, int velicina, int i) { //Provjereno
    while(!jeLiList(velicina, i)) {
        int veci = lijevoDijete(i);
        int dd=desnoDijete(i);

        if(dd<velicina && el[dd]>el[veci])
            veci = dd;
        if(el[i] < el[veci]) {
            zamijeni(el[i], el[veci]);
            i = veci;
        }
        else return;
    }
}

void popraviGore(vector<int> &el, int i) {  //Provjereno
    while(i!=0 && el[i]>el[roditelj(i)]) {
        zamijeni(el[i], el[roditelj(i)]);
        i = roditelj(i);
    }
}

void stvoriGomilu(vector<int> &a) {         //Provjereno
    for(int i = (a.size()-1)/2; i>=0; i--)
        popraviDolje(a, a.size(), i);
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) { //Provjereno
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a, velicina-1);
}

int izbaciPrvi(vector<int> &a, int &velicina) { //Provjereno
    if(velicina == 0)
        throw "Nema elemenata!\n";
    zamijeni(a[0], a[--velicina]);
    if(velicina != 0)
        popraviDolje(a, velicina, 0);
    return a[velicina];
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    int velicina = a.size();
    for(int i=0; i<a.size()-1; i++)
        izbaciPrvi(a, velicina);
}

int main() {
    vector<int> a;
    a.push_back(546);
    a.push_back(1);
    a.push_back(31);
    a.push_back(1001);
    a.push_back(104351);
    a.push_back(7);
    a.push_back(6);
    a.push_back(60045);
    gomilaSort(a);
    for(int i(0);i<a.size();i++){
        cout<<a[i]<<" ";
    }  
    return 0;
}

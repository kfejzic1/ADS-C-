#include <iostream>

using namespace std;

int fib(int n){
    if(n<=1)
        return n;
    return fib(n-1)+fib(n-2);
}

int fib_petlja(int n){
    if(n<=1)
        return n;
    int pretprosli(0), prosli(1), rez;
    for(int i(2); i<=n; i++){
        rez = pretprosli + prosli;
        pretprosli = prosli;
        prosli = rez;
    }
    return rez;
}

int fib2_0(int n, int pretprosli = 0, int prosli = 1){
    if(n == 0)
        return pretprosli;
    if(n == 1)
        return prosli;
    return fib2_0(n-1, prosli, prosli+pretprosli);
}

int nzd(int x, int y){
    if(x == 0)
        return y;
    return nzd(y%x, x);
}

int main() {
    std::cout << "Unesi redni broj fibonacijevog niza: ";
    int n;
    std::cin >> n;
    std::cout << "F[" << n << "]=" << fib2_0(n);

    std::cout << "\nUnesi dva broja ciji NZD zelis da izracunas: ";
    int a, b;
    std::cin >> a >> b;
    std::cout << "NZD(" << a << "," << b << ")=" << nzd(a, b);
    return 0;
}

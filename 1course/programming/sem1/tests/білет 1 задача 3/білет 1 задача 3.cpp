#include <iostream>
#include <math.h>

using namespace std;

long long int factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}

long long int BinomCoef(int n, int k) {
    return factorial(n) / (factorial(k) * factorial(n - k));
}

void print(int a, int b, int n) {
    for (int k = 0; k <= n; ++k) {
        cout << BinomCoef(n, k) * pow(a, n - k) * pow(b, k) << " ";
    }
}

int main()
{
    int a, b, n;
    cout<<"Enter a, b and n: ";
    cin>>a>>b>>n;
    print(a, b, n);
    return 0;
}

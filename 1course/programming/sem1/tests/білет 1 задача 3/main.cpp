//білет 1 задача 3

#include <iostream>
#include <math.h>

using namespace std;

long long int fact(int n)
{
    if (n == 0 || n == 1)
        return 1;
    return n * fact(n - 1);
}

long long int BinomCoef(int n, int k)
{
    return fact(n) / (fact(k) * fact(n - k));
}

void print(int a, int b, int n)
{
    for (int k = 0; k <= n; k++)
    {
        cout << BinomCoef(n, k) * pow(a, n - k) * pow(b, k) << " ";
    }
}

int main()
{
    int a, b, n;
    cout<<"Enter a, b and n: ";
    cin>>a>>b>>n;
    cout<<"("<<a<<"x+"<<b<<")^"<<n<<"\nCoef:\n";
    print(a, b, n);
    return 0;
}

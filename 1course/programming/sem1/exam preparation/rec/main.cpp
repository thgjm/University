#include <iostream>

using namespace std;

float power(int x, int n)
{
    if(n==0) return 1;
    if(n<0) return 1/power(x, -n);
    if(n%2==0) { float b = power(x, n/2); return b*b;}
    return x*power(x, n-1);

}

int main()
{
    int n;
    cin>>n;

    return 0;
}

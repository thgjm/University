//1 задача

#include <iostream>

using namespace std;

long long int fact(int n)
{
    if(n<2) return 1;
    return n*fact(n-2);
}

int main()
{
    int n, k=0;
    cout<<"enter number: ";
    cin>>n;
    long long int n1=fact(n);
    while(n1%3==0)
    if(n1%3==0) { n1/=3; k++; }
    cout<<"The amount of times number " << n << "!! divides by 3 = "  <<k;
    return 0;
}

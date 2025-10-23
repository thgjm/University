#include <iostream>
#include <cmath>

using namespace std;

bool prime(int n)
{
    bool symb=true;
    if(n==1) symb=false;
    for(int i=2; i<=sqrt(n); i++)
        if(n%i==0) symb=false ;
    return symb;
}

int main()
{
    int n, sum=0;
    cin>>n;
    for(int i=1; i<=n/2; i++)
    {
        if(n%i==0 && prime(i)) { cout<<i<<" "; sum+=i; }
    }
    if(prime(n)) sum+=n;
    cout<<endl<<sum;
    return 0;
}

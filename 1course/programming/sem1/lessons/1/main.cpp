#include <iostream>

using namespace std;

int digits_n(int n)
{
    int k;
    while(n!=0)
    {
        n/=10;
        k++;
    }
    return k;
}

int main()
{
    int n;
    cin>>n;
    int dig=digits_n(n);
    int* a=new int[dig];
    int res1=0, res2=0;
    for(int i=0; i<dig; i++)
    {
        a[dig-1-i]=n%10;
        n/=10;
    }
    for(int i=0; i<dig; i+=2)
    {
        res1*=10;
        res1+=a[i];
    }
    cout<<res1<<endl;
     for(int i=1; i<dig; i+=2)
    {
        res2*=10;
        res2+=a[i];
    }
    cout<<res2;
    return 0;
}

//числа каталана


#include <iostream>

long long int global;

using namespace std;

long long int catalane(int n)
{
    if(n==0) return 1;
    long long int res=0;
    for(long long int i=0; i<n;i++)
        res+=catalane(i)*catalane(n-1-i);
    return res;
}

int main()
{
    int n;
    cin>>n;
    long long int *arr=new long long int[n];
    for(int i=0; i<n; i++) arr[i]=0;
    arr[0]=1;
    for(int i=1; i<n; i++)
    {
        for(int j=0; j<i; j++)
            arr[i]+=arr[j]*arr[i-1-j];

    }
    cout<<arr[n-1]<<endl;
    return 0;
}

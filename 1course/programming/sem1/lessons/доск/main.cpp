#include <iostream>

using namespace std;

int main()
{
    int n, sum=0;
    cin>>n;
    for(int i=1; i<=n;i++)
    {
        for(int j=1; j<i;j++) if(!(i%j)) sum+=j;
        if(i==sum) cout<<i<<" ";
        sum=0;
    }
    return 0;
}

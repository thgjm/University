#include <iostream>

using namespace std;

int main()
{
    int numb, k=0;
    cin>>numb;
    while(numb%2==0)
    {
        numb/=2;
        k++;
    }
    cout<<k;
    return 0;
}

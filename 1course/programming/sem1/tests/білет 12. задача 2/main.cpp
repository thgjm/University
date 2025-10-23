//2 задача

#include <iostream>

using namespace std;

int main()
{
    int n;
    cout<<"Enter number: ";
    cin>>n;
    for(int a=0; a<n*2; a++)
        for(int b=0; b<n*2; b++)
    if(n==(a*a*a-b*b*b)) { cout<<"Number " << n << "=" << a <<"^3-" << b <<"^3="<< a*a*a << "-" <<b*b*b; cout<<endl<<a<<" and " << b; return 0;}
    cout<<"Impossible";
    return 0;
}

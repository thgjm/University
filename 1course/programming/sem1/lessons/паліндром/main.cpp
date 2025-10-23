#include <iostream>

using namespace std;

/*int main()
{
    int numb;
    cin >> numb;
    int inv=0;
    int nn=numb;
    while(numb!=0)
    {
        inv=inv*10+numb%10;
        numb/=10;
    }
    cout<<(inv==nn);
    return 0;
}*/

int main(){
    string str;
    cin>>str;
    int i=str[0], j=str[str.size()-1];
    while(i!=j || i<j)
    {
        if(i!=j) { cout<<"not a palindrom"; return 0;}
    }
    cout<<"palindrom";
    return 0;
}

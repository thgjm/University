#include <iostream>

using namespace std;

int main()
{
    int k=0;
    char* a;
    cin>>a;
    if(a[0]==' ')
        while(a[0]==' ')
        {
            for(int i=0; i<a; i++) { a[i]=a[i+1]; cout<<a[i]; }
        }
   /* for(int i=0; i<a.size(); i++)
    {
        if(a[i]==' ' && a[i+1]==' ') { for(int j=i+1; j<a.size()-1; j++) a[j]=a[j+1];  i--; k++;}
    }
    for(int i=a.size()-1; i>a.size()-1-k; i--)
        a[i]=' ';
    cout<<a; */
    return 0;
}

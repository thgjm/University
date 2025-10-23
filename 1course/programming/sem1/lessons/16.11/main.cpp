#include <iostream>
#include <string>

using namespace std;

int main()
{
    int p=1;
    string a;
    string res;
    cin>>a;
    int s=a.size();
    for(int i=s-1;i>=0; i--)
    {   int temp=(int)a[i]-'0'+p;
        res = to_string(temp%2) + res;
        p=temp/2;
    }
    if(p==1) res=to_string(1)+res;
    cout<<res;
    return 0;
}

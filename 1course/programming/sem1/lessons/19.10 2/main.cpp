//

#include <iostream>

using namespace std;

void f(string s, int dif, int n)
{
    if(n%2!=0) return;
    int num_left=n-s.size();
    if(num_left==0)
    {
        cout<<s<<endl;
        return;
    }
    if (dif>0) f(s+')', dif-1, n);
    if(dif<num_left) f(s+'(', dif+1, n);
}

int main()
{
    int n;
    cin>>n;
    f("",0,n);
    return 0;
}

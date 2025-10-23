#include <iostream>

using namespace std;

int main()
{
    string str = "9+5+6-8";
    string b="";
    int a = 0;
    for(int i=0; i<str.size(); i++)
    {
        if(str[i]!='+' || str[i]!='-') { b+=str[i]; cout<<" ";}
        else if (str[i] == '+') a+= b - '0';
    }
    cout<<a;
    return 0;
}

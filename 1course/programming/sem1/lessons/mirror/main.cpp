#include <iostream>

using namespace std;

int main()
{
    string a;
    getline(cin, a);
    for(int i=a.size()-1; i>-1; i--)
    {
        cout<<a[i];
    }
    return 0;
}

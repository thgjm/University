#include <iostream>

using namespace std;

int main()
{
    string in;
    getline(cin, in);
    for(int i=0; i<in.size(); i++)
    {
        if(!(in[i]<='9' && in[i]>='0')) cout<<in[i];
    }
    return 0;
}

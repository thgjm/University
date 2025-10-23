#include <iostream>

using namespace std;

int main()
{
    string in;
    int len=0;
    getline(cin, in);
    if(in[0]!=' ') { cout<<in[0]; len++; }
    for(int i=1; i<in.size(); i++)
    {
        if(in[i]==' ')
        {
            if(in[i-1]!=' ')
            cout<<len<<endl;
            len=0;
        }
        else { cout<<in[i]; len++; }
    }
    if(len>0) cout<<len;
    return 0;
}

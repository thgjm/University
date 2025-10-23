#include <iostream>
#include <string>

using namespace std;

int main()
{
    string numb1, numb2;
    cout << "Enter 2 numbers\n";
    cin >> numb1 >> numb2;
    while(numb1.length()!=numb2.length())
    {
        if(numb1.length()>numb2.length())
            numb2 = "0"+ numb2;
        else numb1 = "0" + numb1;
    }
    int sum, rem=0;
    string res="";
    for(int i=numb1.length()-1; i>=0; i--)
    {
        sum=numb1[i]-'0'+numb2[i]-'0'+rem;
        rem=sum/10;
        res=to_string(sum % 10) + res;
    }
    if(rem>0) res=to_string(rem) + res;
    cout << "Sum = " << res;
    return 0;
}

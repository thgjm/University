//вивести всі прості числа до якогось числа (100)

#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    cout<<"2 ";
    for(int n=3; n<=100; n+=2)
    {
        bool symb=true;
        for(int i=3; i<sqrt(n) && symb; i+=2)
            if(n % i == 0) symb=false;
        if(symb) cout<<n<<" ";
    }
    return 0;
}

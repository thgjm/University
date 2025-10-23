#include <iostream>

using namespace std;

int f(int x, int y)
{
    if(x==1 || y==1) return 1;
    return f(x-1, y-1)+f(x, y-1) + f(x-1, y);

}

int main()
{
    int n;
    cin>>n;
    cout<<f(n,n);
    return 0;
}

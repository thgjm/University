#include <iostream>

using namespace std;

int length(int x1, int x2, int y1, int y2)
{
    return ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

int main()
{
    int x1, x2, y1, y2, x3, y3;
    cout<<"enter the first point: ";
    cin>>x1>>y1;
    cout<<"enter the second point: ";
    cin>>x2>>y2;

    return 0;
}

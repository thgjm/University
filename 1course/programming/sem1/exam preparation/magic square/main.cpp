//визначити чи є матриця магічним квадратом

#include <iostream>

using namespace std;


int arr_i(int** arr, int n)
{
    return 1;
}

int main()
{
    int n;
    cin>>n;
    int** Arr;
    Arr = new int*[n];
    for(int i=0; i<n; i++) Arr[i]=new int[n];
    cout<<arr_i(Arr, n);
    return 0;
}

//чи є квадратна матриця симетричною

#include <iostream>
#include <clocale>

using namespace std;

bool sim(int** arr, int n)
{
    for(int i=0; i<n; i++)
       { for(int j=0; j<i; j++)
         if(arr[i][j]!=arr[j][i]) return false; }
    return true;

}

int main()
{
    setlocale(LC_ALL, "");
    int n;
    cout<<"Введiть розмiр квадратної матрицi: ";
    cin>>n;
    int** arr=new int*[n];
    for(int i=0; i<n; i++) arr[i]=new int[n];
    cout<<"Введiть елементи квадратної матрицi: ";
    for(int i=0; i<n; i++)
    for(int j=0; j<n;j++)
    cin>>arr[i][j];
    if(sim(arr, n)) cout<<"Матриця симетрична";
    else cout<<"Матриця не є симетричною";
    delete[] arr;
    return 0;
}

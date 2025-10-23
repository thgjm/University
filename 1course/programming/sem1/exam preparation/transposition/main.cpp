//транспонування квадратної матриці

#include <iostream>
#include <clocale>

using namespace std;

const int n = 4;

int **transp(int** arr)
{
    int b;
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<i; j++)
        {
            b=arr[i][j];
            arr[i][j]=arr[j][i];
            arr[j][i]=b;
        }
    }
    return **arr;

}

int main()
{
    setlocale(LC_ALL, "");
    int n;
    cin>>n;
    int arr[n][n]= { 1, 2, 3, 4,
                     8, 5, 15, 9,
                     5, 12, 6, 18,
                     6, 7, 10, 7
                   };
    cout<<"Ваша матриця:\n";
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
            cout<<arr[i][j]<<" ";
        cout<<endl;
    }
    int* b=transp(arr);
    int arr2[n][n];
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
            arr2[i][j]=*b;
            *b++;
        }
    cout<<"Транспонована матриця:\n";
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
            cout<<arr2[i][j]<<" ";
        cout<<endl;
    }
    return 0;
}

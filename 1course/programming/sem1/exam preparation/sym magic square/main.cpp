//�� � ��������� ������� �����������

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
    cout<<"����i�� ����i� ��������� ������i: ";
    cin>>n;
    int** arr=new int*[n];
    for(int i=0; i<n; i++) arr[i]=new int[n];
    cout<<"����i�� �������� ��������� ������i: ";
    for(int i=0; i<n; i++)
    for(int j=0; j<n;j++)
    cin>>arr[i][j];
    if(sim(arr, n)) cout<<"������� ����������";
    else cout<<"������� �� � �����������";
    delete[] arr;
    return 0;
}

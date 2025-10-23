//В масиві цілих додатних чисел підрахувати кількість різних чисел та надрукувати їх.

#include <iostream>
using namespace std;
int main()
{    int N;
    cout<<"enter the number of elements in array with natural numbers: ";
    cin>>N;
    cout<<"enter the elements in an array: ";
    int max_n=0, k=0;
    int arr[N];
    for(int i=0; i<N; i++)
        cin>>arr[i];
    for(int i=0; i<N; i++)
        if(arr[i]>max_n) max_n=arr[i];
    int arr2[max_n]={0};
    for(int i=0; i<N; i++)
        for(int j=0; j<max_n; j++)
            if(arr[i]==j+1) arr2[j]++;
    for(int i=0; i<max_n; i++)
        if(arr2[i]==1) k++;
    if(k) cout<<"different numbers in an array: ";
    for(int i=0; i<max_n; i++)
        if(arr2[i]==1) cout<<i+1<<" ";
    if(k) cout<<endl;
    cout<<"the amount of different numbers: "<<k;
    return 0; }

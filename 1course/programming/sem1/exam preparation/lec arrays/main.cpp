//пошук заданого числа у одновимірному масиві
#include <iostream>

using namespace std;

int main()
{
    int n, N, k=-1;
    cout<<"enter the number we gotta find: ";
    cin>>n;
    cout<<"enter the number of elements in an array: ";
    cin>>N;
    int *arr = new int[N];
    cout<<"enter the elements in an array: ";
    for(int i=0; i<N; i++)
        cin>>arr[i];
    cout<<"your array: \n";
    for(int i=0; i<N; i++)
    {
        cout<<arr[i]<< " ";
        if(arr[i]==n) k=i+1;
    }
    if(k!=-1) cout<<"\nyour number is at the place with index " << k;
    else cout<<"\nyour number is not in an array :( ";
    return 0;
}

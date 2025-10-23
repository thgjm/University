//здійснити пошук заданого числа у сортованому масиві

#include <iostream>

using namespace std;

int search_num(int arr[], int num, int min_num, int max_num)
{
    if(min_num > max_num) return -1; //число не в масиві
    if(num<arr[min_num] || num>arr[max_num]) return -1; //число не в масиві
    if(num==arr[min_num]) return min_num; //число дорівнює найменшому
    if(num==arr[max_num]) return max_num; //число дорівнює найбільшому
    int mid=(max_num+min_num)/2; //розділення масиву пополовинно
    if(num==arr[mid]) return mid; //якщо елемент дорівнює середині
    if(num>arr[mid]) return search_num(arr, num, mid+1, max_num-1); //рекурсія
    if(num<arr[mid]) return search_num(arr, num, min_num+1, mid-1);
}

int main()
{
    int N;
    cout<<"enter the number of elements in sorted array: ";
    cin>>N;
    int arr[N];
    int min_n=0, max_n=N-1;
    cout<<"enter elements in array: ";
    for(int i=0; i<N; i++)
        cin>>arr[i];
    int n;
    cout<<"the number we search: ";
    cin>>n;
    if(search_num(arr, n, min_n, max_n)==-1) cout<<"this element is not in an array";
    else cout<<"your element is at the place with index "<<search_num(arr, n, min_n, max_n);
    return 0;
}

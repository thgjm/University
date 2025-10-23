#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool Triangle(int a, int b, int c)
{
    if ((a*a + b*b > c*c) && (a*a + c*c > b*b) && (b*b + c*c > a*a)) return true;
    else return false;
}

void find_triangles(int* arr, int n)
{
    vector<vector<int>> check;
    for (int i = 0; i < n - 2; i++)
        for (int j = i + 1; j < n - 1; j++)
            for (int k = j + 1; k < n; k++)
                if (Triangle(arr[i], arr[j], arr[k]))
                {
                    vector<int> tr = {arr[i], arr[j], arr[k]};
                    if (count(check.begin(), check.end(), tr) == 0)
                    {
                        check.push_back(tr);
                        cout << "Triangle: " << tr[0] << ", " << tr[1] << ", " << tr[2] << endl;
                    }
                }
}

int main()
{
    int n;
    cout<<"Enter n: ";
    cin>>n;
    int* arr = new int[n];
    cout<<"Enter array elements: ";
    for(int i=0; i<n; i++)
        cin>>arr[i];
    sort(arr, arr+n);
    find_triangles(arr, n);
    delete[] arr;
    return 0;
}






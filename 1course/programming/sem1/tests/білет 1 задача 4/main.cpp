//білет 1 задача 4

#include <iostream>
#include <time.h>

using namespace std;

int main()
{
    srand(time(NULL));
    int n;
    cout<<"Enter n: ";
    cin>>n;
    double** a=new double*[n];
    for(int i=0; i<n; i++)
        a[i]=new double[n];
    cout<<"Matrix:\n";
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            a[i][j]=rand()%50;
            cout<<a[i][j]<<" ";
        }
        cout<<endl;
    }
    double max_row, min_col;

     for(int i=0; i<n; i++)
    {
        double max_row=a[i][0];
        double min_col=a[i][0];
        int max_j=0;
        int min_i=i;
        for(int j=0; j<n; j++)
        {
            if(a[i][j]>max_row) { max_row=a[i][j]; max_j=j; min_col=max_row;}
        }
        for(int k=0; k<n; k++)
        {
             if(a[k][max_j]<min_col) { min_col=a[k][max_j]; min_i=k;}
        }

        if(max_row == min_col) cout<<endl<<"a["<<min_i+1<<"]["<<max_j+1<<"]="<<max_row<<" ";
    }
    delete [] a;
    return 0;
}

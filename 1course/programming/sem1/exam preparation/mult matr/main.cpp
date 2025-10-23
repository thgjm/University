#include <iostream>

using namespace std;

double** mult_matr(double** a, double** b, int n)
{
    double** res = new double* [n];
    for(int i=0; i<n; i++) res[i]=new double [n];
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
    {
        res[i][j]=0;
        for(int k=0; k<n; k++) res[i][j]+=a[i][k]*b[k][j];
    }
    return res;
}

double** output_matr(double** a, int n)
{
    cout<<"Your matrix:\n";
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
            cout<<a[i][j]<<" ";
        cout<<endl;
    }
}

double** input_matr(double** a, int n)
{
    cout<<"enter matrix\n";
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++) cin>>a[i][j];
}

int main()
{
    int n;
    cout<<"enter n\n";
    cin>>n;
    double** a=new double* [n];
    for(int i=0; i<n; i++) a[i]=new double [n];
    input_matr(a, n);
    output_matr(a, n);
    cout<<endl;
    return 0;
}

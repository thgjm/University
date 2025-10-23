#include <iostream>
#include <cmath>

using namespace std;

const double delta = 0.0000001;

double function(double x)
{
    return sin(x);
}

double derivative(double x, double (*f)(double))
{
    return ((f(x + delta) - f(x))/delta);
}

long double integral(double a, double b, long long int n, double (*f)(double))
{
    double h = (b-a)/n;
    double sum=0;
    double x = a+h;
    while(x<b)
    {
        sum+=sqrt( pow(derivative(x, f), 2) + 1 );
        x+=h;
    }
    return (h/2)*(sqrt( pow(derivative(a, f), 2) + 1 )+sqrt( pow(derivative(b, f), 2) + 1 )+2*sum);
}

int main()
{
    double a, b;
    long long int n;
    cout<<"Enter a (starting point): ";
    cin>>a;
    cout<<"Enter b (ending point): ";
    cin>>b;
    cout<<"Enter n (the number of intervals): ";
    cin>>n;
    cout<<"L = "<<integral(a, b, n, function);
    return 0;
}

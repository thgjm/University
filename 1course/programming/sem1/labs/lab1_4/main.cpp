#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    long long int numb_m, numb_10=0, i=0, m;
    cout << "Enter the number and numeral system (<10)\n";
    cin >> numb_m >> m;
    cout << numb_m << "(" << m << ") = ";
    while (numb_m > 0)
    {
        numb_10 += (numb_m%10) * pow(m,i);
        numb_m /= 10;
        i++;
    }
    cout << numb_10 << "(10)";
    return 0;
}

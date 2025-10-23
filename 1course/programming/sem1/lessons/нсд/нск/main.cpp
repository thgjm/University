#include <iostream>
#include <cmath>

using namespace std;

int nsd(int n1, int n2)
{
    while(n1 != n2) {
    if(n1 > n2)
      n1 -= n2;
    else
      n2 -= n1;
  }
  return n1;
}

int nsk (int n1, int n2)
{
    return abs(n1*n2/nsd(n1, n2));
}

int main()
{
    cout << nsd(10, 84) << " " << nsk(10, 84);
    return 0;
}

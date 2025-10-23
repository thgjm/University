#include <iostream>
#include <math.h>

using namespace std;

float srt(float eps, int sum)
{
    if(sum<eps) return sum;
    else sum=sqrt(7+srt(eps, sum));
}

int main()
{
    float eps;
    cin>>eps;
    return 0;
}

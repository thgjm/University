//білет 1 задача 2

#include <iostream>

using namespace std;

struct rational{
    int num, den;
};

int nsk(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

rational reduction(rational &a)
{
    int nsk_a = nsk(a.num, a.den);
    a.num /= nsk_a;
    a.den /= nsk_a;
}

void input(rational &a)
{
    cout<<"Enter numerator: ";
    cin>>a.num;
    cout<<"Enter denominator(cannot be 0): ";
    cin>>a.den;
    return;
}

void output(rational &a)
{
    cout<<a.num<<"/"<<a.den<<endl;
    return;
}

rational add_rational(rational &a, rational &b)
{
    rational res;
    res.num=a.num*b.den+b.num*a.den;
    res.den=a.den*b.den;
    reduction(res);
    return res;
}

rational mult_rational(rational &a, rational &b)
{
    rational res;
    res.num=a.num*b.num;
    res.den=a.den*b.den;
    reduction(res);
    return res;
}

int main()
{
    rational a, b;
    input(a), input(b);
    rational c=add_rational(a, b);
    cout<<"Addition: ";
    output(c);
    rational m=mult_rational(a, b);
    cout<<"Multiplication: ";
    output(m);
    return 0;
}

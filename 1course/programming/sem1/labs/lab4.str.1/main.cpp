#include <iostream>
#include <string>

using namespace std;

bool palindrome_check(string s, int i, int j)
{
    while(j<i)
    {
        if(s[i] != s[j])
        {
            return false;
        }
        i--;
        j++;
    }
    return true;
}

string reverse_string(string s)
{
    string ss=s;
    int n = s.size();
    for(int i=0; i<n; i++)
        ss[i]=s[n-1-i];
    return ss;
}

int main()
{
    string s;
    cout<<"Enter string: ";
    cin>>s;
    int n = s.size();
    int starting_index2 = 0;
    int max_len1 = 0, max_len2 = 0;


    //паліндром на початку
    for(int i=0; i<n; i++)
    {
            if(palindrome_check(s, i, 0))
        {
            if(i+1 > max_len1)
            {
                max_len1 = i+1;
            }
        }
    }

    //паліндром на кінці
    for(int i=n-1; i>=0; i--)
    {
            if(palindrome_check(s, n-1, i))
        {
            if(n-1-i+1 > max_len2)
            {
                max_len2 = n-1-i+1;
                starting_index2 = i;
            }
        }
    }

    if(max_len1>max_len2)
    {
        int nn=n-max_len1;
        string ss=s.substr(max_len1, nn);
        s=reverse_string(ss)+s;
        cout<<s;
    }

    if(max_len2>=max_len1)
    {
        int nn=n-max_len2;
        string ss=s.substr(0, starting_index2);
        s=s+reverse_string(ss);
        cout<<s;
    }

    /*for(int i=n-1; i>=0; i--)
    {
        for(int j=i; j>=0; j--)
        {
            if(palindrome_check(s, i, j))
            {
                if(i-j+1 > max_len)
                {
                    max_len = i-j+1;
                    starting_index = j;
                }
            }
        }
    }
    cout<<"Palindrome string: \n";
    if(starting_index==0 && max_len!=n)
    {
        int nn=n-max_len;
        string ss=s.substr(max_len, nn);
        s=reverse_string(ss)+s;
        cout<<s;
    }
    else if(starting_index==n-max_len)
    {
        int nn=n-max_len;
        string ss=s.substr(0, starting_index);
        s=s+reverse_string(ss);
        cout<<s;
    }
    else
    {
        int nn=n-max_len-starting_index;
        string ss=s.substr(max_len+starting_index, nn);
        nn=n-max_len-nn;
        string sss=s.substr(0, nn);
        s.insert(starting_index, reverse_string(ss));
        s=s+reverse_string(sss);
        cout<<s;
    }*/
    return 0;
}


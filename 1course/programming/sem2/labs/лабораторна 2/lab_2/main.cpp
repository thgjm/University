#include <iostream>
#include <cmath>
using namespace std;
struct node_bottom
{
    bool val;
    node_bottom *next;
    node_bottom(bool tf) : val(tf), next(NULL) {}
};

struct node_top
{
    node_bottom *head;
    node_top *next;
    node_top(node_bottom *term) : head(term), next(NULL) {}
};

bool booleanValue(int num, int n)
{
    return (num & (1 << n)) != 0;
}

void add_element_top(node_top *&func, node_bottom *term)
{
    node_top *ell = new node_top(term);
    node_top *tmp = func;
    if (func == NULL)
    {
        func = ell;
        return;
    }
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = ell;
}

void add_element_bottom(node_bottom *&func, bool val)
{
    node_bottom *ell = new node_bottom(val);
    node_bottom *tmp = func;
    if (func == NULL)
    {
        func = ell;
        return;
    }
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = ell;
}

void print_table(node_top*& func, int n)
{
    int num_of_val = log2(n);
    for (int i = 1; i <= num_of_val; i++)
        cout << "X" << i << "\t";
    cout << "| F\n";

    for (int i = 0; i < num_of_val; i++)
        cout << "--------";
    cout << "----\n";

    FILE *fp = fopen("func.txt", "r");
    char temp;

    for (int i = 0; i < n; i++)
    {
        bool check_1_0 = false, check_1=false;
        if (fp)
        {
            while (fread(&temp, 1, 1, fp))
                if (fp && (temp == '1' || temp == '0'))
                {
                    check_1_0 = true;
                    if(temp == '1') check_1 = true;
                    break;
                }
        }
        node_bottom* value = NULL;
        for (int j = num_of_val - 1; j >= 0; j--)
        {
            if(check_1_0)
            {
                cout << booleanValue(i, j) << "\t";
                if(check_1) add_element_bottom(value, booleanValue(i, j));
            }
        }
        if(check_1_0) cout<<"| "<<temp<<"\n";
        if(check_1) add_element_top(func, value);
    }
    fclose(fp);
}

void print_dnf(node_top *func)
{
    while(func)
    {
        node_bottom* temp=func->head;
        cout<<"(";
        while(temp)
        {
            cout<<temp->val;
            temp=temp->next;
        }
        cout<<")";
        func=func->next;
    }
}

int main()
{
    node_top *dnf = NULL;
    char n_temp;
    int n = 0;
    FILE *fp = fopen("func.txt", "r");
    if (fp)
    {
        while (fread(&n_temp, 1, 1, fp))
            if ((n_temp == '1' || n_temp == '0'))
                n++;
    }
    fclose(fp);
    if (log2(n) - static_cast<int>(log2(n)) != 0)
    {
        cout << "Incorrect.";
        return 0;
    }
    cout<<"The truth table: \n";
    print_table(dnf, n);
    if(dnf!=NULL)
    {
        cout<<"\nThe DNF form: ";
        print_dnf(dnf);
    }
    return 0;
}

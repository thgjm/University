/*задача 2. варіант 1.*/

#include <iostream>

using namespace std;

struct node
{
    int data;
    node* next;
    node(int val): data(val), next(NULL) {}
};

void add_element_last(node*& p, int val)
{
    node* ell=new node(val);
    node* temp=p;
    if(p==NULL) { p=ell; return;}
    while(temp->next!=NULL)
        temp=temp->next;
    temp->next=ell;
}

void print_list(node* p)
{
    if(p==NULL) return;
    while(p)
    {
        cout<<p->data<<" ";
        p=p->next;
    }
    cout<<endl;
}

void odd_even(node* p, node*& list_odd, node*& list_even)
{
    if(p==NULL) return;
    while(p)
    {
        if(p->data%2) add_element_last(list_odd,p->data);
        else add_element_last(list_even, p->data);
        p=p->next;
    }
    return;
}



int main()
{
    node* mylist=NULL;
    node* list_even=NULL, *list_odd=NULL;
    for(int i=0; i<10; i++)
        add_element_last(mylist, rand()%50);
    print_list(mylist);
    odd_even(mylist, list_odd, list_even);
    print_list(list_odd);
    print_list(list_even);
    delete mylist;
    return 0;
}

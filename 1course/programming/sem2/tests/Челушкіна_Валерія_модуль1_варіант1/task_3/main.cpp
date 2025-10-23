/*задача 3. варіант 1.*/


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
    if(p==NULL)
    {
        p=ell;
        p->next=p;
        return;
    }
    while(temp->next!=p)
        temp=temp->next;
    temp->next=ell;
    ell->next=p;
}

void print_list(node* p)
{
    node* temp=p;
    if(p==NULL) return;
    do
    {
        cout << temp->data << " ";
        temp = temp->next;
    }
    while (temp != p);
    cout << endl;
}

void delete_subsequences(node*& p)
{
    if(p==NULL) return;
    node* temp=p;
    node* prev=NULL;
    node*ttmp=p;
    while(ttmp->next!=p)
        ttmp=ttmp->next;
    prev=ttmp;
    while(temp->next!=p)
    {
        if(temp->data == (temp->next)->data)
        {
            int cur = temp->data;
            while((temp->next)->data == cur)
            {
                node*tmp=temp->next->next;
                temp->next->next=NULL;
                delete temp->next;
                temp->next=tmp;
            }


        }
        prev=prev->next;
        temp=temp->next;
    }
}

int main()
{
    node* mylist=NULL;
    add_element_last(mylist, 1);
    add_element_last(mylist, 2);
    add_element_last(mylist, 3);
    add_element_last(mylist, 3);
    add_element_last(mylist, 3);
    add_element_last(mylist, 3);
    add_element_last(mylist, 3);
    add_element_last(mylist, 5);
    add_element_last(mylist, 5);
    add_element_last(mylist, 5);
    add_element_last(mylist, 5);
    add_element_last(mylist, 6);
    add_element_last(mylist, 9);
    add_element_last(mylist, 9);
    add_element_last(mylist, 9);
    add_element_last(mylist, 9);
    print_list(mylist);
    delete_subsequences(mylist);
    print_list(mylist);
    return 0;
}

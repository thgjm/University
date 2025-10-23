#include <iostream>
#include <limits.h>

using namespace std;

struct node
{

    int data;
    node* next;
    node(int val):data(val), next(NULL) {}
};

struct list
{
    node* head;

    void add_incr(list &res, node*& tmp, int &last_added)
    {
        if(last_added<tmp->data)
            res.add_last(tmp->data);
        last_added=tmp->data;
        tmp=tmp->next;
    }

public:

    list():head(NULL) {}

    void add_elem(int val)
    {
        if(head==NULL)
            head=new node(val);
        else
        {
            node* new_head=new node(val);
            new_head->next=head;
            head=new_head;
        }
    }

    void print()
    {
        node* tmp=head;
        while(tmp!=NULL)
        {
            cout<<tmp->data<<" ";
            tmp=tmp->next;
        }
        cout<<endl;
    }

    int last()
    {
        node* tmp=head;
        while(tmp->next != NULL)
            tmp=tmp->next;
        return tmp->data;
    }

    void del_last()
    {
        node* tmp=head;
        if(head->next==NULL)
        {
            delete head;
            return;
        }
        while(tmp->next->next != NULL)
            tmp=tmp->next;
        node* ttmp=tmp->next;
        delete ttmp;
        tmp->next=NULL;
    }

    void add_last(int val)
    {
        node* ell = new node(val);
        node* tmp=head;
        if(head==NULL)
        {
            head=ell;
            return;
        }
        while(tmp->next!=NULL) tmp=tmp->next;
        tmp->next=ell;
    }

    void del_head()
    {
        if(head==NULL) return;
        node* tmp=head;
        head=head->next;
        delete tmp;
    }

    void reverse_list()
    {
        node* temp_head=NULL;
        node* cur=head;
        while(cur!=NULL)
        {
            node* temp_cur=cur;
            cur=cur->next;
            temp_cur->next=temp_head;
            temp_head=temp_cur;
        }
        head=temp_head;
    }

    void cyclic_shift_left()
    {
        if(head==NULL) return;
        node* temp_head=head;
        head=head->next;
        temp_head->next=NULL;
        node* cur=head;
        while(cur->next!=NULL)
            cur=cur->next;
        cur->next=temp_head;
    }

    void cyclic_shift_right()
    {
        node* prev = head;
        node* cur=prev->next;
        if(cur==NULL) return;
        while(cur->next!=NULL)
        {
            prev=cur;
            cur=cur->next;
        }
        prev->next=NULL;
        cur->next=head;
        head=cur;
    }

    void insert(int val, int findVal)
    {
        node* temp=head;
        while(temp->next!=NULL)
        {
            if(temp->data==findVal)
                break;
            temp=temp->next;
        }
        if(temp->next==NULL) return;
        node* newnode=new node(val);
        newnode->next=temp->next;
        temp->next=newnode;
    }

    void del_val(int val)
    {
        node* temp=head;
        node* prev=NULL;
        while(temp)
        {
            if(temp->data==val)
                break;
            prev=temp;
            temp=temp->next;
        }
        if(temp==NULL) return;
        if(prev==NULL) del_head();
        else
        {
            prev->next = prev->next->next;
            delete temp;
        }
    }



    list union_no_repeat(const list &my_list1)
    {
        node* tmp1=head;
        node* tmp2=my_list1.head;
        list res;
        int last_added=INT_MIN;
        while(tmp1!=NULL && tmp2!=NULL)
        {
            if(tmp1->data < tmp2->data)
                add_incr(res, tmp1, last_added);
            else
            add_incr(res, tmp2, last_added);
        }
        while(tmp1!=NULL)
             add_incr(res, tmp1, last_added);
        while(tmp2!=NULL)
             add_incr(res, tmp2, last_added);
        return res;
    }

    list intersect(const list &my_list1)
    {
        node* tmp1=head;
        node* tmp2=my_list1.head;
        list res;
        int last_added=INT_MIN;
        while(tmp1!=NULL && tmp2!=NULL)
        {
            if(tmp1->data!=tmp2->data)
            {   if(tmp1->data < tmp2->data)
                    tmp1=tmp1->next;
                else
                    tmp2=tmp2->next;
            }
            else
                add_incr(res, tmp1, last_added);
        }
        return res;
    }

    list difference(const list &my_list1)
    {
        node* tmp1=head;
        node* tmp2=my_list1.head;
        list res;
        int last_added=INT_MIN;
        while(tmp1!=NULL && tmp2!=NULL)
        {
            if(tmp1->data==tmp2->data)
            {
                last_added=tmp1->data;
                tmp1=tmp1->next;
                tmp2=tmp2->next;
            }
            else
                {
                    if(tmp1->data < tmp2->data)
                        add_incr(res, tmp1, last_added);
                    else
                        tmp2=tmp2->next;
                }
        }
        while(tmp1!=NULL) add_incr(res, tmp1, last_added);
        return res;
    }

};

int main()
{
    list my_list1;
    int cur_val=rand()%5;
    for(int i=0; i<20; i++)
    {
        my_list1.add_last(cur_val);
        cur_val=cur_val+rand()%5;
    }
    my_list1.print();

    my_list1.insert(1, 3);
    my_list1.print();

    //list my_list2;
    //cur_val=rand()%5;
    //for(int i=0; i<20; i++)
    //{
    //    my_list2.add_last(cur_val);
    //    cur_val=cur_val+rand()%5;
    //}
    //my_list2.print();
    /*my_list.reverse_list();
    my_list.print();
    my_list.cyclic_shift_left();
    my_list.print();
    my_list.cyclic_shift_right();
    my_list.print();
    my_list.insert(1, 4);
    my_list.print();
    my_list.del_val(5);
    my_list.print();
    my_list.del_val(0);
    my_list.print();
    my_list.del_val(9);
    my_list.print();*/

    /*list res=my_list1.union_no_repeat(my_list2);
    cout<<"-------------------"<<endl;
    res.print();
    res=my_list1.intersect(my_list2);
    cout<<"-------------------"<<endl;
    res.print();
    res=my_list1.difference(my_list2);
    cout<<"-------------------"<<endl;
    res.print();*/
    return 0;
}

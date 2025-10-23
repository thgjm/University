#include <iostream>

using namespace std;

struct node
{
    int data;
    node* l;
    node* r;
    node* parent;
    node(int val): data(val), l(NULL), r(NULL), parent(NULL) {}
};

class bin_tree
{
    node* head;
    public:
    bin_tree():head(NULL){};
    void add(int data)
    {
        node* to_ins=new node(data);
        if(head==NULL)
        {
            head=to_ins;
            return;
        }
        node* tmp=head;
        while(true)
        {
            if (tmp->data>data)
            {
                if(tmp->l!=NULL)
                    tmp=tmp->l;
                else
                {
                    tmp->l=to_ins;
                    to_ins->parent=tmp;
                    break;
                }
            }
            else
            {
                if(tmp->r!=NULL)
                    tmp=tmp->r;
                else
                {
                    tmp->r=to_ins;
                    to_ins->parent=tmp;
                    break;
                }
            }
        }
    }

    void print_sorted()
    {
        print_sorted_internal(head);
    }

    void print_sorted_internal(node* p)
    {
        if (p==NULL) return;
        print_sorted_internal(p->l);
        cout<<p->data<<" ";
        print_sorted_internal(p->r);
    }

    bool element_exist(int val)
    {
        node* tmp=head;
        while(tmp!=NULL)
        {
            if(val==tmp->data) return true;
            if(val<tmp->data)
                tmp=tmp->l;
            else
                tmp=tmp->r;
        }
        return false;
    }

    int number_of_elements()
    {
        return number_of_elements_internal(head);
    }

    int number_of_elements_internal(node* p)
    {
        if(p==NULL) return 0;
        return number_of_elements_internal(p->l)+number_of_elements_internal(p->r)+1;
    }

    int max_depth()
    {
        return max_depth_internal(head);
    }

    int max_depth_internal(node* p)
    {
        if(p==NULL) return 0;
        return 1+max(max_depth_internal(p->l), max_depth_internal(p->r));
    }

    int sum_of_elements()
    {
        return sum_of_elements_internal(head);
    }

    int sum_of_elements_internal(node* p)
    {
        if(p==NULL) return 0;
        return sum_of_elements_internal(p->l)+sum_of_elements_internal(p->r)+p->data;
    }

    node* find_element(int val)
    {
        node* tmp=head;
        while(tmp!=NULL)
        {
            if(val==tmp->data) return tmp;
            if(val<tmp->data)
                tmp=tmp->l;
            else
                tmp=tmp->r;
        }
        return NULL;
    }

    void del_leaf(node* ttmp)
    {
        node* par=ttmp->parent;
        if(par==NULL)
        {
            delete ttmp;
            head=NULL;
            return;
        }
        if(par->l==ttmp) par->l=NULL;
        if(par->r==ttmp) par->r=NULL;
        delete ttmp;
        return;
    }

    void remove_the_element(int val)
    {
        node* tmp=find_element(val);
        if(tmp==NULL) return;
        node* ttmp=tmp->l;
        if(ttmp!=NULL)
        {

            while(ttmp->r!=NULL || ttmp->l!=NULL)
        {
            if(ttmp->r!=NULL) ttmp=ttmp->r;
            else ttmp=ttmp->l;

        }
        swap(tmp->data, ttmp->data);
        del_leaf(ttmp);
        return;
        }
        ttmp=tmp->r;
        if(ttmp!=NULL)
        {
            while(ttmp->r!=NULL || ttmp->l!=NULL)
        {
            if(ttmp->l!=NULL) ttmp=ttmp->l;
            else ttmp=ttmp->r;

        }
        swap(tmp->data, ttmp->data);
        del_leaf(ttmp);
        return;
        }
        del_leaf(tmp);
    }

};

int main()
{
    bin_tree tree;
    for(int i=0; i<8; i++)
        tree.add(rand()%20);
    tree.print_sorted();
    cout<<"\n\n";
    //cout<<tree.element_exist(10);
    //cout<<endl<<tree.number_of_elements();
    //cout<<endl<<tree.max_depth();
    //cout<<endl<<tree.sum_of_elements();
    cout<<endl;
    int num_to_del;
    cin>>num_to_del;
    tree.remove_the_element(num_to_del);
    tree.print_sorted();
    return 0;
}

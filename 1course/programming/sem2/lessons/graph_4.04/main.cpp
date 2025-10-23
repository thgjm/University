#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>
#include <string>
#include <set>
using namespace std;

struct Node
{
  std::set<Node*> from;
  std::set<Node*> to;
};
class Graph
{
  std::vector<Node*> list;
  public:
  int read_from_file(string name)
  {
    ifstream inputFile(name.c_str());
    if (!inputFile.is_open()) return -1;
    list.clear();
    int size;
    inputFile>>size;
    for (int i=0;i<size;i++) list.push_back(new Node);
    for (int i=0;i<size;i++)
      for (int j=0;j<size;j++)
        {
          int is_connected;
          inputFile>>is_connected;
          if (i!=j&&is_connected==1)
          {
            list[i]->from.insert(list[j]);
            list[j]->to.insert(list[i]);
          }
        }
    return 0;
  }



  void del_node(unsigned int index)
  {
    if (list.size()<=index) return;
    Node* to_del=list[index];
    for (set<Node*>::iterator it=to_del->from.begin();it!=to_del->from.end();it++)
      for (set<Node*>::iterator it_internal=(*it)->to.begin();it_internal!=(*it)->to.end();it_internal++)
      (*it)->to.erase(to_del);

    for (set<Node*>::iterator it=to_del->to.begin();it!=to_del->to.end();it++)
      (*it)->from.erase(to_del);

    list.erase(list.begin()+index);
  }

  void merge_nodes(unsigned int index1, unsigned int index2)
  {
      if (list.size()<=index1||list.size()<=index2) return;
      Node* to_merge=list[index1];
    Node* to_del=list[index2];
    for (set<Node*>::iterator it=to_del->from.begin();it!=to_del->from.end();it++)

    {
        (*it)->to.erase(to_del);
        (*it)->to.insert(to_merge);
    }

    for (set<Node*>::iterator it=to_del->to.begin();it!=to_del->to.end();it++)

    {
        (*it)->from.erase(to_del);
        (*it)->from.insert(to_merge);
    }
    list.erase(list.begin()+index2);
  }


};


int main()
{
  Graph gr;
  gr.read_from_file("test_graph.txt");
  gr.del_node(0);
  //gr.merge_nodes()
  _getch();
}

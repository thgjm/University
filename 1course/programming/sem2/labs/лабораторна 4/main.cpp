#include <iostream>
#include <vector>
#include <stack>
#include <fstream>

using namespace std;

class Graph
{
  int V;
  vector<vector<pair<int, int>>> adj;
  void fillStack(int v, bool visitedV[], stack<int> &Stack);
  void DFS(int v, bool visitedV[]);
  void merge(vector<pair<int, int>> &vec, int l, int m, int r);

public:
  Graph(int V);
  void addEdge(int v, int w, int weight);
  void MakeGraph();
  void printSCC(int &scc);
  Graph transpose();
  int sumOfVertex(int v);
  void bubbleSort(vector<pair<int, int>> &vec);
  void mergeSort(vector<pair<int, int>> &vec, int l, int r);
  void printGraph();
  void printGraphSorted();
};

Graph::Graph(int V) : V(V), adj(V) {}

void Graph::MakeGraph()
{
  int num = 0, weight = 0, vert = 0;
  ifstream fp("../test_graph2.txt");
  if (!fp.is_open())
    return;
  string line;
  while (getline(fp, line))
  {
    int i = 0;
    // cout<<vert<<": ";
    while (line[i])
    {
      if (line[i] == ' ')
      {
        i++;
        continue;
      }
      while (isdigit(line[i]))
      {
        num = num * 10 + (line[i] - '0');
        i++;
      }
      // cout << num;
      if (line[i] == '(')
      {
        i++;
        while (line[i] != ')')
        {
          weight = weight * 10 + (line[i] - '0');
          i++;
        }
      }
      // cout << "(" << weight << "); ";
      adj[vert].push_back(make_pair(num, weight));
      num = 0;
      weight = 0;
      i++;
    }
    vert++;
    // cout << '\n';
  }
  fp.close();
}

void Graph::DFS(int v, bool visitedV[])
{
  visitedV[v] = true;
  cout << v << " ";

  for (auto i = adj[v].begin(); i != adj[v].end(); i++)
    if (!visitedV[i->first])
      DFS(i->first, visitedV);
}

Graph Graph::transpose()
{
  Graph g(V);
  for (int v = 0; v < V; v++)
    for (auto i = adj[v].begin(); i != adj[v].end(); i++)
      g.adj[i->first].push_back(make_pair(v, i->second));
  return g;
}

void Graph::addEdge(int v, int w, int weight)
{
  adj[v].push_back(make_pair(w, weight));
}

void Graph::fillStack(int v, bool visitedV[], stack<int> &Stack)
{
  visitedV[v] = true;

  for (auto i = adj[v].begin(); i != adj[v].end(); i++)
    if (!visitedV[i->first])
      fillStack(i->first, visitedV, Stack);

  Stack.push(v);
}

void Graph::printSCC(int &scc)
{
  stack<int> Stack;

  bool *visitedV = new bool[V];

  for (int i = 0; i < V; i++)
    visitedV[i] = false;

  for (int i = 0; i < V; i++)
    if (visitedV[i] == false)
      fillStack(i, visitedV, Stack);

    /*stack<int> Stack1;
    Stack1=Stack;

while (!Stack1.empty())
  {
      int v=Stack1.top();
    cout<<v<<" ";
    Stack1.pop();
  }
  cout<<"\n\n";*/

  Graph graph_rev = transpose();

  for (int i = 0; i < V; i++)
    visitedV[i] = false;

  while (!Stack.empty())
  {
    int v = Stack.top();
    Stack.pop();

    if (visitedV[v] == false)
    {
      graph_rev.DFS(v, visitedV);
      scc++;
      cout << endl;
    }
  }
  delete[] visitedV;
}

int Graph::sumOfVertex(int v)
{
  int sum = 0;
  for (auto i = adj[v].begin(); i != adj[v].end(); i++)
    sum += i->second;
  return sum;
}

void Graph::bubbleSort(vector<pair<int, int>> &vec)
{
  int n = vec.size();
  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - i - 1; j++)
      if (vec[j].second > vec[j + 1].second)
        swap(vec[j], vec[j + 1]);
}

void Graph::merge(vector<pair<int, int>> &vec, int l, int m, int r)
{
  int n1 = m - l + 1;
  int n2 = r - m;
  vector<pair<int, int>> L(n1), R(n2);

  for (int i = 0; i < n1; i++)
    L[i] = vec[l + i];
  for (int j = 0; j < n2; j++)
    R[j] = vec[m + 1 + j];


  int i = 0;
  int j = 0;
  int k = l;
  while (i < n1 && j < n2)
  {
    if (L[i].second <= R[j].second)
    {
      vec[k] = L[i];
      i++;
    }
    else
    {
      vec[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1)
  {
    vec[k] = L[i];
    i++;
    k++;
  }

  while (j < n2)
  {
    vec[k] = R[j];
    j++;
    k++;
  }
}

void Graph::mergeSort(vector<pair<int, int>> &vec, int l, int r)
{
  if (l < r)
  {
    int m = l + (r - l) / 2;
    mergeSort(vec, l, m);
    mergeSort(vec, m + 1, r);
    merge(vec, l, m, r);
  }
}

void Graph::printGraphSorted()
{

  vector<pair<int, int>> vertexSum;
  for (int v = 0; v < V; v++)
    vertexSum.push_back(make_pair(v, sumOfVertex(v)));

  //bubbleSort(vertexSum);

  mergeSort(vertexSum, 0, V - 1);

  for (auto iter = vertexSum.begin(); iter != vertexSum.end(); iter++)
  {
    cout << "vertex " << iter->first << ": ";
    for (auto i = adj[iter->first].begin(); i != adj[iter->first].end(); i++)
      cout << i->first << "(" << i->second << "); ";
    cout << "-- " << iter->second << endl;
  }
}

void Graph::printGraph()
{
  for (int v = 0; v < V; v++)
  {
    cout << "vertex " << v << ": ";
    for (auto i = adj[v].begin(); i != adj[v].end(); i++)
      cout << i->first << "(" << i->second << "); ";
    cout << "-- " << sumOfVertex(v) << endl;
  }
}

int count_vertexes()
{
  int vert = 0;
  ifstream fp("../test_graph2.txt");
  if (!fp.is_open())
    return -1;
  string line;
  while (getline(fp, line))
    vert++;
  fp.close();
  return vert;
}

int main()
{
  int scc = 0;
  int V = count_vertexes();
  Graph graph(V);
  graph.MakeGraph();
  /*graph.addEdge(0, 1, 1);
  graph.addEdge(0, 2, 1);
  graph.addEdge(0, 3, 1);
  graph.addEdge(0, 4, 1);
  graph.addEdge(0, 5, 1);
  graph.addEdge(1, 2, 9);
  graph.addEdge(2, 3, 4);
  graph.addEdge(3, 4, 2);
  graph.addEdge(4, 5, 6);*/

  cout << "Strongly Connected Components:\n";
  graph.printSCC(scc);
  if (scc == 1)
    cout << "The Graph is strongly connected.\n";
  else
    cout << "The Graph is not strongly connected and has " << scc << " strongly connected components.\n";
  cout << "\n\tThe graph:\n";
  graph.printGraph();
  cout << "\n\tThe sorted graph:\n";
  graph.printGraphSorted();
  return 0;
}

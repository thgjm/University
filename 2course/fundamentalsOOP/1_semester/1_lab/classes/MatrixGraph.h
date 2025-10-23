#include "Graph.h"

// -----------Graphs, represented with adjacency matrix-----------

class MatrixGraph : Graph
{
private:
  int Vertices;
  bool **adjMatrix;
  string FileName;

  void addEdge_internal(int v, int w, bool directed);
  void removeEdge_internal(int v, int w, bool directed);
  void removeVertex_internal(int v);
  void DFS(int active, bool visited[]);

public:
  MatrixGraph(int Vertices, string filename) : Graph(Vertices, filename), Vertices(Vertices), FileName(filename)
  {
    adjMatrix = new bool *[Vertices];
    for (int i = 0; i < Vertices; i++)
    {
      adjMatrix[i] = new bool[Vertices];
      for (int j = 0; j < Vertices; j++)
        adjMatrix[i][j] = false;
    }
  }

  int getVertices()
  {
    return Vertices;
  }

  void read_matrix_from_file();
  void print_matrix();
  void print_list();

  // these four methods involve interaction with the user
  void addEdge_matrix();
  void removeEdge_matrix();
  void addVertex_matrix();
  void removeVertex_matrix();

  bool isConnected_matrix();

  int shortestPath_matrix(int start, int end);

  MatrixGraph complementMatrixGraph();

  void findSpanningTree_matrix(int start);

  bool **getAdjMatrix()
  {
    return adjMatrix;
  }

  ~MatrixGraph()
  {
    for (int i = 0; i < Vertices; i++)
      delete[] adjMatrix[i];
    delete[] adjMatrix;
  }
};

// -----------Functions for "MatrixGraph"-----------

void MatrixGraph::read_matrix_from_file()
{
  ifstream fp(FileName);
  if (!fp.is_open())
  {
    cout << "Cannot open the file." << endl;
    return;
  }

  string line;
  int vertex = 0;

  while (getline(fp, line) && vertex < Vertices)
  {
    int adj_vertex = 0;
    for (char ch : line)
      if (ch == '1' || ch == '0')
      {
        if (ch == '1')
          adjMatrix[vertex][adj_vertex] = true;
        adj_vertex++;
      }
    vertex++;
  }

  fp.close();
}

void MatrixGraph::print_matrix()
{
  for (int i = 0; i < Vertices; i++)
  {
    for (int j = 0; j < Vertices; j++)
      cout << adjMatrix[i][j] << " ";
    cout << endl;
  }
}

void MatrixGraph::print_list()
{
  for (int i = 0; i < Vertices; i++)
  {
    cout << "vertex " << i << ": ";
    int check = 0;
    for (int j = 0; j < Vertices; j++)
      if (adjMatrix[i][j])
      {
        cout << j << " ";
        check++;
      }
    if (check == 0)
      cout << "∅";
    cout << endl;
  }
}

void MatrixGraph::addEdge_internal(int v, int w, bool directed)
{
  adjMatrix[v][w] = true;
  if (!directed)
    adjMatrix[w][v] = true;
}

void MatrixGraph::addEdge_matrix()
{
  int vertex1, vertex2;
  bool directed;

  cout << "Enter the two already existing vertexes between which you want to add an edge: ";

  check_the_vertex(vertex1);
  check_the_vertex(vertex2);

  is_graph_directed(directed);

  addEdge_internal(vertex1, vertex2, directed);
}

void MatrixGraph::removeEdge_internal(int v, int w, bool directed)
{
  adjMatrix[v][w] = false;
  if (!directed)
    adjMatrix[w][v] = false;
}

void MatrixGraph::removeVertex_internal(int v)
{
  while (v < Vertices)
  {
    for (int i = 0; i < Vertices; ++i)
      adjMatrix[i][v] = adjMatrix[i][v];

    for (int i = 0; i < Vertices; ++i)
      adjMatrix[v][i] = adjMatrix[v][i];
    v++;
  }
  Vertices--;
}

void MatrixGraph::addVertex_matrix()
{
  int newVerticesCount = Vertices + 1;

  bool **newAdjMatrix = new bool *[newVerticesCount];
  for (int i = 0; i < newVerticesCount; ++i)
    newAdjMatrix[i] = new bool[newVerticesCount];

  for (int i = 0; i < Vertices; ++i)
    for (int j = 0; j < Vertices; ++j)
      newAdjMatrix[i][j] = adjMatrix[i][j];

  for (int i = 0; i < newVerticesCount; ++i)
  {
    newAdjMatrix[Vertices][i] = false;
    newAdjMatrix[i][Vertices] = false;
  }

  for (int i = 0; i < Vertices; ++i)
    delete[] adjMatrix[i];
  delete[] adjMatrix;

  adjMatrix = newAdjMatrix;

  Vertices++;

  cout << "Choose what vertex you want to add:\ntype 1 if isolated vertex;\n2 - if connected to the other one;\n3 - if in between others.\n";
  int answer, vertex1, current_vertex = Vertices - 1;
  while (true)
  {
    cin >> answer;
    if (answer == 1)
      break;
    else if (answer == 2)
    {
      bool directed;
      cout << "Type in the vertex to which this one is be connected: ";
      check_the_vertex(vertex1);

      is_graph_directed(directed);

      addEdge_internal(vertex1, current_vertex, directed);
      break;
    }
    else if (answer == 3)
    {
      int vertex2;
      bool directed;
      cout << "Type in the vertexes between which this one is connected: ";

      check_the_vertex(vertex1);

      check_the_vertex(vertex2);

      is_graph_directed(directed);

      removeEdge_internal(vertex1, vertex2, directed);

      addEdge_internal(vertex1, current_vertex, directed);

      addEdge_internal(current_vertex, vertex2, directed);

      break;
    }
    else
      cout << "Incorrect input. Please try again: ";
  }
}

void MatrixGraph::removeVertex_matrix()
{
  int vertex1;

  cout << "Enter the vertex you want to remove: ";

  check_the_vertex(vertex1);

  removeVertex_internal(vertex1);
}

MatrixGraph MatrixGraph::complementMatrixGraph()
{
  MatrixGraph complementGraph(Vertices, FileName);
  for (int i = 0; i < Vertices; i++)
    for (int j = 0; j < Vertices; j++)
    {
      if (i != j && !adjMatrix[i][j])
        complementGraph.addEdge_internal(i, j, true);
    }
  return complementGraph;
}

void MatrixGraph::DFS(int active, bool visited[])
{
  visited[active] = true;
  for (int i = 0; i < Vertices; i++)
    if (adjMatrix[active][i] == 1 && !visited[i])
      DFS(i, visited);
}

bool MatrixGraph::isConnected_matrix()
{
  bool *visited = new bool[Vertices];
  for (int v = 0; v < Vertices; v++)
  {
    for (int i = 0; i < Vertices; i++)
      visited[i] = false;
    DFS(v, visited);
    for (int i = 0; i < Vertices; i++)
      if (!visited[i])
        return false;
  }
  return true;
}

int MatrixGraph::shortestPath_matrix(int start, int end)
{
  if (start < 0 || start >= Vertices || end < 0 || end >= Vertices)
  {
    cout << "Incorrect input" << endl;
    return -1;
  }

  if (start == end)
    return 0;

  vector<bool> visited(Vertices, false);
  vector<int> distance(Vertices, -1);

  vector<int> queue;
  visited[start] = true;
  distance[start] = 0;
  queue.push_back(start);

  while (!queue.empty())
  {
    int current = queue.front();
    queue.erase(queue.begin());
    for (int i = 0; i < Vertices; i++)
    {
      if (adjMatrix[current][i] && !visited[i])
      {
        visited[i] = true;
        distance[i] = distance[current] + 1;

        if (i == end)
          return distance[i];
        queue.push_back(i);
      }
    }
  }
  return -1;
}

void MatrixGraph::findSpanningTree_matrix(int start)
{
  vector<bool> visited(Vertices, false);
  vector<int> tree(Vertices, -1);
  stack<int> st;

  st.push(start);
  visited[start] = true;

  while (!st.empty())
  {
    int u = st.top();
    st.pop();

    for (int v = 0; v < Vertices; v++)
    {
      if (adjMatrix[u][v] && !visited[v])
      {
        visited[v] = true;
        tree[v] = u;
        st.push(v);
      }
    }
  }

  printSpanningTree(start, tree);
}
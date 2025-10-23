#include "MatrixGraph.h"

// -----------Graphs, represented with incendence matrix-----------

class IncendenceMatrixGraph : Graph
{
private:
  int Vertexes;
  int Edges;
  int **incMatrix;
  string filename;

  void addEdge_internal(int v, int w, bool directed);
  void removeEdge_internal(int v, int w, bool directed);
  void removeVertex_internal(int v);
  void plusOneEdge();
  void DFS(int vertex, vector<bool> &visited);

  bool is_directed();

  bool are_vertexes_connected(int vertex1, int vertex2);

public:
  IncendenceMatrixGraph(int Vertices, int Edges, string filename) : Graph(Vertices, filename), Vertexes(Vertices), Edges(Edges), filename(filename)
  {
    incMatrix = new int *[Vertexes];
    for (int i = 0; i < Vertexes; i++)
    {
      incMatrix[i] = new int[Edges];
      for (int j = 0; j < Edges; j++)
        incMatrix[i][j] = 0;
    }
  }

  int getVertexes()
  {
    return Vertexes;
  }

  int getEdges()
  {
    return Edges;
  }

  void readFromFile();
  void printIncMatrix();
  void printList();

  // these four methods involve interaction with the user
  void addEdge_incMatrix();
  void removeEdge_incMatrix();
  void addVertex_incMatrix();
  void removeVertex_incMatrix();

  bool isConnected_incMatrix(); // for undirected only

  int shortestPath_incMatrix(int start, int end);

  IncendenceMatrixGraph complementMatrixGraph();

  void findSpanningTree_matrix(int start);

  int **getIncMatrix()
  {
    return incMatrix;
  }

  ~IncendenceMatrixGraph()
  {
    for (int i = 0; i < Vertexes; i++)
      delete[] incMatrix[i];
    delete[] incMatrix;
  }
};

// -----------Functions for "IncendenceMatrixGraph"-----------

void IncendenceMatrixGraph::readFromFile()
{
  ifstream fp(filename);
  if (!fp.is_open())
  {
    cout << "Cannot open the file." << endl;
    return;
  }

  string line;
  int vertex = 0;

  while (getline(fp, line) && vertex < Vertexes)
  {
    int edges = 0;
    int i = 0;
    while (i < line.size())
    {
      if (line[i] == ' ')
      {
        i++;
        continue;
      }
      if (line[i] == '-' && line[i + 1] == '1')
      {
        incMatrix[vertex][edges] = -1;
        i++;
        i++;
        edges++;
      }
      else if (line[i] == '1' || line[i] == '0')
      {
        if (line[i] == '1')
          incMatrix[vertex][edges] = 1;
        else
          incMatrix[vertex][edges] = 0;
        edges++;
        i++;
      }
    }
    vertex++;
  }
}

void IncendenceMatrixGraph::printIncMatrix()
{
  for (int i = 0; i < Vertexes; i++)
  {
    for (int j = 0; j < Edges; j++)
      cout << incMatrix[i][j] << " ";
    cout << endl;
  }
}

void IncendenceMatrixGraph::printList()
{
  for (int i = 0; i < Vertexes; i++)
  {
    int edges = 0;
    cout << "vertex " << i << " is incendent to edges: ";
    bool check = false;
    for (int j = 0; j < Edges; j++)
      if (incMatrix[i][j] == 1 || incMatrix[i][j] == -1)
      {
        cout << j << " ";
        check = true;
      }
    if (!check)
      cout << "∅";
    cout << endl;
  }
}

void IncendenceMatrixGraph::addEdge_internal(int vertex1, int vertex2, bool directed)
{
  incMatrix[vertex1][Edges - 1] = 1;
  if (!directed)
    incMatrix[vertex2][Edges - 1] = 1;
  else
    incMatrix[vertex2][Edges - 1] = -1;
}

void IncendenceMatrixGraph::plusOneEdge()
{
  int newEdges = Edges + 1;

  int **newIncMatrix = new int *[Vertexes];
  for (int i = 0; i < Vertexes; i++)
    newIncMatrix[i] = new int[newEdges];

  for (int i = 0; i < Vertexes; i++)
    for (int j = 0; j < Edges; j++)
      newIncMatrix[i][j] = incMatrix[i][j];

  for (int i = 0; i < Vertexes; i++)
    newIncMatrix[i][newEdges - 1] = 0;

  for (int i = 0; i < Vertexes; ++i)
    delete[] incMatrix[i];
  delete[] incMatrix;

  incMatrix = newIncMatrix;

  Edges++;
}

void IncendenceMatrixGraph::addEdge_incMatrix()
{
  plusOneEdge();

  int vertex1, vertex2;

  cout << "Enter the two already existing vertexes between which you want to add an edge: ";

  check_the_vertex(vertex1);
  check_the_vertex(vertex2);

  bool directed = is_directed();

  addEdge_internal(vertex1, vertex2, directed);
}

void IncendenceMatrixGraph::removeEdge_internal(int vertex1, int vertex2, bool directed)
{
  int edgeNum = -1;
  for (int i = 0; i < Edges; i++)
  {
    if (incMatrix[vertex1][i] == -1 && incMatrix[vertex2][i] == 1)
    {
      cout << "There is no edge from vertex " << vertex1 << " to vertex " << vertex2;
      edgeNum = -2;
      break;
    }
    else if (incMatrix[vertex1][i] == 1 && incMatrix[vertex2][i] == -1)
    {
      edgeNum = i;
      for (int i = 0; i < Vertexes; i++)
        for (int j = edgeNum; j < Edges - 1; j++)
          incMatrix[i][j] = incMatrix[i][j + 1];
      break;
    }
    else if (incMatrix[vertex1][i] == 1 && incMatrix[vertex2][i] == 1)
    {
      edgeNum = i;
      if (!directed)
      {
        for (int i = 0; i < Vertexes; i++)
          for (int j = edgeNum; j < Edges - 1; j++)
            incMatrix[i][j] = incMatrix[i][j + 1];
      }

      if (directed)
        incMatrix[vertex1][i] = -1;

      break;
    }
  }
  if (edgeNum == -1)
    cout << "There is no edge between these vertixes.";
  Edges--;
}

bool IncendenceMatrixGraph::is_directed()
{
  for (int i = 0; i < Vertexes; i++)
    for (int j = 0; j < Edges; j++)
      if (incMatrix[i][j] == -1)
        return true;
  return false;
}

void IncendenceMatrixGraph::removeEdge_incMatrix()
{
  int vertex1, vertex2;
  cout << "Enter the two already existing vertexes between which you want to remove the edge: ";

  check_the_vertex(vertex1);
  check_the_vertex(vertex2);

  bool directed = is_directed();

  removeEdge_internal(vertex1, vertex2, directed);
}

void IncendenceMatrixGraph::addVertex_incMatrix()
{
  int newVertexes = Vertexes + 1;

  int **newIncMatrix = new int *[newVertexes];
  for (int i = 0; i < newVertexes; ++i)
    newIncMatrix[i] = new int[Edges];

  for (int i = 0; i < Vertexes; ++i)
    for (int j = 0; j < Edges; ++j)
      newIncMatrix[i][j] = incMatrix[i][j];

  for (int j = 0; j < Edges; j++)
    newIncMatrix[newVertexes - 1][j] = 0;

  for (int i = 0; i < Vertexes; i++)
    delete[] incMatrix[i];
  delete[] incMatrix;

  incMatrix = newIncMatrix;

  Vertexes++;

  cout << "Choose what vertex you want to add:\ntype 1 if isolated vertex;\n2 - if connected to the other one;\n3 - if in between others.\n";
  int answer, vertex1, current_vertex = Vertexes - 1;
  while (true)
  {
    cin >> answer;
    if (answer == 1)
      break;
    else if (answer == 2)
    {

      cout << "Type in the vertex to which this one is be connected: ";
      check_the_vertex(vertex1);

      bool directed = is_directed();

      plusOneEdge();

      addEdge_internal(vertex1, current_vertex, directed);
      break;
    }
    else if (answer == 3)
    {
      int vertex2;
      cout << "Type in the vertexes between which this one is connected: ";

      check_the_vertex(vertex1);

      check_the_vertex(vertex2);

      bool directed = is_directed();

      removeEdge_internal(vertex1, vertex2, directed);

      plusOneEdge();

      addEdge_internal(vertex1, current_vertex, directed);

      plusOneEdge();

      addEdge_internal(current_vertex, vertex2, directed);

      break;
    }
    else
      cout << "Incorrect input. Please try again: ";
  }
}

bool IncendenceMatrixGraph::are_vertexes_connected(int vertex1, int vertex2)
{
  if (vertex1 < 0 || vertex1 >= Vertexes || vertex2 < 0 || vertex2 >= Vertexes)
  {
    cout << "Invalid vertex index." << endl;
    return false;
  }

  for (int i = 0; i < Edges; i++)
  {
    if (i < 0 || i >= Edges)
      continue;
    if ((incMatrix[vertex1][i] == 1 || incMatrix[vertex1][i] == -1) && (incMatrix[vertex2][i] == 1 || incMatrix[vertex2][i] == -1))
      return true;
  }
  return false;
}

void IncendenceMatrixGraph::removeVertex_internal(int vertex)
{
  int edgesArray = Edges, removeEdgesCount = 0;
  int edgesToRemove[edgesArray];
  for (int i = 0; i < edgesArray; i++)
    edgesToRemove[i] = -1;

  for (int j = 0; j < Vertexes; j++)
    for (int i = 0; i < edgesArray; i++)
      if (j != vertex)
        if ((incMatrix[vertex][i] == 1 || incMatrix[vertex][i] == -1) && (incMatrix[j][i] == 1 || incMatrix[j][i] == -1))
        {
          edgesToRemove[i] = j;
          removeEdgesCount++;
        }

  if (removeEdgesCount != 0)
  {
    if (removeEdgesCount != 1)
    {
      bool directed = is_directed();

      for (int i = 0; i < edgesArray; i++)
        if (edgesToRemove[i] != -1)
          for (int j = i + 1; j < edgesArray; j++)
          {
            if (edgesToRemove[j] != -1)
              if (!are_vertexes_connected(edgesToRemove[i], edgesToRemove[j]))
              {
                plusOneEdge();
                addEdge_internal(edgesToRemove[i], edgesToRemove[j], directed);
              }
          }
    }
    for (int i = 0; i < edgesArray; i++)
    {
      if (edgesToRemove[i] != -1)
        removeEdge_internal(vertex, edgesToRemove[i], false);
    }
  }

  for (int i = vertex; i < Vertexes - 1; i++)
    for (int j = 0; j < Edges; j++)
      incMatrix[i][j] = incMatrix[i + 1][j];

  Vertexes--;

  delete[] incMatrix[Vertexes];
}

void IncendenceMatrixGraph::removeVertex_incMatrix()
{
  int vertex1;

  cout << "Enter the vertex you want to remove: ";

  check_the_vertex(vertex1);

  removeVertex_internal(vertex1);
}

bool IncendenceMatrixGraph::isConnected_incMatrix()
{

  for (int v = 0; v < Vertexes; v++)
  {
    vector<bool> visited(Vertexes, false);
    DFS(v, visited);
    for (bool v : visited)
      if (!v)
        return false;
  }

  return true;
}

void IncendenceMatrixGraph::DFS(int vertex, vector<bool> &visited)
{
  visited[vertex] = true;

  for (int edge = 0; edge < Edges; edge++)
    if (incMatrix[vertex][edge] != 0)
      for (int otherVertex = 0; otherVertex < Vertexes; otherVertex++)
        if (otherVertex != vertex && incMatrix[otherVertex][edge] != 0)
          if (!visited[otherVertex])
            DFS(otherVertex, visited);
}

IncendenceMatrixGraph IncendenceMatrixGraph::complementMatrixGraph()
{
  int totalEdges = Vertexes * (Vertexes - 1) / 2;
  int complementGraphEdges = totalEdges - Edges;
  IncendenceMatrixGraph complementGraph(Vertexes, complementGraphEdges, filename);

  int edgeIndex = 0;
  for (int i = 0; i < Vertexes; i++)
    for (int j = i + 1; j < Vertexes; j++)
      if (!are_vertexes_connected(i, j))
      {
        complementGraph.incMatrix[i][edgeIndex] = 1;
        complementGraph.incMatrix[j][edgeIndex] = 1;
        edgeIndex++;
      }
  return complementGraph;
}

int IncendenceMatrixGraph::shortestPath_incMatrix(int start, int end)
{
  if (start < 0 || start >= Vertexes || end < 0 || end >= Vertexes)
  {
    cout << "Invalid vertex index." << endl;
    return -1;
  }

  vector<int> distance(Vertexes, -2);
  distance[start] = 0;

  queue<int> q;
  q.push(start);
  while (!q.empty())
  {
    int current = q.front();
    q.pop();

    for (int edge = 0; edge < Edges; edge++)
      if (incMatrix[current][edge] != 0)
        for (int otherVertex = 0; otherVertex < Vertexes; otherVertex++)
          if ((incMatrix[otherVertex][edge] != 0 && otherVertex != current) && distance[otherVertex] == -2)
          {
            distance[otherVertex] = distance[current] + 1;
            q.push(otherVertex);

            if (otherVertex == end)
              return distance[otherVertex];
          }
  }
  return -1;
}

void IncendenceMatrixGraph::findSpanningTree_matrix(int start)
{
    vector<bool> visited(Vertexes, false);
    vector<int> tree(Vertexes, -1);
    stack<int> st;

    st.push(start);
    visited[start] = true;

    while (!st.empty())
    {
        int u = st.top();
        st.pop();

        for (int edge = 0; edge < Edges; edge++)
            if (incMatrix[u][edge] != 0)
                for (int v = 0; v < Vertexes; v++)
                    if (incMatrix[v][edge] != 0 && v != u && !visited[v])
                    {
                        visited[v] = true; 
                        tree[v] = u;
                        st.push(v);
                    }
    }

    printSpanningTree(start, tree);
}
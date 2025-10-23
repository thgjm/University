#include "classes/Classes.h"

// -----------Main function-----------

int main()
{
  //------------------Graphs (slightly interactive)------------------

  cout << "\t\t\t Graphs.\n\n";

  // string filename = enter_filename(); -- the ability for the user to enter the file that they need for themselves

  cout << "\t Graph, represented with adjacency list.\n\n"; // the same could be done with other two representations, but there will be too many repeats in the code

  string filename = "textfiles/graph_test.txt";
  int Vertices = count_vertices(filename);
  Graph graph(Vertices, "textfiles/graph_test.txt");
  graph.read_graph_from_file();
  cout << "\tOriginal graph:\n\n";
  graph.print_graph();
  cout << "Do you want to add/remove vertex or edge?\n1 - add vertex;\n2 - remove vertex;\n3 - add edge;\n4 - remove edge;\n5 - exit\n";
  int choice;
  cin >> choice;
  switch (choice)
  {
  case 1:
    graph.add_the_vertex();
    break;
  case 2:
    graph.remove_the_vertex();
  case 3:
    graph.add_the_edge();
    break;
  case 4:
    graph.remove_the_edge();
    break;
  case 5:
    break;
  default:
    cout << "Incorrect input.";
    break;
  }

  if (choice > 0 && choice < 5)
  {
    cout << "New graph: \n\n";
    graph.print_graph();
  }

  cout << "\nIs graph connected?";

  if (graph.isConnected())
    cout << "\tYes.\n";
  else
    cout << "\t\tNo.\n";

  Graph complementGraph = graph.complementGraph();
  cout << "\nComplement graph to this one: \n\n";
  complementGraph.print_graph();

  cout << "\nType in the two vertices between which you want to find a path: ";
  int v1, v2;
  cin >> v1 >> v2;
  cout << "\nThe shortest path is: " << graph.shortest_path(v1, v2);

  cout << "\n\nType in the vertex from where you want to find a spanning tree: ";
  cin >> v1;
  graph.findSpanningTree(v1);

  cout << endl
       << "The work with graphs is ended. Press Enter to continue..";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cin.get();

  //------------------Probability theory (not interactive)------------------

  cout << "\n\t\t\tProbability theory.\n\n";

  vector<double> values = {0.5, 1, 2, 4};
  vector<double> probabilities = {0.2, 0.1, 0.3, 0.4};

  cout << "\tDiscrete random value.\n";

  cout << "\nNormal distribution example:\n";
  DiscreteRandomVariable discreteRV("Event1", values, probabilities);
  printOutDRV(discreteRV);

  cout << "\nBinomial distribution example:\n";
  BinomialDistribution binomialRV("Event2", 7, 2, 0.24);
  printOutDRV(binomialRV);

  cout << "\nPoisson distribution example:\n";
  PoissonDistribution poissonRV("Event3", 4, 0.24);
  printOutDRV(poissonRV);

  cout << "\nGeometric distribution example:\n";
  GeometricDistribution geometricRV("Event4", 10, 0.5);
  printOutDRV(geometricRV);

  cout << "\n\tContinuous random value.\n";

  double a = 0.0, b = 10.0, x = 3.0;
  UniformDistribution uniform("Event5", a, b);
  cout << "\nContinuous uniform distribution example:\n";
  printOutCRV(uniform, x);

  double lambda = 2.0;
  x = 2.0;
  ExponentialDistribution exponential("Event6", lambda);
  cout << "\nExponential distribution example:\n";
  printOutCRV(exponential, x);

  cout << "\n\tCombined random events.\n";

  printOutCRE(geometricRV, binomialRV);

  return 0;
}
#include "classes/Classes.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "classes/doctest.h"

// -----------Testing-----------

TEST_CASE("Testing some functions for Graph class.")
{
  string filename = "textfiles/graph_test.txt";
  int V = count_vertices(filename);
  Graph graph(V, filename);
  graph.read_graph_from_file();

  SUBCASE("Is graph connected?")
  CHECK(graph.isConnected() == true);

  SUBCASE("Check complemented graph.")
  {
    Graph complementGraph = graph.complementGraph();

    vector<vector<int>> toExpect = {
        {4, 5},
        {3, 4, 5},
        {3, 5},
        {1, 2, 4, 5},
        {0, 1, 3},
        {0, 1, 2, 3},
    };

    vector<vector<int>> adj = complementGraph.get_graph();
    CHECK(adj == toExpect);
  }

  SUBCASE("Check shortest path.")
  {
    SUBCASE("Between 0 and 1.")
    CHECK(graph.shortest_path(0, 1) == 1);

    SUBCASE("Between 0 and 5.")
    CHECK(graph.shortest_path(0, 5) == 3);
  }
}

TEST_CASE("Testing functions for MatrixGraph class.")
{
  string filename = "textfiles/matrix_graph_test.txt";
  int V = count_matrix_vertices(filename);
  MatrixGraph graph(V, filename);
  graph.read_matrix_from_file();

  SUBCASE("Is graph connected?")
  CHECK(graph.isConnected_matrix() == true);

  SUBCASE("Check complemented graph.")
  {
    MatrixGraph complementGraph = graph.complementMatrixGraph();

    bool **adjComplementMatr = complementGraph.getAdjMatrix();
    bool **adjMatr = graph.getAdjMatrix();
    for (int i = 0; i < V; ++i)
      for (int j = 0; j < V; ++j)
        if (i != j)
        {
          if (adjMatr[i][j] == false)
            CHECK(adjComplementMatr[i][j] == true);
          else
            CHECK(adjComplementMatr[i][j] == false);
        }
  }

  SUBCASE("Check shortest path.")
  {
    SUBCASE("Between 0 and 1.")
    CHECK(graph.shortestPath_matrix(0, 1) == 1);

    SUBCASE("Between 0 and 5.")
    CHECK(graph.shortestPath_matrix(0, 5) == 3);
  }
}

TEST_CASE("Testing functions for IncendenceMatrixGraph class.")
{
  string filename = "textfiles/incendence_matrix_graph_test.txt";
  int edges;
  int vertices = count_vertices_edges(filename, edges);
  IncendenceMatrixGraph graph(vertices, edges, filename);
  graph.readFromFile();

  SUBCASE("Is graph connected?")
  CHECK(graph.isConnected_incMatrix() == true);
  SUBCASE("Complemented graph.")
  {
    IncendenceMatrixGraph complementGraph = graph.complementMatrixGraph();

    int expectedMatrix[6][9] =
        {
            {1, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 0, 0},
            {0, 0, 1, 0, 0, 1, 0, 1, 1},
            {1, 0, 0, 1, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 0, 1}};

    for (int i = 0; i < complementGraph.getVertexes(); i++)
      for (int j = 0; j < complementGraph.getEdges(); j++)
        CHECK(complementGraph.getIncMatrix()[i][j] == expectedMatrix[i][j]);
  }
  SUBCASE("Check shortest path.")
  {
    SUBCASE("Between 0 and 1.")
    CHECK(graph.shortestPath_incMatrix(0, 1) == 1);

    SUBCASE("Between 3 and 5.")
    CHECK(graph.shortestPath_incMatrix(3, 5) == 4);
  }
}

TEST_CASE("Testing functions for DiscreteRandomVariable class.")
{
  SUBCASE("Test one.")
  {
    std::vector<double> values = {0.5, 1, 2, 4};
    std::vector<double> probabilities = {0.2, 0.1, 0.3, 0.4};
    DiscreteRandomVariable discreteRV("Example", values, probabilities);
    CHECK(discreteRV.probability() == 1);
    CHECK(abs(discreteRV.expectedValue() - 2.4) < 1e-6);
    CHECK(abs(discreteRV.variance() - 1.99) < 1e-6);
  }

  SUBCASE("Test two.")
  {
    std::vector<double> values = {1, 2};
    std::vector<double> probabilities = {0.5, 0.5};
    DiscreteRandomVariable discreteRV("Coin throw", values, probabilities);
    CHECK(discreteRV.probability() == 1);
    CHECK(abs(discreteRV.expectedValue() - 1.5) < 1e-6);
    CHECK(abs(discreteRV.variance() - 0.25) < 1e-6);
  }
}

TEST_CASE("Testing functions for BinomialDistribution class.")
{
  SUBCASE("Test one.")
  {
    BinomialDistribution binomialRV("Example1", 5, 0, 0.51);
    vector<double> prob = binomialRV.getProbabilities();
    vector<double> toExpect = {0.0282475, 0.147002, 0.306005, 0.318495, 0.165747, 0.0345025};
    for (auto i = 0; i < prob.size(); i++)
      CHECK(abs(prob[i] - toExpect[i]) < 1e-6);

    CHECK(abs(binomialRV.expectedValue() - 2.55) < 1e-6);
    CHECK(abs(binomialRV.variance() - 1.2495) < 1e-6);
  }

  SUBCASE("Test two.")
  {
    BinomialDistribution binomialRV("Example2", 3, 0, 0.24);
    vector<double> prob = binomialRV.getProbabilities();
    vector<double> toExpect = {0.438976, 0.415872, 0.131328, 0.013824};
    for (auto i = 0; i < prob.size(); i++)
      CHECK(abs(prob[i] - toExpect[i]) < 1e-6);

    CHECK(abs(binomialRV.expectedValue() - 0.72) < 1e-6);
    CHECK(abs(binomialRV.variance() - 0.5472) < 1e-6);
  }
}

TEST_CASE("Testing functions for PoissonDistribution class.")
{
  SUBCASE("Test one.")
  {
    PoissonDistribution poissonRV("Example1", 4, 2);
    vector<double> prob = poissonRV.getProbabilities();
    CHECK(abs(prob[0] - 0.0902235) < 1e-6);
    CHECK(poissonRV.expectedValue() == 2);
    CHECK(poissonRV.variance() == 2);
  }

  SUBCASE("Test two.")
  {
    PoissonDistribution poissonRV("Example2", 4, 0.24);
    vector<double> prob = poissonRV.getProbabilities();
    CHECK(abs(prob[0] - 0.000108743) < 1e-6);
    CHECK(abs(poissonRV.expectedValue() - 0.24) < 1e-6);
    CHECK(abs(poissonRV.variance() - 0.24) < 1e-6);
  }
}

TEST_CASE("Testing functions for GeometricDistribution class.")
{
  SUBCASE("Test one.")
  {
    GeometricDistribution geometricRV("Example1", 4, 0.2);
    vector<double> prob = geometricRV.getProbabilities();
    CHECK(abs(prob[0] - 0.1024) < 1e-6);
    CHECK(abs(geometricRV.expectedValue() - 5) < 1e-6);
    CHECK(abs(geometricRV.variance() - 20) < 1e-6);
  }

  SUBCASE("Test two.")
  {
    GeometricDistribution geometricRV("Example2", 10, 0.5);
    vector<double> prob = geometricRV.getProbabilities();
    CHECK(abs(prob[0] - 0.000976562) < 1e-6);
    CHECK(abs(geometricRV.expectedValue() - 2) < 1e-6);
    CHECK(abs(geometricRV.variance() - 2) < 1e-6);
  }
}

TEST_CASE("Testing functions for UniformDistribution class.")
{
  SUBCASE("Test one.")
  {
    UniformDistribution uniformRV("Event1", 0, 10);
    CHECK(uniformRV.getProbabilityDensity(5.0) == 0.1);
    CHECK(uniformRV.CDF(5.0) == 0.5);
    CHECK(uniformRV.expectedValue() == 5);
    CHECK(abs(uniformRV.variance() - 8.33333) < 1e-5);
    CHECK(uniformRV.getProbabilityDensity(15.0) == 0);
    CHECK(uniformRV.CDF(-5.0) == 0);
  }

  SUBCASE("Test two.")
  {
    UniformDistribution uniformRV("Event2", 20, 100);
    CHECK(uniformRV.getProbabilityDensity(73.0) == 0.0125);
    CHECK(uniformRV.CDF(73.0) == 0.6625);
    CHECK(uniformRV.expectedValue() == 60);
    CHECK(abs(uniformRV.variance() - 533.33333) < 1e-5);
    CHECK(uniformRV.getProbabilityDensity(10.0) == 0);
    CHECK(uniformRV.CDF(105.0) == 1);
  }
}

TEST_CASE("Testing functions for ExponentialDistribution class.")
{
  SUBCASE("Test one.")
  {
    ExponentialDistribution exponential("Event1", 2.0);
    CHECK(abs(exponential.getProbabilityDensity(1.0) - 0.270671) < 1e-6);
    CHECK(abs(exponential.CDF(1.0) - 0.864665) < 1e-6);
    CHECK(exponential.expectedValue() == 0.5);
    CHECK(exponential.variance() == 0.25);
    CHECK(exponential.getProbabilityDensity(-1.0) == 0);
    CHECK(exponential.CDF(-1.0) == 0);
  }

  SUBCASE("Test two.")
  {
    ExponentialDistribution exponential("Event1", 4.0);
    CHECK(abs(exponential.getProbabilityDensity(5.0) - 8.24461e-09) < 1e-9);
    CHECK(abs(exponential.CDF(5.0) - 1) < 1e-6);
    CHECK(exponential.expectedValue() == 0.25);
    CHECK(exponential.variance() == 0.0625);
    CHECK(exponential.getProbabilityDensity(-12.0) == 0);
    CHECK(exponential.CDF(-15.0) == 0);
  }
}

TEST_CASE("Testing functions for CombinedRandomEvent class.")
{
  SUBCASE("Test one.")
  {
    BinomialDistribution eventA("Event A", 10, 3, 0.5);
    PoissonDistribution eventB("Event B", 3, 2.0);
    CombinedRandomEvent combinedEvent1("A AND B", eventA, eventB, "AND");
    CHECK(abs(combinedEvent1.probability() - 0.0211461) < 1e-7);
    CombinedRandomEvent combinedEvent2("A OR B", eventA, eventB, "OR");
    CHECK(abs(combinedEvent2.probability() - 0.276488) < 1e-6);
  }

  SUBCASE("Test two.")
  {
    BinomialDistribution eventA("Event A", 5, 1, 0.32);
    GeometricDistribution eventB("Event B", 12, 0.27);
    CombinedRandomEvent combinedEvent1("A AND B", eventA, eventB, "AND");
    CHECK(abs(combinedEvent1.probability() - 0.00289782) < 1e-7);
    CombinedRandomEvent combinedEvent2("A OR B", eventA, eventB, "OR");
    CHECK(abs(combinedEvent2.probability() - 0.347675) < 1e-6);
  }
}
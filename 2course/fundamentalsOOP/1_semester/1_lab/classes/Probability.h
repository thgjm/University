#include <iostream>
#include <vector>
#include <cmath>
#include <functional>

class RandomEvent
{
protected:
  std::string name;

public:
  RandomEvent(const std::string &name) : name(name) {}

  std::string getName() const
  {
    return name;
  }

  virtual double probability() const = 0;
};

class RandomVariable : public RandomEvent
{
protected:
  std::vector<double> values;

public:
  RandomVariable(const std::string &name, const std::vector<double> &values)
      : RandomEvent(name), values(values) {}

  std::vector<double> getValues() const
  {
    return values;
  }

  virtual double expectedValue() const = 0;
  virtual double variance() const = 0;
};

class DiscreteRandomVariable : public RandomVariable
{
protected:
  std::vector<double> probabilities;

  // used in child classes
  double factorial(int num) const
  {
    if (num == 0 || num == 1)
      return 1;
    return num * factorial(num - 1);
  }

public:
  DiscreteRandomVariable(const std::string &name, const std::vector<double> &values, const std::vector<double> &probabilities)
      : RandomVariable(name, values), probabilities(probabilities)
  {
    if (values.size() != probabilities.size())
    {
      std::cout << "Error. The number of values and probabilities should be equal." << std::endl;
      this->values.clear();
      this->probabilities.clear();
      return;
    }
  }

  double probability() const override
  {
    double totalProbability = 0.0;
    for (const auto &prob : probabilities)
      totalProbability += prob;
    return totalProbability;
  }

  double expectedValue() const override
  {
    double expValue = 0.0;
    for (auto i = 0; i < values.size(); i++)
      expValue += values[i] * probabilities[i];
    return expValue;
  }

  double variance() const override
  {
    double expValue = expectedValue();
    double expValueSquare = 0.0;
    for (auto i = 0; i < values.size(); i++)
      expValueSquare += (values[i] * values[i]) * probabilities[i];

    double var = expValueSquare - (expValue * expValue);

    return var;
  }

  void printOutDistribution()
  {
    for (auto i = 0; i < values.size(); i++)
      std::cout << "P(ξ = " << values[i] << ") = " << probabilities[i] << "\n";
  }

  std::vector<double> getProbabilities() const
  {
    return probabilities;
  }
};

class BinomialDistribution : public DiscreteRandomVariable
{
private:
  int n;    // number of trials
  int k;    // number of successes (for some exact probability)
  double p; // probability of success
public:
  BinomialDistribution(const std::string &name, int n, int k, double p)
      : DiscreteRandomVariable(name, {}, {})
  {
    if (n < 0 || k < 0 || k > n || p < 0 || p > 1)
    {
      std::cout << "Error. Invalid parameters." << std::endl;
      this->n = 0;
      this->p = 0.0;
      this->k = 0;
      return;
    }
    this->n = n;
    this->p = p;
    this->k = k;
    calculateDistribution();
  }

  void calculateDistribution() // distribution for ξ = k, where k=[0, n]
  {
    values.clear();
    probabilities.clear();

    for (int kk = 0; kk <= n; kk++)
    {
      double probability = binomialProbability(n, kk, p);
      values.push_back(kk);
      probabilities.push_back(probability);
    }
  }

  double binomialProbability(int n, int k, double p) const
  {
    // binomial distribution formula
    double combination = factorial(n) / (factorial(k) * factorial(n - k));
    return combination * pow(p, k) * pow(1 - p, n - k);
  }

  double expectedValue() const override
  {
    return n * p;
  }

  double variance() const override
  {
    return n * p * (1 - p);
  }

  double probability() const override
  {
    return probabilities[k];
  }
};

class PoissonDistribution : public DiscreteRandomVariable
{
private:
  int k;
  double lambda; // lambda>0
public:
  PoissonDistribution(const std::string &name, int k, double lambda)
      : DiscreteRandomVariable(name, {}, {})
  {
    if (lambda <= 0 || k < 0)
    {
      std::cout << "Error. Invalid parameters." << std::endl;
      this->k = 0;
      this->lambda = 0.0;
      return;
    }
    this->k = k;
    this->lambda = lambda;
    calculateDistribution();
  }

  void calculateDistribution() // distribution for ξ = k
  {
    values.clear();
    probabilities.clear();

    double probability = poissonProbability(k, lambda);
    values.push_back(k);
    probabilities.push_back(probability);
  }

  double poissonProbability(int k, double lambda) const
  {
    // poisson distribution formula
    return pow(lambda, k) * exp(-lambda) / factorial(k);
  }

  double expectedValue() const override
  {
    return lambda;
  }

  double variance() const override
  {
    return lambda;
  }
};

// measures the probability of success after a given number of trials.
class GeometricDistribution : public DiscreteRandomVariable
{
private:
  int k;    // number of trials
  double p; // probability of success
public:
  GeometricDistribution(const std::string &name, int k, double p)
      : DiscreteRandomVariable(name, {}, {})
  {
    if (k < 0 || p < 0 || p > 1)
    {
      std::cout << "Error. Invalid parameters." << std::endl;
      this->k = 0;
      this->p = 0.0;
      return;
    }
    this->k = k;
    this->p = p;
    calculateDistribution();
  }

  void calculateDistribution() // distribution for ξ = k
  {
    values.clear();
    probabilities.clear();

    double probability = geometricProbability(k, p);
    values.push_back(k);
    probabilities.push_back(probability);
  }

  double geometricProbability(int k, double lambda) const
  {
    // geometric distribution formula
    return pow(1 - p, k - 1) * p;
  }

  double expectedValue() const override
  {
    return 1 / p;
  }

  double variance() const override
  {
    return (1 - p) / (p * p);
  }
};

class ContinuousRandomVariable : public RandomVariable
{
protected:
  std::function<double(double)> pdf; // Probability density function

public:
  ContinuousRandomVariable(const std::string &name, std::function<double(double)> pdf)
      : RandomVariable(name, {}), pdf(pdf) {}

  // Probability density function
  double getProbabilityDensity(double x) const
  {
    return pdf(x);
  }

  double probability() const override
  {
    return 0.0;
  };

  virtual double CDF(double x) const = 0; // cumulative distribution function
};

class UniformDistribution : public ContinuousRandomVariable
{
private:
  double a; // lower limit
  double b; // higher limit

public:
  UniformDistribution(const std::string &name, double a, double b)
      : ContinuousRandomVariable(name, [a, b](double x)
                                 { return (x >= a && x <= b) ? 1.0 / (b - a) : 0.0; }),
        a(a), b(b)
  {
    if (a >= b)
    {
      std::cout << "Error. Invalid parameters." << std::endl;
      this->a = 0.0;
      this->b = 1.0;
      return;
    }
  }

  double CDF(double x) const override
  {
    if (x < a)
      return 0.0;
    else if (x > b)
      return 1.0;
    else
      return (x - a) / (b - a);
  }

  double expectedValue() const override
  {
    return (a + b) / 2.0;
  }

  double variance() const override
  {
    return (b - a) * (b - a) / 12.0;
  }
};

// Class for Exponential Distribution
class ExponentialDistribution : public ContinuousRandomVariable
{
private:
  double lambda; // Rate parameter

public:
  ExponentialDistribution(const std::string &name, double lambda)
      : ContinuousRandomVariable(name, [lambda](double x)
                                 { return (x < 0) ? 0 : lambda * exp(-lambda * x); }),
        lambda(lambda) {}

  double CDF(double x) const override
  {
    if (x < 0)
      return 0.0;
    else
      return 1 - exp(-lambda * x);
  }

  double expectedValue() const override
  {
    return 1 / lambda;
  }

  double variance() const override
  {
    return 1 / (lambda * lambda);
  }
};

class CombinedRandomEvent : public RandomEvent
{
private:
  const RandomEvent &event1;
  const RandomEvent &event2;
  std::string operation; // "and" or "or"

public:
  CombinedRandomEvent(const std::string &name, const RandomEvent &event1, const RandomEvent &event2, const std::string &operation)
      : RandomEvent(name), event1(event1), event2(event2), operation(operation) {}

  double probability() const override
  {
    if (operation == "AND")
      return event1.probability() * event2.probability(); // P(A and B) = P(A) * P(B) (if independent)
    else if (operation == "OR")
      return event1.probability() + event2.probability() - event1.probability() * event2.probability(); // P(A or B) = P(A) + P(B) - P(A and B)
    else
    {
      std::cout << "Invalid operation. Use 'AND' or 'OR'." << std::endl;
      return 0.0;
    }
  }
};
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Rational
{
private:
  int num, den;

  void simplify()
  {
    int gcd = std::__gcd(num, den);
    num /= gcd;
    den /= gcd;
    if (den < 0)
    {
      num = -num;
      den = -den;
    }
  }

public:
  Rational(int n = 0, int d = 1) : num(n), den(d)
  {
    if (den == 0)
      throw std::invalid_argument("Denominator cannot be zero");
    simplify();
  }

  bool operator<(const Rational &other) const
  {
    return num * other.den < other.num * den;
  }

  bool operator>(const Rational &other) const
  {
    return num * other.den > other.num * den;
  }

  bool operator==(const Rational &other) const
  {
    return num == other.num && den == other.den;
  }

  Rational operator-(const Rational &other) const
  {
    return Rational(num * other.den - other.num * den, den * other.den);
  }

  friend ostream &operator<<(ostream &os, const Rational &r)
  {
    os << r.num << "/" << r.den;
    return os;
  }
};

// Визначення кольорів
enum Color
{
  RED,
  BLACK
};

// Структура відрізка
struct Segment
{
  Rational start, end;
  Segment(const Rational &s, const Rational &e) : start(s), end(e) {}
};

// Вузол дерева
struct Node
{
  Segment segment;
  Color color;
  Node *left, *right, *parent;
  Node(const Segment &s) : segment(s), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree
{
private:
  Node *root;

  void leftRotate(Node *x)
  {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
      y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
      root = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void rightRotate(Node *x)
  {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
      y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
      root = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  void insertFixup(Node *z)
  {
    while (z != root && z->parent->color == RED)
    {
      if (z->parent == z->parent->parent->left)
      {
        Node *y = z->parent->parent->right;
        if (y && y->color == RED)
        {
          z->parent->color = BLACK;
          y->color = BLACK;
          z->parent->parent->color = RED;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->right)
          {
            z = z->parent;
            leftRotate(z);
          }
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          rightRotate(z->parent->parent);
        }
      }
      else
      {
        Node *y = z->parent->parent->left;
        if (y && y->color == RED)
        {
          z->parent->color = BLACK;
          y->color = BLACK;
          z->parent->parent->color = RED;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->left)
          {
            z = z->parent;
            rightRotate(z);
          }
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          leftRotate(z->parent->parent);
        }
      }
    }
    root->color = BLACK;
  }

  void insert(const Segment &segment)
  {
    Node *z = new Node(segment);
    Node *y = nullptr;
    Node *x = root;
    while (x != nullptr)
    {
      y = x;
      if (z->segment.start < x->segment.start)
        x = x->left;
      else
        x = x->right;
    }
    z->parent = y;
    if (y == nullptr)
      root = z;
    else if (z->segment.start < y->segment.start)
      y->left = z;
    else
      y->right = z;
    insertFixup(z);
  }

public:
  RedBlackTree(const vector<pair<Rational, Rational>> &segments) : root(nullptr)
  {
    for (const auto &seg : segments)
      insert(Segment(seg.first, seg.second));
  }

  Segment findMinSegment() const
  {
    Node *node = root;
    while (node->left != nullptr)
      node = node->left;
    return node->segment;
  }

  Segment findMaxSegment() const
  {
    Node *node = root;
    while (node->right != nullptr)
      node = node->right;
    return node->segment;
  }

  Rational findSegmentLength(const Segment &segment) const
  {
    return segment.end - segment.start;
  }
};

int main()
{
  vector<pair<Rational, Rational>> segments = {
      {Rational(1, 2), Rational(3, 4)},
      {Rational(2, 3), Rational(5, 6)},
      {Rational(1, 4), Rational(2, 5)}};

  RedBlackTree tree(segments);

  Segment minSegment = tree.findMinSegment();
  Segment maxSegment = tree.findMaxSegment();
  cout << "Min segment: " << minSegment.start << " - " << minSegment.end << endl;
  cout << "Max segment: " << maxSegment.start << " - " << maxSegment.end << endl;
  return 0;
}
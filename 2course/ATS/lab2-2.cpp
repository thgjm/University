#include <iostream>
#include <algorithm>
#include <queue>

enum Color
{
  RED,
  BLACK
};

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

  Rational operator+(const Rational &other) const
  {
    int n = num * other.den + other.num * den;
    int d = den * other.den;
    return Rational(n, d);
  }

  friend std::ostream &operator<<(std::ostream &os, const Rational &r)
  {
    os << r.num << "/" << r.den;
    return os;
  }
};

struct SegmentNode
{
  Rational start, end;
  Rational value;
  Color color;
  SegmentNode *left, *right, *parent;

  SegmentNode(Rational s, Rational e, Rational v)
      : start(s), end(e), value(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class SegmentTreeRBT
{
private:
  SegmentNode *root;

  //  left rotation
  void leftRotate(SegmentNode *x)
  {
    if (x == nullptr || x->right == nullptr)
      return;

    SegmentNode *y = x->right;
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

  //  right rotation
  void rightRotate(SegmentNode *y)
  {
    if (y == nullptr || y->left == nullptr)
      return;

    SegmentNode *x = y->left;
    y->left = x->right;
    if (x->right != nullptr)
      x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nullptr)
      root = x;
    else if (y == y->parent->left)
      y->parent->left = x;
    else
      y->parent->right = x;
    x->right = y;
    y->parent = x;
  }

  // fix violations after inserting a node
  void fixInsert(SegmentNode *z)
  {
    while (z != root && z->parent->color == RED)
    {
      if (z->parent == z->parent->parent->left)
      {
        SegmentNode *y = z->parent->parent->right;
        if (y != nullptr && y->color == RED)
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
        SegmentNode *y = z->parent->parent->left;
        if (y != nullptr && y->color == RED)
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

  // Transplant function used in deletion
  void transplant(SegmentNode *u, SegmentNode *v)
  {
    if (u->parent == nullptr)
      root = v;
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
    if (v != nullptr)
      v->parent = u->parent;
  }

  //    delete a node
  void deleteNode(SegmentNode *z)
  {
    if (z == nullptr)
      return;

    SegmentNode *y = z;
    SegmentNode *x = nullptr;
    Color y_original_color = y->color;

    if (z->left == nullptr)
    {
      x = z->right;
      transplant(z, z->right);
    }
    else if (z->right == nullptr)
    {
      x = z->left;
      transplant(z, z->left);
    }
    else
    {
      y = minimum(z->right);
      y_original_color = y->color;
      x = y->right;

      if (y->parent == z)
      {
        if (x != nullptr)
          x->parent = y;
      }
      else
      {
        if (x != nullptr)
          x->parent = y->parent;
        transplant(y, y->right);
        if (y->right != nullptr)
          y->right->parent = y;
        y->right = z->right;
        if (y->right != nullptr)
          y->right->parent = y;
      }
      transplant(z, y);
      y->left = z->left;
      if (y->left != nullptr)
        y->left->parent = y; // Check if y->left is not nullptr before assigning parent
      y->color = z->color;
    }

    if (y_original_color == BLACK && x != nullptr) // Check if x is not nullptr
      fixDelete(x);

    delete z; // Free memory allocated for the deleted node
  }

  // Function to fix violations after deleting a node
  void fixDelete(SegmentNode *x)
  {
    while (x != root && x != nullptr && x->color == BLACK)
    {
      if (x == x->parent->left)
      {
        SegmentNode *w = x->parent->right;
        if (w->color == RED)
        {
          w->color = BLACK;
          x->parent->color = RED;
          leftRotate(x->parent);
          w = x->parent->right;
        }
        if ((w->left == nullptr || w->left->color == BLACK) &&
            (w->right == nullptr || w->right->color == BLACK))
        {
          w->color = RED;
          x = x->parent;
        }
        else
        {
          if (w->right == nullptr || w->right->color == BLACK)
          {
            if (w->left != nullptr)
              w->left->color = BLACK;
            w->color = RED;
            rightRotate(w);
            w = x->parent->right;
          }
          w->color = x->parent->color;
          x->parent->color = BLACK;
          if (w->right != nullptr)
            w->right->color = BLACK;
          leftRotate(x->parent);
          x = root;
        }
      }
      else
      {
        SegmentNode *w = x->parent->left;
        if (w->color == RED)
        {
          w->color = BLACK;
          x->parent->color = RED;
          rightRotate(x->parent);
          w = x->parent->left;
        }
        if ((w->right == nullptr || w->right->color == BLACK) &&
            (w->left == nullptr || w->left->color == BLACK))
        {
          w->color = RED;
          x = x->parent;
        }
        else
        {
          if (w->left == nullptr || w->left->color == BLACK)
          {
            if (w->right != nullptr)
              w->right->color = BLACK;
            w->color = RED;
            leftRotate(w);
            w = x->parent->left;
          }
          w->color = x->parent->color;
          x->parent->color = BLACK;
          if (w->left != nullptr)
            w->left->color = BLACK;
          rightRotate(x->parent);
          x = root;
        }
      }
    }
    if (x != nullptr)
      x->color = BLACK;
  }

  // find the minimum node in a subtree
  SegmentNode *minimum(SegmentNode *node)
  {
    while (node->left != nullptr)
      node = node->left;
    return node;
  }

  // print the tree structure (in-order traversal)
  void printHelper(SegmentNode *node, int space)
  {
    if (!node)
      return;
    space += 5;
    printHelper(node->right, space);
    for (int i = 5; i < space; i++)
      std::cout << " ";
    std::cout << "[" << node->start << ", " << node->end << "] = " << node->value
              << " (" << (node->color == RED ? "R" : "B") << ")\n";
    printHelper(node->left, space);
  }

  void insertInterval(SegmentNode *newNode)
  {
    SegmentNode *y = nullptr;
    SegmentNode *x = root;

    while (x != nullptr)
    {
      y = x;
      if (newNode->start < x->start)
        x = x->left;
      else
        x = x->right;
    }

    newNode->parent = y;
    if (y == nullptr)
      root = newNode;
    else if (newNode->start < y->start)
      y->left = newNode;
    else
      y->right = newNode;

    fixInsert(newNode);
  }

  Rational querySumInterval(SegmentNode *node, const Rational &l, const Rational &r)
  {
    if (node == nullptr || node->end < l || node->start > r)
      return Rational(0);

    Rational sum(0);

    if ((node->start < r || node->start == r) && (node->end > l || node->end == l))
      sum = sum + node->value;

    sum = sum + querySumInterval(node->left, l, r);
    sum = sum + querySumInterval(node->right, l, r);

    return sum;
  }

  Rational queryMinInterval(SegmentNode *node, const Rational &l, const Rational &r)
  {
    if (node == nullptr || node->end < l || node->start > r)
      return Rational(INT_MAX, 1);

    Rational minVal = node->value;
    if ((node->start > r || node->end < l))
      minVal = Rational(INT_MAX, 1);

    Rational leftMin = queryMinInterval(node->left, l, r);
    Rational rightMin = queryMinInterval(node->right, l, r);

    return std::min({minVal, leftMin, rightMin});
  }

  Rational queryMaxInterval(SegmentNode *node, const Rational &l, const Rational &r)
  {
    if (node == nullptr || node->end < l || node->start > r)
      return Rational(INT_MIN, 1);

    Rational maxVal = node->value;
    if ((node->start > r || node->end < l))
      maxVal = Rational(INT_MIN, 1);

    Rational leftMax = queryMaxInterval(node->left, l, r);
    Rational rightMax = queryMaxInterval(node->right, l, r);

    return std::max({maxVal, leftMax, rightMax});
  }

public:
  SegmentTreeRBT() : root(nullptr) {}

  // insert a node
  void insert(const Rational &l, const Rational &r, const Rational &val)
  {
    SegmentNode *node = new SegmentNode(l, r, val);
    insertInterval(node);
  }

  // delete a node by interval
  void remove(const Rational &l, const Rational &r)
  {
    SegmentNode *z = root;
    while (z != nullptr)
    {
      if (l < z->start)
        z = z->left;
      else if (l > z->start)
        z = z->right;
      else
      {
        if (r == z->end)
        {
          deleteNode(z);
          return;
        }
        else if (r < z->end)
          z = z->left;
        else
          z = z->right;
      }
    }
    std::cout << "Node with interval [" << l << ", " << r << "] not found in the tree." << std::endl;
  }

  Rational querySum(const Rational &l, const Rational &r)
  {
    return querySumInterval(root, l, r);
  }

  Rational queryMin(const Rational &l, const Rational &r)
  {
    return queryMinInterval(root, l, r);
  }

  Rational queryMax(const Rational &l, const Rational &r)
  {
    return queryMaxInterval(root, l, r);
  }

  // print the tree structure
  void printTree() { printHelper(root, 0); }

  void printLevelOrder()
  {
    if (root == nullptr)
      return;

    std::queue<SegmentNode *> q;
    q.push(root);

    while (!q.empty())
    {
      int levelSize = q.size();
      while (levelSize--)
      {
        SegmentNode *node = q.front();
        q.pop();

        std::cout << "[" << node->start << ", " << node->end << "] = " << node->value
                  << " (" << (node->color == RED ? "R" : "B") << ")  ";

        if (node->left)
          q.push(node->left);
        if (node->right)
          q.push(node->right);
      }
      std::cout << "\n";
    }
  }
};

int main()
{
  SegmentTreeRBT tree;

  int n;

  std::cout << "How many nods in the tree?\n";
  std::cin >> n;
  for (int i = 0; i < n; i++)
  {
    int num1, den1, num2, den2, num3, den3;
    std::cout << "Node " << i + 1 << ":\n";
    std::cout << "Enter num and den of the start of interval: ";
    std::cin >> num1 >> den1;
    std::cout << "Enter num and den of the end of interval: ";
    std::cin >> num2 >> den2;
    std::cout << "Enter num and den of the value: ";
    std::cin >> num3 >> den3;
    tree.insert(Rational(num1, den1), Rational(num2, den2), Rational(num3, den3));
  }

  std::cout << "\nTree structure (lever order):\n";
  tree.printLevelOrder();

  int num1, den1, num2, den2;

  std::cout << "\nEnter num and den of the start of interval of the sum: ";
  std::cin >> num1 >> den1;
  std::cout << "Enter num and den of the end of interval of the sum: ";
  std::cin >> num2 >> den2;
  Rational result = tree.querySum(Rational(num1, den1), Rational(num2, den2));
  std::cout << "\nSum on [" << Rational(num1, den1) << ", " << Rational(num2, den2) << "]: " << result << "\n";

  std::cout << "\nEnter num and den of the start of interval of the min: ";
  std::cin >> num1 >> den1;
  std::cout << "Enter num and den of the end of interval of the min: ";
  std::cin >> num2 >> den2;
  Rational min = tree.queryMin(Rational(num1, den1), Rational(num2, den2));
  std::cout << "\nMin on [" << Rational(num1, den1) << ", " << Rational(num2, den2) << "]: " << min << "\n";

  std::cout << "\nEnter num and den of the start of interval of the max: ";
  std::cin >> num1 >> den1;
  std::cout << "Enter num and den of the end of interval of the max: ";
  std::cin >> num2 >> den2;
  Rational max = tree.queryMax(Rational(num1, den1), Rational(num2, den2));
  std::cout << "\nMax on [" << Rational(num1, den1) << ", " << Rational(num2, den2) << "]: " << max << "\n";

  return 0;
}
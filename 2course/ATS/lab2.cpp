#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>

// Клас для представлення раціональних чисел
class Rational
{
private:
  int num, den; // чисельник і знаменник

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

  friend std::ostream &operator<<(std::ostream &os, const Rational &r)
  {
    os << r.num << "/" << r.den;
    return os;
  }
};

// Вузол AA-дерева
struct AANode
{
  Rational key;
  AANode *left;
  AANode *right;
  int level;

  AANode(Rational k) : key(k), left(nullptr), right(nullptr), level(1) {}
};

// AA-дерево
class AATree
{
private:
  AANode *root;

  AANode *skew(AANode *node)
  {
    if (!node || !node->left || node->left->level != node->level)
      return node;
    AANode *L = node->left;
    node->left = L->right;
    L->right = node;
    return L;
  }

  AANode *split(AANode *node)
  {
    if (!node || !node->right || !node->right->right || node->level != node->right->right->level)
      return node;
    AANode *R = node->right;
    node->right = R->left;
    R->left = node;
    R->level++;
    return R;
  }

  AANode *insert(AANode *node, Rational key)
  {
    if (!node)
      return new AANode(key);

    if (key < node->key)
      node->left = insert(node->left, key);
    else if (key > node->key)
      node->right = insert(node->right, key);
    else
      return node;

    node = skew(node);
    node = split(node);
    return node;
  }

  void updateLevels(AANode *node)
  {
    if (!node)
      return;

    int leftLevel = node->left ? node->left->level : 0;
    int rightLevel = node->right ? node->right->level : 0;

    node->level = std::min(leftLevel, rightLevel) + 1; // В AA-дереві рівень визначається найменшим рівнем дитини + 1
  }

  void updateTreeLevels(AANode *node)
  {
    if (!node)
      return;

    updateTreeLevels(node->left);
    updateTreeLevels(node->right);
    updateLevels(node);
  }

  AANode *decrease_level(AANode *T)
  {
    if (!T)
      return nullptr;
    int shouldBe = std::min(T->left ? T->left->level : 0, T->right ? T->right->level : 0) + 1;
    if (shouldBe < T->level)
    {
      T->level = shouldBe;
      if (T->right && shouldBe < T->right->level)
      {
        T->right->level = shouldBe;
      }
    }
    return T;
  }

  AANode *remove(AANode *T, Rational key)
  {
    if (!T)
      return nullptr;

    if (key < T->key)
    {
      T->left = remove(T->left, key);
    }
    else if (key > T->key)
    {
      T->right = remove(T->right, key);
    }
    else
    {
      if (!T->left && !T->right)
      {
        delete T;
        return nullptr;
      }
      else if (!T->left)
      {
        AANode *temp = T->right;
        delete T;
        return temp;
      }
      else if (!T->right)
      {
        AANode *temp = T->left;
        delete T;
        return temp;
      }
      else
      {
        AANode *successor = T->right;
        while (successor->left)
          successor = successor->left;
        T->key = successor->key;
        T->right = remove(T->right, successor->key);
      }
    }

    T = decrease_level(T);
    T = skew(T);
    if (T->right)
      T->right = skew(T->right);
    if (T->right && T->right->right)
      T->right->right = skew(T->right->right);
    T = split(T);
    if (T->right)
      T->right = split(T->right);

    return T;
  }

  void printTree(AANode *node)
  {
    if (!node)
      return;

    std::map<int, std::vector<Rational>> levels;
    std::queue<AANode *> q;
    q.push(node);
    int maxLevel = node->level, minLevel = node->level;

    while (!q.empty())
    {
      AANode *current = q.front();
      q.pop();
      levels[current->level].push_back(current->key);
      minLevel = std::min(minLevel, current->level);
      maxLevel = std::max(maxLevel, current->level);

      if (current->left)
        q.push(current->left);
      if (current->right)
        q.push(current->right);
    }

    for (int i = minLevel; i <= maxLevel; ++i)
    {
      std::sort(levels[i].begin(), levels[i].end());

      std::cout << "Level " << i << ": ";
      for (const auto &key : levels[i])
        std::cout << key << " ";
      std::cout << "\n";
    }
  }

  int findLevel(AANode *node, Rational key)
  {
    if (!node)
      return -1;

    if (key == node->key)
      return node->level;

    if (key < node->key)
      return findLevel(node->left, key);
    else
      return findLevel(node->right, key);
  }

public:
  AATree() : root(nullptr) {}

  void insert(Rational key)
  {
    root = insert(root, key);
    updateTreeLevels(root);
  }

  void remove(Rational key)
  {
    root = remove(root, key);
    updateTreeLevels(root);
  }

  void buildFromVector(const std::vector<Rational> &values)
  {
    for (const auto &val : values)
    {
      root = insert(root, val);
      updateTreeLevels(root);
    }
  }

  void display()
  {
    printTree(root);
  }

  int findLevel(Rational key)
  {
    return findLevel(root, key);
  }
};

int main()
{
  AATree tree;
  std::vector<Rational> values = {Rational(6, 7), Rational(15, 19), Rational(2, 3), Rational(3, 11), Rational(1, 2)};
  tree.buildFromVector(values);
  std::cout << "AA Tree:\n";
  tree.display();
  std::cout << "Number 6/7 is on the level "<<tree.findLevel(Rational(6, 7));
  std::cout << "\nNumber 1/2 is on the level "<<tree.findLevel(Rational(1, 2));
  return 0;
}
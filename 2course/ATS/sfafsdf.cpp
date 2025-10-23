#include <iostream>
#include <algorithm>

using namespace std;

// Клас для представлення раціональних чисел
class Rational {
private:
    long long numerator, denominator;

    void reduce() {
      long long g = std::__gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    Rational(long long num = 0, long long den = 1) : numerator(num), denominator(den) {
        if (denominator == 0) {
            throw invalid_argument("Denominator cannot be zero");
        }
        reduce();
    }

    Rational operator+(const Rational& other) const {
        return Rational(numerator * other.denominator + other.numerator * denominator,
                        denominator * other.denominator);
    }

    Rational operator-(const Rational& other) const {
        return Rational(numerator * other.denominator - other.numerator * denominator,
                        denominator * other.denominator);
    }

    Rational operator*(const Rational& other) const {
        return Rational(numerator * other.numerator, denominator * other.denominator);
    }

    Rational operator/(const Rational& other) const {
        return Rational(numerator * other.denominator, denominator * other.numerator);
    }

    bool operator<(const Rational& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }

    bool operator>(const Rational& other) const {
        return numerator * other.denominator > other.numerator * denominator;
    }

    bool operator==(const Rational& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }

    friend ostream& operator<<(ostream& os, const Rational& r) {
        os << r.numerator << "/" << r.denominator;
        return os;
    }
};

// Вузол червоно-чорного дерева
struct Node {
    Rational value;
    Node *left, *right, *parent;
    bool isRed;

    Node(Rational val) : value(val), left(nullptr), right(nullptr), parent(nullptr), isRed(true) {}
};

// Клас червоно-чорного дерева (RB Tree) з функціями сегментного дерева
class SegmentTreeRB {
private:
    Node* root;

    void rotateLeft(Node*& root, Node*& pt);
    void rotateRight(Node*& root, Node*& pt);
    void fixViolation(Node*& root, Node*& pt);
    Node* insertBST(Node* root, Node* pt);
    Rational rangeSum(Node* node, Rational left, Rational right);
    Rational rangeMin(Node* node, Rational left, Rational right);
    Rational rangeMax(Node* node, Rational left, Rational right);

public:
    SegmentTreeRB() : root(nullptr) {}
    Rational getSum(Rational left, Rational right);
    Rational getMin(Rational left, Rational right);
    Rational getMax(Rational left, Rational right);
    void insert(Rational val);
    void inorder();
    void inorderHelper(Node* root);
};

void SegmentTreeRB::rotateLeft(Node*& root, Node*& pt) {
    Node* pt_right = pt->right;
    pt->right = pt_right->left;
    if (pt->right)
        pt->right->parent = pt;
    pt_right->parent = pt->parent;
    if (!pt->parent)
        root = pt_right;
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;
    else
        pt->parent->right = pt_right;
    pt_right->left = pt;
    pt->parent = pt_right;
}

void SegmentTreeRB::rotateRight(Node*& root, Node*& pt) {
    Node* pt_left = pt->left;
    pt->left = pt_left->right;
    if (pt->left)
        pt->left->parent = pt;
    pt_left->parent = pt->parent;
    if (!pt->parent)
        root = pt_left;
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;
    else
        pt->parent->right = pt_left;
    pt_left->right = pt;
    pt->parent = pt_left;
}

void SegmentTreeRB::fixViolation(Node*& root, Node*& pt) {
    Node* parent_pt = nullptr;
    Node* grand_parent_pt = nullptr;
    while ((pt != root) && (pt->isRed) && (pt->parent->isRed)) {
        parent_pt = pt->parent;
        grand_parent_pt = parent_pt->parent;
        if (parent_pt == grand_parent_pt->left) {
            Node* uncle_pt = grand_parent_pt->right;
            if (uncle_pt && uncle_pt->isRed) {
                grand_parent_pt->isRed = true;
                parent_pt->isRed = false;
                uncle_pt->isRed = false;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->right) {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->isRed, grand_parent_pt->isRed);
                pt = parent_pt;
            }
        } else {
            Node* uncle_pt = grand_parent_pt->left;
            if (uncle_pt && uncle_pt->isRed) {
                grand_parent_pt->isRed = true;
                parent_pt->isRed = false;
                uncle_pt->isRed = false;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->left) {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->isRed, grand_parent_pt->isRed);
                pt = parent_pt;
            }
        }
    }
    root->isRed = false;
}

Node* SegmentTreeRB::insertBST(Node* root, Node* pt) {
    if (!root) return pt;
    if (pt->value < root->value) {
        root->left = insertBST(root->left, pt);
        root->left->parent = root;
    } else if (pt->value > root->value) {
        root->right = insertBST(root->right, pt);
        root->right->parent = root;
    }
    return root;
}

void SegmentTreeRB::insert(Rational val) {
    Node* pt = new Node(val);
    root = insertBST(root, pt);
    fixViolation(root, pt);
}

void SegmentTreeRB::inorderHelper(Node* root) {
    if (!root) return;
    inorderHelper(root->left);
    cout << root->value << " ";
    inorderHelper(root->right);
}

void SegmentTreeRB::inorder() {
    inorderHelper(root);
    cout << endl;
}

Rational SegmentTreeRB::getSum(Rational left, Rational right) {
  return rangeSum(root, left, right);
}

Rational SegmentTreeRB::getMin(Rational left, Rational right) {
  return rangeMin(root, left, right);
}

Rational SegmentTreeRB::getMax(Rational left, Rational right) {
  return rangeMax(root, left, right);
}

int main() {
    SegmentTreeRB tree;
    tree.insert(Rational(1, 2));
    tree.insert(Rational(3, 4));
    tree.insert(Rational(1, 3));
    tree.insert(Rational(5, 6));
    tree.inorder();
    return 0;
}

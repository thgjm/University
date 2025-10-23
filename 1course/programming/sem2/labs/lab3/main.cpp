#include <iostream>
#include <string>
#include <cctype>
#include <climits>

using namespace std;

struct Node
{
    string data;
    Node *left;
    Node *right;
};

bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*');
}

bool isVariable(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isPossible(char c)
{
    return (isOperator(c) || isVariable(c) || isdigit(c - '0') || '(' || ')' || '.');
}

Node *createNode(const string &data, Node *left, Node *right)
{
    Node *newNode = new Node();
    newNode->data = data;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

bool isValidExpression(string expression)
{
    for (char c : expression)
    {
        if (!isPossible(c))
            return false;
        if (isOperator(c) || c == '(' || c == ')' || c=='.')
            continue;
        if (!isdigit(c) && !isVariable(c))
            return false;
    }
    return true;
}

bool isvalid(string expression)
{
    string exp=expression;
    int count=0;
    if(isOperator(exp[0]) || isOperator(exp[expression.length()-1])) return false;
    if(exp[0]=='.' || exp[expression.length()-1]=='.') return false;
    for (int i = 0; i < expression.length(); i++)
    {
        if( i<expression.length()-1 && isOperator(exp[i]) && isOperator(exp[i+1])) return false;
        if( i<expression.length()-1 && exp[i]=='.' && (exp[i+1] == '(' || exp[i+1] == ')' || isOperator(exp[i+1]) || isVariable(exp[i+1]) ) ) return false;
        if( i<expression.length()-1 && exp[i+1]=='.' && (exp[i] == ')' || exp[i]=='(' || isOperator(exp[i]) || isVariable(exp[i]) ) ) return false;
        if (expression[i] == '(')
            count++;
        else if (expression[i] == ')')
            count--;
    }
    if(count!=0) return false;
    return true;
}

Node *buildExpressionTree(string expression, int start, int end)
{
    if (start > end)
        return NULL;

    int lowestPrecedence = INT_MAX;
    int operatorIndex = -1;
    int parenthesisCount = 0;

    for (int i = start; i <= end; ++i)
    {
        if (expression[i] == '(')
            parenthesisCount++;
        else if (expression[i] == ')')
            parenthesisCount--;
        else if (parenthesisCount == 0 && isOperator(expression[i]))
        {
            int precedence;
            if (expression[i] == '+' || expression[i] == '-')
                precedence = 1;
            else
                precedence = 2;
            if (precedence <= lowestPrecedence)
            {
                lowestPrecedence = precedence;
                operatorIndex = i;
            }
        }
    }

    if (operatorIndex == -1)
    {
        if (expression[start] == '(' && expression[end] == ')')
            return buildExpressionTree(expression, start + 1, end - 1);
        else
            return createNode(expression.substr(start, end - start + 1), NULL, NULL);
    }

    Node *root = createNode(expression.substr(operatorIndex, 1), buildExpressionTree(expression, start, operatorIndex - 1), buildExpressionTree(expression, operatorIndex + 1, end));

    return root;
}

string adjustExpression(string expression)
{
    string adjusted;
    for (size_t i = 0; i < expression.size(); ++i)
    {
        adjusted += expression[i];
        if (i < expression.size() - 1 && isdigit(expression[i]) && (isVariable(expression[i + 1]) || expression[i + 1] == '('))
            adjusted += '*';
        if (i < expression.size() - 1 && isVariable(expression[i]) && (isVariable(expression[i + 1]) || isdigit(expression[i + 1]) || expression[i + 1] == '('))
            adjusted += '*';
    }
    return adjusted;
}

double evaluateExpression(Node *root)
{
    if (!root)
        return 0;

    if (!root->left && !root->right)
    {
        if (isVariable(root->data[0]))
        {
            cout << "Enter value for variable " << root->data[0] << ": ";
            int value;
            cin >> value;
            return value;
        }
        else
            return stod(root->data);
    }

    double leftValue = evaluateExpression(root->left);
    double rightValue = evaluateExpression(root->right);

    switch (root->data[0])
    {
    case '+':
        return leftValue + rightValue;
    case '-':
        return leftValue - rightValue;
    case '*':
        return leftValue * rightValue;
    default:
        return 0;
    }
}

Node *diff(Node *root, char variable)
{
    if (!root)
        return NULL;

    if (!root->left && !root->right)
    {
        if (isVariable(root->data[0]))
        {
            if (root->data[0] == variable)
                return createNode("1", NULL, NULL);
            else
                return createNode("0", NULL, NULL);
        }
        else
            return createNode("0", NULL, NULL);
    }

    Node *newRoot;

    switch (root->data[0])
    {
    case '+':
    case '-':
        return newRoot = createNode(root->data, diff(root->left, variable), diff(root->right, variable));
        break;
    case '*':
        return newRoot = createNode("+",
                                    createNode("*", diff(root->left, variable), root->right),
                                    createNode("*", root->left, diff(root->right, variable)));
        break;
        // default:
        //     return newRoot = createNode("0", NULL, NULL);
    }
}

/*void deltr(Node *p)
{
    if (!p)
        return;
    deltr(p->left);
    deltr(p->right);
    delete p;
}*/

Node *simpl(Node *p)
{
    if (p == NULL)
        return NULL;

    p->left = simpl(p->left);
    p->right = simpl(p->right);

    if (!isOperator(p->data[0]))
        return p;

    Node *pl = p->left;
    Node *pr = p->right;

    switch (p->data[0])
    {
    case '+':
        if (pl->data == "0")
        {
            delete pl;
            delete p;
            return pr;
        }
        if (pr->data == "0")
        {
            delete pr;
            delete p;
            return pl;
        }
        break;
    case '-':
        if (pr->data == "0")
        {
            delete pr;
            delete p;
            return pl;
        }
        break;
    case '*':
        if (pl->data == "0" || pr->data == "0")
        {
            delete p;
            return createNode("0", NULL, NULL);
        }
        if (pl->data == "1")
        {
            delete pl;
            delete p;
            return pr;
        }
        if (pr->data == "1")
        {
            delete pr;
            delete p;
            return pl;
        }
        break;
    }

    return p;
}

void inorderTraversal(Node *root)
{
    if (root == NULL)
        return;
    string v = root->data;
    if (root)
    {
        if (isOperator(v[0]))
        {
            cout << "(";
            inorderTraversal(root->left);
            cout << root->data;
            inorderTraversal(root->right);
            cout << ")";
        }
        else
            cout << v;
    }
}

/*void inorderTraversal(Node *root)
{
    if(root==NULL) return;
    if (root)
    {
        if(root->left!=NULL)
        inorderTraversal(root->left);
        cout << root->data << " ";
        if(root->right!=NULL)
        inorderTraversal(root->right);
    }
}*/

int main()
{
    string expression;
    cout << "Enter the expression (no spaces):\n\n";
    getline(cin, expression);
    expression = adjustExpression(expression);
    if (!isValidExpression(expression) || !isvalid(expression))
    {
        cout << "incorrect expression entry.";
        return 0;
    }
    Node *root = buildExpressionTree(expression, 0, expression.length() - 1);
    //cout << "\nExpression: ";
    //inorderTraversal(root);
    cout << "\nEnter variable for differentiation: ";
    char var;
    cin >> var;
    //cout << "\nExpression after differentiation: ";
    root = diff(root, var);
    //inorderTraversal(root);
    root = simpl(root);
    cout << "\nExpression after differentiation and simplification:\n\n";
    inorderTraversal(root);
    cout << endl;
    cout << "\nCalculation:\n\n";
    cout << evaluateExpression(root);
    cout<<endl;
    return 0;
}

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <set>
#include <iomanip>

using namespace std;

const int a = 2;
const int b = 47;
const int p = 211; // Просте число для модулювання

// Функція хешування першого рівня
int primary_hash(const string &key, int m)
{
    int hash_value = 0;
    for (char ch : key)
        hash_value = (hash_value * a + ch + b) % p;
    return hash_value % m;
}

// Функція хешування другого рівня
int secondary_hash(const string &key, int m, int a2, int b2)
{
    int hash_value = 0;
    for (char ch : key)
        hash_value = (hash_value * a2 + ch + b2) % p;
    return hash_value % m;
}

struct SecondaryHashTable
{
    vector<string> table;
    int a2, b2, size;

    SecondaryHashTable(const vector<string> &values)
    {
        size = values.size() * values.size();
        table.resize(size, "");
        a2 = 41; // Фіксовані коефіцієнти для підтаблиці
        b2 = 132;

        bool collision;
        do
        {
            collision = false;
            table.assign(size, "");
            for (const string &key : values)
            {
                int idx = secondary_hash(key, size, a2, b2);
                if (!table[idx].empty())
                {
                    collision = true;
                    break;
                }
                table[idx] = key;
            }
        } while (collision);
    }

    int find(const string &key)
    {
        int idx = secondary_hash(key, size, a2, b2);
        if (table[idx] == key)
            return idx;
        return -1;
    }
};

struct PerfectHashTable
{
    vector<SecondaryHashTable *> table;
    unordered_map<int, vector<string>> collisions;
    int size;

    PerfectHashTable(const vector<string> &keys)
    {
        set<string> unique_keys(keys.begin(), keys.end()); // Видаляємо повтори
        vector<string> unique_data(unique_keys.begin(), unique_keys.end());
        size = unique_data.size();
        table.resize(size, nullptr);

        for (const string &key : unique_data)
        {
            int idx = primary_hash(key, size);
            collisions[idx].push_back(key);
        }

        for (auto &[idx, values] : collisions)
            table[idx] = new SecondaryHashTable(values);
    }

    ~PerfectHashTable()
    {
        for (auto &entry : table)
            delete entry;
    }

    void print()
    {
        cout << "\n+--------+-----------------+" << endl;
        cout << "| Index  | Values          |" << endl;
        cout << "+--------+-----------------+" << endl;
        for (size_t i = 0; i < table.size(); ++i)
        {
            cout << "| " << setw(6) << i << " | ";
            if (table[i])
                for (const auto &key : table[i]->table)
                    if (!key.empty())
                        cout << key << " ";
            cout << "|" << endl;
        }
        cout << "+--------+-----------------+" << endl;
    }

    void find(const string &key)
    {
        int idx = primary_hash(key, size);
        if (table[idx])
        {
            int sub_idx = table[idx]->find(key);
            if (sub_idx != -1)
            {
                cout << "Element '" << key << "' found at primary index " << idx << " in secondary index " << sub_idx << "\n";
                return;
            }
        }
        cout << "Element '" << key << "' not found.\n";
    }
};

int main()
{
    vector<string> keys;
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;
    
    cout << "Enter the elements: \n";
    for (int i = 0; i < n; ++i)
    {
        string key;
        cin >> key;
        keys.push_back(key);
    }
    
    PerfectHashTable hashTable(keys);
    hashTable.print();
    
    string search_key;
    cout << "Enter a key to search: ";
    cin >> search_key;
    hashTable.find(search_key);
    
    return 0;
}
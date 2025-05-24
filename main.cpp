#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <chrono>
#include <random>

using namespace std;
using HashFunction = function<int(int, int)>;

class HashTable {
public:
    HashTable(int size, HashFunction func) : tableSize(size), hashFunc(func) {
        table.resize(tableSize);
    }

    void insert(int key) {
        int index = hashFunc(key, tableSize);
        table[index].push_back(key);
    }

    void remove(int key) {
        int index = hashFunc(key, tableSize);
        table[index].remove(key);
    }

private:
    int tableSize;
    HashFunction hashFunc;
    vector<list<int>> table;
};

int hash1(int key, int size) {
    return key % size;
}

int hash2(int key, int size) {
    return (key / 10) % size;
}

int hash3(int key, int size) {
    return (key ^ (key >> 4)) % size;
}

void testHashFunction(HashFunction func, const string& name,
                      const vector<int>& dataInsert,
                      const vector<int>& dataRemove,
                      const string& caseType,
                      int tableSize) {
    HashTable ht(tableSize, func);

    cout << "=== " << name << " | " << caseType << " ===\n";

    auto startInsert = chrono::high_resolution_clock::now();
    for (int key : dataInsert) {
        ht.insert(key);
    }
    auto endInsert = chrono::high_resolution_clock::now();

    auto startRemove = chrono::high_resolution_clock::now();
    for (int key : dataRemove) {
        ht.remove(key);
    }
    auto endRemove = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(endInsert - startInsert).count();
    auto removeTime = chrono::duration_cast<chrono::microseconds>(endRemove - startRemove).count();

    cout << "Czas dodawania: " << insertTime << " mikrosekund\n";
    cout << "Czas usuwania:  " << removeTime << " mikrosekund\n\n";
}

vector<int> generateData(int n, const string& type, int tableSize) {
    vector<int> data;
    if (type == "optymistyczny") {
        for (int i = 0; i < n; ++i)
            data.push_back(i); 
    } else if (type == "sredni") {
        mt19937 gen(42);
        uniform_int_distribution<> dist(1, 1000000);
        for (int i = 0; i < n; ++i)
            data.push_back(dist(gen));
    } else if (type == "pesymistyczny") {
        for (int i = 0; i < n; ++i)
            data.push_back(i * tableSize);
    }
    return data;
}

int main() {
    const int TABLE_SIZE = 10007;
    vector<string> cases = { "optymistyczny", "sredni", "pesymistyczny" };
    vector<int> testSizes = { 1000, 10000, 100000 };

    for (int N : testSizes) {
        cout << "==============================\n";
        cout << "#### TEST DLA N = " << N << " ####\n";
        cout << "==============================\n";

        for (const auto& caseType : cases) {
            auto insertData = generateData(N, caseType, TABLE_SIZE);
            auto removeData = insertData;

            testHashFunction(hash1, "Hash 1: x % size", insertData, removeData, caseType, TABLE_SIZE);
            testHashFunction(hash2, "Hash 2: (x / 10) % size", insertData, removeData, caseType, TABLE_SIZE);
            testHashFunction(hash3, "Hash 3: (x ^ (x >> 4)) % size", insertData, removeData, caseType, TABLE_SIZE);
        }

        cout << endl;
    }

    return 0;
}

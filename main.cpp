#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <chrono>
#include <random>

using HashFunction = std::function<int(int, int)>;

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
    std::vector<std::list<int>> table;
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

void testHashFunction(HashFunction func, const std::string& name,
                      const std::vector<int>& dataInsert,
                      const std::vector<int>& dataRemove,
                      const std::string& caseType,
                      int tableSize) {
    HashTable ht(tableSize, func);

    std::cout << "=== " << name << " | " << caseType << " ===\n";

    auto startInsert = std::chrono::high_resolution_clock::now();
    for (int key : dataInsert) {
        ht.insert(key);
    }
    auto endInsert = std::chrono::high_resolution_clock::now();

    auto startRemove = std::chrono::high_resolution_clock::now();
    for (int key : dataRemove) {
        ht.remove(key);
    }
    auto endRemove = std::chrono::high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(endInsert - startInsert).count();
    auto removeTime = std::chrono::duration_cast<std::chrono::microseconds>(endRemove - startRemove).count();

    std::cout << "Czas dodawania: " << insertTime << " mikrosekund\n";
    std::cout << "Czas usuwania:  " << removeTime << " mikrosekund\n\n";
}

std::vector<int> generateData(int n, const std::string& type, int tableSize) {
    std::vector<int> data;
    if (type == "optymistyczny") {
        for (int i = 0; i < n; ++i)
            data.push_back(i); 
    } else if (type == "sredni") {
        std::mt19937 gen(42);
        std::uniform_int_distribution<> dist(1, 1000000);
        for (int i = 0; i < n; ++i)
            data.push_back(dist(gen));
    } else if (type == "pesymistyczny") {
        for (int i = 0; i < n; ++i)
            data.push_back(i * tableSize);
    }
    return data;
}

int main() {
    const int N = 10000;
    const int TABLE_SIZE = 10007;

    std::vector<std::string> cases = { "optymistyczny", "sredni", "pesymistyczny" };

    for (const auto& caseType : cases) {
        auto insertData = generateData(N, caseType, TABLE_SIZE);
        auto removeData = insertData;

        testHashFunction(hash1, "Hash 1: x % size", insertData, removeData, caseType, TABLE_SIZE);
        testHashFunction(hash2, "Hash 2: (x / 10) % size", insertData, removeData, caseType, TABLE_SIZE);
        testHashFunction(hash3, "Hash 3: (x ^ (x >> 4)) % size", insertData, removeData, caseType, TABLE_SIZE);
    }

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

const float LOAD_FACTOR = 0.75;  // For growing
const float SHRINK_FACTOR = 0.25;  // For shrinking

// Node for the doubly linked list
struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;

    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// Doubly linked list for handling collisions
struct DoublyLinkedList {
    Node* head;

    DoublyLinkedList() : head(nullptr) {}

    // Insert node at the beginning
    void insert(int key, int value) {
        Node* newNode = new Node(key, value);
        if (head != nullptr) {
            head->prev = newNode;
        }
        newNode->next = head;
        head = newNode;
    }

    // Find node by key
    Node* find(int key) {
        Node* temp = head;
        while (temp) {
            if (temp->key == key) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // Remove a node by key
    void remove(int key) {
        Node* temp = head;
        while (temp) {
            if (temp->key == key) {
                if (temp->prev) temp->prev->next = temp->next;
                if (temp->next) temp->next->prev = temp->prev;
                if (temp == head) head = temp->next;
                delete temp;
                return;
            }
            temp = temp->next;
        }
    }
};

// Hash Table class
class HashTable {
    int size;
    int capacity;
    DoublyLinkedList* table;

    // Generic hash function using multiplication and division
    int hashFunction(int key) {
        float A = 0.618033;  // Fractional part of the golden ratio
        return floor(capacity * fmod(key * A, 1));
    }

    // Resize the hash table (either grow or shrink)
    void resize(int newCapacity) {
        DoublyLinkedList* newTable = new DoublyLinkedList[newCapacity];
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i].head;
            while (current) {
                int newIndex = current->key % newCapacity;  // Rehashing
                newTable[newIndex].insert(current->key, current->value);
                current = current->next;
            }
        }
        delete[] table;
        table = newTable;
        capacity = newCapacity;
    }

public:
    HashTable(int cap = 8) : capacity(cap), size(0) {
        table = new DoublyLinkedList[capacity];
    }

    ~HashTable() {
        delete[] table;
    }

    // Insert a key-value pair
    void insert(int key, int value) {
        if ((float)size / capacity > LOAD_FACTOR) {
            resize(2 * capacity);  // Grow
        }

        int index = hashFunction(key);
        if (table[index].find(key)) {
            table[index].remove(key);  // Overwrite existing key
        }

        table[index].insert(key, value);
        ++size;
    }

    // Remove a key-value pair
    void remove(int key) {
        int index = hashFunction(key);
        table[index].remove(key);
        --size;

        if (capacity > 8 && (float)size / capacity < SHRINK_FACTOR) {
            resize(capacity / 2);  // Shrink
        }
    }

    // Find a value by key
    int find(int key) {
        int index = hashFunction(key);
        Node* result = table[index].find(key);
        if (result) return result->value;
        return -1;  // Key not found
    }

    // Print hash table (for debugging purposes)
    void printTable() {
        for (int i = 0; i < capacity; ++i) {
            cout << "Bucket " << i << ": ";
            Node* temp = table[i].head;
            while (temp) {
                cout << "(" << temp->key << ", " << temp->value << ") ";
                temp = temp->next;
            }
            cout << endl;
        }
    }
};

int main() {
    HashTable ht;
    ht.insert(1, 100);
    ht.insert(2, 200);
    ht.insert(15, 300);  // Collision with key 1 

    cout << "Find 1: " << ht.find(1) << endl;  // Should print 100
    cout << "Find 2: " << ht.find(2) << endl;  // Should print 200
    cout << "Find 15: " << ht.find(15) << endl;  // Should print 300

    ht.remove(2);
    cout << "Find 2 after deletion: " << ht.find(2) << endl;  // Should print -1

    ht.printTable();
    return 0;
}

/*
OUTPUT: 
Find 1: 100
Find 2: 200
Find 15: 300
Find 2 after deletion: -1
Bucket 0: 
Bucket 1: 
Bucket 2: (15, 300) 
Bucket 3: 
Bucket 4: (1, 100) 
Bucket 5: 
Bucket 6: 
Bucket 7: 
*/
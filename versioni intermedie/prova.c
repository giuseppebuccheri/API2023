#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

// Struttura per un elemento dell'hash table
struct HashNode {
    int key;
    int value;
    struct HashNode* next;
};

// Struttura per l'hash table
struct HashTable {
    struct HashNode* table[TABLE_SIZE];
};

// Funzione per inizializzare un hash table
void initHashTable(struct HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
}

// Funzione per l'hashing della chiave
int hashFunction(int key) {
    return key % TABLE_SIZE;
}

// Funzione per inserire un valore nell'hash table
void insert(struct HashTable* ht, int key, int value) {
    int index = hashFunction(key);

    struct HashNode* newNode = malloc(sizeof(struct HashNode));
    newNode->key = key;
    newNode->value = value;
    newNode->next = ht->table[index];

    ht->table[index] = newNode;
}

// Funzione per cercare un valore nell'hash table
int search(struct HashTable* ht, int key) {
    int index = hashFunction(key);

    struct HashNode* currentNode = ht->table[index];
    while (currentNode != NULL) {
        if (currentNode->key == key) {
            return currentNode->value;
        }
        currentNode = currentNode->next;
    }

    return -1; // Valore non trovato
}

void printHashTable(struct HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Bucket %d: ", i);

        struct HashNode* currentNode = ht->table[i];
        while (currentNode != NULL) {
            printf("(%d, %d) -> ", currentNode->key, currentNode->value);
            currentNode = currentNode->next;
        }

        printf("NULL\n");
    }
}

int main() {
    int inputValues[100];

    struct HashTable ht;
    initHashTable(&ht);

    printf("inserisci 5 valori\n");
    // Supponiamo di avere un array di valori da caricare nell'hash table
    for(int a=0;a<5;a++)
        scanf("%d",&inputValues[a]);

    for (int i = 0; i < 5; i++) {
        insert(&ht, i, inputValues[i]);
    }

    // Esempio di ricerca
    int keyToSearch = 12;
    int foundValue = search(&ht, keyToSearch);

    if (foundValue != -1) {
        printf("Value for key %d: %d\n", keyToSearch, foundValue);
    } else {
        printf("Key %d not found.\n", keyToSearch);
    }

    printf("Hash Table:\n");
    printHashTable(&ht);

    return 0;
}
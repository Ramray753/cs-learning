// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 31 * 31 * 31;

// Hash table
node *table[N];
int word_num;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int h_value = hash(word);
    if (table[h_value] == NULL){
        return false;
    }
    node *tmp = table[h_value];
    while (tmp != NULL){
        for (int i = 0; i < LENGTH + 1; i++){
            if ((char) tolower(word[i]) != tmp -> word[i]){
                break;
            }
            if (word[i] == '\0' && tmp -> word[i] == '\0'){
                return true;
            }
        }
        tmp = tmp -> next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int out = 7;
    int i = 0;
    while (word[i] != '\0'){
        out += out * 31 + tolower(word[i]);
        i++;
    }
    return out % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open file
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL){
        return false;
    }

    // Read each word
    char buffer[LENGTH + 1];
    int h_value;
    node *new;
    while (fscanf(fp, "%s\n", buffer) != EOF){
        new = malloc(sizeof(node));
        if (new == NULL){
            fclose(fp);
            return false;
        }
        strcpy(new -> word, buffer);
        new -> next = NULL;
        h_value = hash(buffer);
        new -> next = table[h_value];
        table[h_value] = new;
        word_num++;
    }
    fclose(fp);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_num;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++){
        node *tmp = table[i];
        node *cur;
        if (tmp != NULL){
            while (tmp != NULL){
                cur = tmp;
                tmp = tmp -> next;
                free(cur);
            }
        }
    }
    return true;
}

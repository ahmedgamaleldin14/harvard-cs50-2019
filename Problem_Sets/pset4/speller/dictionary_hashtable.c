// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

#define HASHTABLE_SIZE 65536

int word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[HASHTABLE_SIZE];

/* Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}*/

unsigned int hash(const char* word)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(word); i<n; i++)
        hash = (hash << 2) ^ tolower(word[i]);
    return hash % HASHTABLE_SIZE;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    int head = 0;
    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if(new_node == NULL)
        {
            unload();
            return false;
        }

        strcpy(new_node->word, word);
        head = hash(new_node->word);

        new_node->next = hashtable[head];
        hashtable[head] = new_node;

        word_count++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (word_count > 0)
    {
        return word_count;
    }
    else
    {
        return 0;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *cursor = hashtable[hash(word)];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int n = 0; n < HASHTABLE_SIZE; n++)
    {
        while (hashtable[n] != NULL)
        {
            node *cursor = hashtable[n];
            hashtable[n] = hashtable[n]->next;
            free(cursor);
        }
    }

    return true;
}

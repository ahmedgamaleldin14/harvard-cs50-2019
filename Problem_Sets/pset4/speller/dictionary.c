// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// to calculate the size of a dictionary
int word_count = 0;

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N+1];
}
node;

// Represents a a root for a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
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

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        for (int i = 0, n = strlen(word); i < n; i++)
        {
            index = islower(word[i]) - 'a';

            if (root->children[index] == NULL)
            {
                root->children[index] = malloc(sizeof(node));
                root = root->children[index];
            }
            else
            {
                root = root->children[index];
            }
        }
        children[i]->is_word = true;

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
    // TODO
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
}

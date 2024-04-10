// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// --------------------------------------------------------------------------------------------------------------------

// Represents a node in a hash table

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// --------------------------------------------------------------------------------------------------------------------

// Signatures

bool free_node(node *ptr);
bool load(const char *dictionary);
unsigned int hash(const char *word);
void find_null(node *table_ptr, node *ptr);

// --------------------------------------------------------------------------------------------------------------------

// Global variables

// TODO: Choose number of buckets in hash table
const unsigned int N = 143092;

// Hash table
node *table[143093] = {NULL};

// Count
unsigned int count = 0;

// --------------------------------------------------------------------------------------------------------------------

// Returns true if word is in dictionary, else false

bool check(const char *word)
{
    // Convert the user input to lower case and get the hash
    int length = strlen(word);
    char converted_word[length + 1];
    for (int i = 0; i < length + 1; i++)
    {
        converted_word[i] = tolower(word[i]);
    }
    unsigned int hash_index = hash(converted_word);

    node *tmp = NULL;
    tmp = table[hash_index];
    if (tmp == NULL)
    {
        return false;
    }
    if (strcmp(tmp->word, converted_word) == 0)
    {
        return true;
    }
    else
    {
        while (tmp != NULL)
        {
            tmp = tmp->next;
            if (strcmp(tmp->word, converted_word) == 0)
            {
                return true;
            }
        }
        return false;
    }
}

// --------------------------------------------------------------------------------------------------------------------

#define MULTIPLIER (37)

unsigned int hash(const char *s)
{
    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *)s;

    h = 0;
    while (*us != '\0')
    {
        h = h * MULTIPLIER + *us;
        us++;
    }

    return h % N;
}
// --------------------------------------------------------------------------------------------------------------------

// Loads dictionary into memory, returning true if successful, else false

bool load(const char *dictionary)
{
    // Open text file
    FILE *dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        return false;
    }

    char buffer[LENGTH + 1] = {0};
    int c = 0;
    int word_count = 0;

    while ((c = fgetc(dictionary_file)) != EOF)
    {
        buffer[word_count] = c;
        word_count++;
        if (c == ' ' || c == '\n')
        {
            // Terminate
            buffer[word_count - 1] = '\0';
            // Reset count
            word_count = 0;
            // Create pointer
            node *ptr = malloc(sizeof(node));
            ptr->next = NULL;

            // Copy word
            for (int i = 0; i < strlen(buffer); i++)
            {
                ptr->word[i] = buffer[i];
            }

            // Add element to list
            unsigned int hash_value = hash(ptr->word);

            // If the index is empty, set value to pointer.
            if (table[hash_value] == NULL)
            {
                table[hash_value] = ptr;
            }
            else
            {
                find_null(table[hash_value], ptr);
            }
            // Increase number of words in dictionary
            count++;

            // Reset buffer
            for (int i = 0; i < LENGTH + 1; i++)
            {
                buffer[i] = '\0';
            }
        }
    }
    fclose(dictionary_file);
    return true;
}

void find_null(node *table_ptr, node *ptr)
{
    if (table_ptr->next == NULL)
    {
        table_ptr->next = ptr;
        return;
    }
    find_null(table_ptr->next, ptr);
    return;
}

// --------------------------------------------------------------------------------------------------------------------

// Returns number of words in dictionary if loaded, else 0 if not yet loaded

unsigned int size(void)
{
    return count;
}

// --------------------------------------------------------------------------------------------------------------------

// Unloads dictionary from memory, returning true if successful, else false

bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            if (free_node(table[i]) == false)
            {
                return false;
            }
        }
    }
    return true;
}

bool free_node(node *ptr)
{
    // base case
    if (ptr->next == NULL)
    {
        free(ptr);
        // notify all pointers in the chain have been freed
        return true;
    }
    // Call the function on the next pointer in the chain
    free_node(ptr->next);
    // Free the node
    free(ptr);
    return true;
}

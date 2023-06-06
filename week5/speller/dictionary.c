// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

unsigned int numWords = 0;

// Returns true if word is in dictionary, else false
//is this word in the dictionary
bool check(const char *word)
{
    // TODO
    //getting the value to be able to check
    int thisindex = hash(word);
    //setting a pointer at the specific hash so we can traverse the list
    node *firstElement = table[thisindex];
    node *ptr = firstElement;
    while (ptr != NULL)
    {
        if (strcasecmp(ptr -> word, word) == 0)
        {
            return true;
        }
        ptr = ptr -> next;
    }
    return false;
}

// Hashes word to a number
//returns a number according to hashtable
//Consider a hash function that uses a sum of ASCII values or the length of a word <-prompt write up
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int total = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        total += toupper(word[i]);
    }
    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
//load all words int the dictionary into a hashtable
//take dictionary file and read from it
bool load(const char *dictionary)
{
    // TODO
    unsigned int index;

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        return false;
    }

    //place holder for strcpy
    char word[LENGTH + 1];
    //using fscanf and eof
    while (fscanf(file, "%s", word) != EOF)
    {
        node *newWord = malloc(sizeof(node));
        //if it could not allocate
        if (newWord == NULL)
        {
            printf("Could not allocate memory.\n");
            return false;
        }
        //copy word into node using strcpy
        //putting th srong word into the struct
        strcpy(newWord -> word, word);
        index = hash(word);
        newWord -> next = table[index];
        table[index] = newWord;
        numWords++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (numWords > 0)
    {
        return numWords;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
//freeing memory
bool unload(void)
{
    // TODO
    //to traverse a list
    node *ptr = NULL;
    //setting the element to first hash to traverse through the rest
    node *currentHash = NULL;
    for (int i = 0; i < N; i++)
    {
        currentHash = table[i];
        while (currentHash != NULL)
        {
            ptr = currentHash;
            currentHash = currentHash -> next;
            free(ptr);

        }
    }

    return true;
}

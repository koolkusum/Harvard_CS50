#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int letterCount(string text)
{
    // Calculate the length of the text
    int length = strlen(text);

    // Count the alphabetic characters
    int letters = 0;

    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }

    return letters;
}

int wordCount(string text)
{
    // Count the spaces to determine the number of words
    int words = 0;
    int length = strlen(text);

    for (int i = 0; i < length; i++)
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }

    return words + 1;
}

int sentCount(string text)
{
    // Count the sentence-ending punctuation marks
    int length = strlen(text);
    int sent = 0;

    for (int i = 0; i < length; i++)
    {
        if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            sent++;
        }
    }

    return sent;
}

int main(void)
{
    // Prompt the user for input
    string text = get_string("Text: ");

    // Get the counts
    int letterCnt = letterCount(text);
    int wordCnt = wordCount(text);
    int sentCnt = sentCount(text);

    // Calculate average sentence length and average letters per word
    double avgS = 100.0 * ((double) sentCnt / wordCnt);
    double avgL = 100.0 * ((double) letterCnt / wordCnt);

    // Calculate the Coleman-Liau index
    double index = 0.0588 * avgL - 0.296 * avgS - 15.8;

    // Print the reading level
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", roundf(index));
    }
}

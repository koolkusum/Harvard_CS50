#include <cs50.h>
#include <stdio.h>

int getStartSize()
{
    int size;
    do
    {
        size = get_int("Start size: ");
    }
    while (size < 9);
    return size;
}

int getEndSize(int startSize)
{
    int size;
    do
    {
        size = get_int("End size: ");
    }
    while (size < startSize);
    return size;
}

int main(void)
{
    //for n llamas we gain n/3 llamas
    //for n llamas we lose n/4 llamas

    // TODO: Prompt for start size
    int startSize = getStartSize();
    // TODO: Prompt for end size
    int endSize = getEndSize(startSize);
    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    while (startSize < endSize)
    {
        startSize = startSize + (startSize / 3) - (startSize / 4);
        years ++;
    }
    // TODO: Print number of years
    printf("Years: %i\n", years);
}

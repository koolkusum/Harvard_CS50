#include <cs50.h>
#include <stdio.h>

int getHeight()
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (1 > height || height > 8);
    return height;
}

int main(void)
{
    int j = 0;
    //getting height from user 1 to 8 exclusive
    int height = getHeight();
    for (int i = 0 ; i < height ; i++)
    {
        //printing the spaces
        for (j = 0; j < height - i - 1; j++)
        {
            printf(" ");
        }
        //printing the tag
        for (j = 0; j <= i; j++)
        {
            printf("#");
        }
        //space in between
        printf("  ");
        //second half
        for (j = 0; j <= i; j++)
        {
            printf("#");
        }

        printf("\n");
    }

}
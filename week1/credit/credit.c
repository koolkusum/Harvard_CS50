#include <cs50.h>
#include <stdio.h>
#include <string.h>

long getCredit()
{
    //can assume the input is numeric
    long number = get_long("Number: ");
    return number;
}


int checkSum(long creditNum, int length)
{
    //using to add every other stuff the way it needs to be added
    int alternate = 0;
    int sum = 0;
    while (creditNum > 0)
    {
        int digit = creditNum % 10;
        if (alternate == 0)
        {
            sum += digit;
            alternate = 1;
        }
        else
        {
            digit *= 2;
            sum += digit % 10 + digit / 10;
            alternate = 0;
        }
        creditNum /= 10;
    }
    int lastDig = sum % 10;
    return lastDig;
}

void checkCard(long creditNum)
{
    char strNum[20];
    //conveting to string to easily get the length of the long
    sprintf(strNum, "%ld", creditNum);
    int length = strlen(strNum);

    //length check
    if (length != 15 && length != 16 && length != 13)
    {
        printf("INVALID\n");
    }
    else
    {
        //performing the checksum
        int sumResult = checkSum(creditNum, length);
        if (sumResult != 0)
        {

            printf("INVALID\n");
        }
        //now checking if it's one of the following 3 cards using length
        else if (length == 15)
        {
            int firstDig = creditNum / 10000000000000;
            //AMEX CARD
            if (firstDig == 34 || firstDig == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (length == 16)
        {
            //Mastercard OR Visa
            int firstDig = creditNum / 100000000000000;
            //printf("%d", firstDig);
            if (firstDig >= 51 && firstDig <= 55)
            {
                printf("MASTERCARD\n");
            }
            else if (firstDig / 10 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (length == 13)
        {
            //VISA
            int firstDig = creditNum / 100000000000;
            if (firstDig / 10 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
}

int main(void)
{
    //american express 15-digit : 34 / 37
    //master card 16-digit : 51 52 53 54 55
    //Visa uses 13- and 16-digit: 4
    long creditNum = getCredit();
    checkCard(creditNum);
    return 0;
}
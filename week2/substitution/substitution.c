#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    //validation test works
    //not the best practice to harcode 26 but im lowkey lazy LOL
    if (argc < 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //first string input?
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    string cipher = argv[1];
    //for consistency we will set all letters to uppercase
    //also checking for alpahbetical
    for (int i = 0; i < 26; i++)
    {
        if (isalpha(cipher[i]))
        {
            cipher[i] = toupper(cipher[i]);
        }
        else
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
    }

    //check for repeats
    for (int i = 0; i < 26; i++)
    {
        for (int j = i + 1; j < 26; j++)
        {
            if (cipher[i] == cipher[j])
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }
    //if it passes all validation tests ask the user for input
    string userInput = get_string("plaintext: ");
    int lenUsrInp = strlen(userInput);
    char cipherText[lenUsrInp + 1];
    for (int i = 0; i < lenUsrInp; i++)
    {
        //is lower letter
        if (islower(userInput[i]) && isalpha(userInput[i]))
        {
            int index = userInput[i] - 'a';
            char target = cipher[index];
            cipherText[i] = tolower(target);
        }
        //is upper letter
        else if (isupper(userInput[i]) && isalpha(userInput[i]))
        {
            int index = userInput[i] - 'A';
            char target = cipher[index];
            cipherText[i] = toupper(target);
        }
        //if not a letter do no change
        else
        {
            cipherText[i] = userInput[i];
        }
    }
    cipherText[lenUsrInp] = '\0';
    printf("ciphertext: %s\n", cipherText);
    return 0;

}
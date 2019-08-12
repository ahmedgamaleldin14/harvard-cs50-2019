#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    //check the number of arguments
    if (argc != 2) 
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    else
    {   
        //to count the number of characters in the argument
        int number = 0;
        int n = strlen(argv[1]);
        for (int i = 0; i < n; i++)
        {
            if (isalpha(argv[1][i]) != 0)
            {
                number += 1;
            }
        }
        //check if the number counter is not equal to the No. of characters
        if (number != strlen(argv[1]))
        {
            printf("Usage: ./vigenere keyword\n");
            return 1;
        }
    }
        
    string plaintext = get_string("plaintext: ");
    int len = strlen(plaintext);
    int k = 0;
    int vig_key;
    printf("ciphertext: ");
    for (int i = 0; i < len; i++)
    {
        //to calculate the vigenere key
        vig_key = shift(argv[1][k]);
            
        //for non-alphabetic symbols to be printed the same
        if (!isalpha(plaintext[i]))
        {
            printf("%c", plaintext[i]);
            k--;
        }
        //use the modular expression to calculate the cipher key
        else if (islower(plaintext[i]))
        {
            printf("%c", ((plaintext[i] - 'a' + vig_key) % 26) + 'a');
        }
        else if (isupper(plaintext[i])) 
        {
            printf("%c", ((plaintext[i] - 'A' + vig_key) % 26) + 'A');
        }
        
        //increase k for the next key character
        k++;
        //loop over if you reached the last key    
        if (k == strlen(argv[1]))
            k = 0;
    }
    printf("\n");  
}


int shift(char c)
{
    if isupper(c)
        return c - 'A';
    else if islower(c)
        return c - 'a';
    else
        return 1;
}

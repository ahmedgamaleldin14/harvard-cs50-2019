#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{    
    //check the number of arguments
    if (argc != 2)
    {
        printf("Usage: ./ceaser key\n");
        return 1;
    }
    else
    {   
        //to count the number of digits in the argument
        int number = 0;
        int n = strlen(argv[1]);
        for (int i = 0; i < n; i++)
        {
            if (isdigit(argv[1][i]) != 0)
            {
                number += 1;
            }
        }
        //check the number counter is not equal to the digits
        if (number != strlen(argv[1]))
        {
            printf("Usage: ./ceaser key\n");
            return 1;
        }
    }

    int key = atoi(argv[1]);  
    string plaintext = get_string("plaintext: ");
    int k = strlen(plaintext);
    printf("ciphertext: ");
    for (int i = 0; i < k; i++)
    {
        //for symbols
        if (plaintext[i] < 'A' || plaintext[i] > 'z'
           || (plaintext[i] > 'Z' && plaintext[i] < 'a'))
        {
            printf("%c", plaintext[i]);
        }
        //use the modular expression to calculate the cipher key
        else if (islower(plaintext[i]))
        {
            printf("%c", ((plaintext[i] - 'a' + key) % 26) + 'a');
        }
        else if (isupper(plaintext[i])) 
        {
            printf("%c", ((plaintext[i] - 'A' + key) % 26) + 'A');
        }
    }
    printf("\n");  
}




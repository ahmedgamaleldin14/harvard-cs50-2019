#include <cs50.h>
#include <stdio.h>

void pyramids(int n);
int get_right_int(string prompt);

int main(void)
{
    
    int hash_number = get_right_int("Pyramids Height? ");
    pyramids(hash_number);
}

int get_right_int(string prompt)
{
    int n;
    do 
    {
        n = get_int("%s", prompt);
    }
    while (n < 1 || n > 8);
    return n;
}

void pyramids(int n)
{
    int count=1;
    for (int i = n; i > 0; i--)
        {
            for (int k = i; k > 1; k--)
            {
                printf(" ");
            }
        
            for (int k = 0; k < count; k++)
            {
                printf("#");
            }
            printf("  ");
            for (int k = 0; k < count; k++)
            {
                printf("#");
            }
            printf("\n");
        
            count += 1;
        }
}

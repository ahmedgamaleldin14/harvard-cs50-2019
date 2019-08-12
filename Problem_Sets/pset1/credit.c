#include <stdio.h>
#include <cs50.h>
#include <math.h>

void check(long number);

int main(void)
{
    long credit_number = get_long("Your Card Number? ");
    //printf("Number: %ld\n", credit_number);
    
    check(credit_number);
    //printf("Sum: %d\n", sum);
}

void check(long number)
{
    int count = 0;
    int multiply = 0;
    int no_multiply = 0;
    long step = number;
    int sum = 0;
    long divider = number;
    
    do
    {
        divider = divider/10;
        count++;
        //printf("Count: %d\n", count);
        //printf("divider: %ld\n", divider);
    }
    while(divider > 0);
    
    if (count == 13 || count == 15)
    {
        int k = count;
        for (int i=0; i < count/2; i++)
        {
            multiply = ((step / (long)round(pow(10,k-2))) % 10) * 2;
            if (multiply > 9)
            {
                int left = multiply / 10;
                int right = multiply % 10;
                sum += left + right;
            }
            else
            {
                sum += multiply;
            }
            no_multiply += (step / (long)round(pow(10,k-2))) / 10;
            //printf("multiply: %d\n", multiply);
            //printf("no multiply: %d\n", no_multiply);
            step = (step % (long)round(pow(10,k-2)));
            //printf("step: %ld\n", step);
            k -= 2;
        }      
        sum += no_multiply + step;
        int check_visa = number / (long)round(pow(10,count-1));
        //printf("visa: %d\n", check_visa);
        int check_AMEX = number / (long)round(pow(10,count-2));
        //printf("AMEX: %d\n", check_AMEX);
        if (sum % 10 == 0)
        {
            if (check_visa == 4)
            {
                printf("VISA\n");
            }
            else if (check_AMEX == 34 || check_AMEX == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    
    else if (count == 16)
    {
        int k = count;
        for (int i=0; i < count/2; i++)
        {
            multiply = ((step / (long)round(pow(10,k-2))) / 10) * 2;
            if (multiply > 9)
            {
                int left = multiply / 10;
                int right = multiply % 10;
                sum += left + right;
            }
            else
            {
                sum += multiply;
            }
            no_multiply += (step / (long)round(pow(10,k-2))) % 10;
            //printf("multiply: %d\n", multiply);
            //printf("no multiply: %d\n", no_multiply);
            step = (step % (long)round(pow(10,k-2)));
            //printf("step: %ld\n", step);
            k -= 2;
        }   
        sum += no_multiply + step;
        int check_visa = number / (long)round(pow(10,count-1));
        int check_MasterCard = number / (long)round(pow(10,count-2));
        
        if (sum % 10 == 0)
        {
            if (check_visa == 4)
            {
                printf("VISA\n");
            }
            else if (check_MasterCard == 51 || check_MasterCard == 52 || check_MasterCard == 53 || check_MasterCard == 54 || check_MasterCard == 55)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}



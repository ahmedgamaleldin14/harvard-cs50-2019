#include <stdio.h>
#include <cs50.h>
#include <math.h>

float get_right_number(string prompt);
int number_of_coins(int money);

int main(void)
{
    float money = get_right_number("Change owed: ");
    int cents = round(money * 100);
    int coins = number_of_coins(cents);
    printf("%d\n", coins);
}

float get_right_number(string prompt)
{
    float n;
    do
    {
        n = get_float("%s", prompt);
    }
    while(n < 0.00);
    return n;
}

int number_of_coins(int money)
{
    int coins = 0;
    do 
    {
        if (money / 25 != 0)
        {
            money = money - 25;
            coins += 1;
        }
        else
        {
            if (money / 10 != 0)
            {
                money = money - 10;
                coins += 1;
            }
            else
            {
                if (money / 5 != 0)
                {
                    money = money - 5;
                    coins += 1;
                }
                else
                {
                    if (money / 1 != 0)
                    {
                        money = money - 1;
                        coins += 1;
                    }
                }
            }
        }
    }
    while (money > 0);
    return coins;
}

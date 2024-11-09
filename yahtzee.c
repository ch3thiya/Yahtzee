/* 
Programmer: Chethiya Ravindranath
Last Changes Made On: 09/11/2024
Module: Programming Methodology

Yahtzee Game
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void rollDice(int values[]);
void printDice(int values[]);
void rerollDice(int values[]);

int main(void)
{
    int values[5] = {};
    int reroll;
    int repeats = 0;
    int category = 0;
    char rollagain;
    int count = 0;
    int onescore;
    int rep_array[2][6] = {{1, 2, 3, 4, 5, 6}, {0, 0, 0, 0, 0, 0}};
    bool two = false;
    bool three = false;
    bool four = false;
    bool largeStraight = false;
    bool smallStraight = false;

    srand(time(0));

    rollDice(values);

    while (repeats < 3)
    {
        printf("Do you want to reroll? (y/n)\n");
        scanf(" %c", &rollagain);

        if (rollagain == 'n')
        {
            break;
        }
        else if (rollagain == 'y')
        {
            rerollDice(values);
            repeats++;
        }
        else
        {
            printf("Invalid Input! Please enter 'y' or 'n'.\n");
        }
    }

    printf("Final dice values: ");
    printDice(values);

    printf("Give a category (1-6 for specific score calculation): ");
    scanf("%d", &category);


    if (category > 0 && category < 7)
    {
        count = 0;
        for (int i = 0; i < 5; i++)
        {
            if (values[i] == category)
            {
                count++;
            }
        }
        onescore = category * count;
        printf("The score for category %d is %d!\n", category, onescore);
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            count = 0;
            for (int j = 0; j < 5; j++)
            {
                if (values[i] == values[j])
                {
                    count++;
                }
            }

            if (i == 0 || values[i] != values[i - 1])
            {
                printf("The number %d appears %d times.\n", values[i], count);
                if (values[i] >= 1 && values[i] <= 6)  
                {
                    rep_array[1][values[i] - 1] = count;  
                }
            }
        }
    }

    for (int i = 0; i < 7; i++)
    {
        if (rep_array[1][i] == 2)
        {
            two = true;
        }
        if (rep_array[1][i] == 3)
        {
            three = true;
        }
        if (rep_array[1][i] == 4)
        {
            two = false;
            three = false;
            four = true;
        }
    }

    if (rep_array[1][0] == 1 && rep_array[1][1] == 1 && rep_array[1][2] == 1 &&
        rep_array[1][3] == 1 && rep_array[1][4] == 1)
    {
        largeStraight = true;
    }
    else if (rep_array[1][1] == 1 && rep_array[1][2] == 1 && rep_array[1][3] == 1 &&
            rep_array[1][4] == 1 && rep_array[1][5] == 1)
    {
        largeStraight = true;
    }
    else if (rep_array[1][0] >= 1 && rep_array[1][1] >= 1 && rep_array[1][2] >= 1 && rep_array[1][3] >= 1)
    {
        smallStraight = true;
    }
    else if (rep_array[1][1] >= 1 && rep_array[1][2] >= 1 && rep_array[1][3] >= 1 && rep_array[1][4] >= 1)
    {
        smallStraight = true;
    }
    else if (rep_array[1][2] >= 1 && rep_array[1][3] >= 1 && rep_array[1][4] >= 1 && rep_array[1][5] >= 1)
    {
        smallStraight = true;
    }

    if (three && !four && !two)
    {
        printf("Three of a kind.\n");
    }
    if (four)
    {
        printf("Four of a kind.\n");
    }
    if (two && three)
    {
        printf("Full house.\n");
    }
    if (largeStraight)
    {
        printf("Large Straight.\n");
    }
    if (smallStraight)
    {
        printf("Small Straight.\n");
    }


    return 0;
}

void rollDice(int values[])
{
    for (int i = 0; i < 5; i++)
    {
        values[i] = (rand() % 6) + 1;
    }
    printDice(values);
}

void printDice(int values[])
{
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", values[i]);
    }
    printf("\n");
}

void rerollDice(int values[])
{
    int numRerolls;
    printf("How many dice do you want to reroll (1 to 5)? ");
    scanf("%d", numRerolls);

    if (numRerolls < 1 || numRerolls > 5) {
        printf("Invalid number of rerolls. Please enter a number between 1 and 5.\n");
        return 1;
    }

    printf("Enter the dice indices you want to reroll (1-5):\n");
    for (int i = 0; i < numRerolls; i++)
    {
        int rerollIndex;
        scanf("%d", &rerollIndex);

        if (rerollIndex < 1 || rerollIndex > 5) {
            printf("Invalid index %d. Please enter a number between 1 and 5.\n", rerollIndex);
            i--;
            continue;
        }

        values[rerollIndex - 1] = (rand() % 6) + 1;
    }

    printf("Updated dice values: ");
    printDice(values);
}

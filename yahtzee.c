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
void getPatterns(int rep_array[2][6], bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee, int category);
void countScore(int values[], int category, int *onescore);
void givePoints(int values[], int point_array[3][12], int category, int *onescore);
int getSum(int values[]);

int main(void)
{
    int values[5] = {2,2,3,3,3};
    int reroll;
    int repeats = 0;
    int category = 0;
    char rollagain;
    int onescore = 0;
    int rep_array[2][6] = {{1, 2, 3, 4, 5, 6}, {0, 0, 0, 0, 0, 0}};
    int point_array[3][12] = {{1,2,3,4,5,6,7,8,9,10,11,12}, {0,0,0,0,0,0,0,25,30,40,0,50}, {0,0,0,0,0,0,0,0,0,0,0,0}};
    bool two = false, three = false, four = false;
    bool largeStraight = false, smallStraight = false, yahtzee = false;

    int x=0;
    while (x < 13){    
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

        printf("Give a category: ");
        scanf("%d", &category);

        if (category > 0 && category < 7)
        {
            countScore(values, category, &onescore);
        }
        else
        {
            getPatterns(rep_array, &two, &three, &four, &smallStraight, &largeStraight, &yahtzee, category);
        }

        if (point_array[2][category-1] == 0)
        {
            givePoints(values, point_array, category, &onescore);
        }
        else
        {
            printf("Points already given!");
        }
        x++;

    }

    int full = 0;
    for(int i=0; i<12; i++)
    {
        full += point_array[1][i];
    }
    printf("\npoints: %d", full);
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
    scanf("%d", &numRerolls);

    if (numRerolls < 1 || numRerolls > 5) {
        printf("Invalid number of rerolls. Please enter a number between 1 and 5.\n");
        return;
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

void countScore(int values[], int category, int *onescore)
{
    int count = 0;
    for (int i = 0; i < 5; i++)
    {
        if (values[i] == category)
        {
            count++;
        }
    }
    *onescore = category * count;
}

void getPatterns(int rep_array[2][6], bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee, int category)
{
    for (int i = 0; i < 6; i++)
    {
        if (rep_array[1][i] == 2)
        {
            *two = true;
        }
        if (rep_array[1][i] == 3)
        {
            *three = true;
        }
        if (rep_array[1][i] == 4)
        {
            *four = true;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (rep_array[i] && rep_array[i + 1] && rep_array[i + 2] && rep_array[i + 3])
        {
            *smallStraight = true;
            if (rep_array[i + 4])
            {
                *largeStraight = true;
            }
        }
    }

    if (rep_array[5] == 5)
    {
        *yahtzee = true;
    }

    if ((category == 7) && three)
    {
        printf("Three of a kind.\n");
    }
    if ((category == 8) && four)
    {
        printf("Four of a kind.\n");
    }
    if ((category == 9) && (two && three))
    {
        printf("Full house.\n");
    }
    if ((category == 10) && smallStraight)
    {
        printf("Small Straight.\n");
    }
    if ((category == 11) && largeStraight)
    {
        printf("Large Straight.\n");
    }
    if (category == 12)
    {
        printf("Chance.\n");
    }
    if ((category == 13) && yahtzee)
    {
        printf("YAHTZEE!!");
    }
}

void givePoints(int values[], int point_array[3][12], int category, int *onescore)
{
    if (category == 9 || category == 10 || category == 11 || category == 13)
    {
        point_array[2][category-1] = 1;
    }
    else if (category == 7 || category == 8 || category == 12)
    {
        point_array[1][category-1] = getSum(values);
        point_array[2][category-1] = 1;
    }
    else
    {
        point_array[1][category-1] = *onescore;
        point_array[2][category-1] = 1;
    }
}

int getSum(int values[])
{
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += values[i];
    }

    return sum;
}


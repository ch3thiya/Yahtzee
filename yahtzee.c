/* 
Programmer: Chethiya Ravindranath
Last Changes Made On: 10/11/2024
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
void givePoints(int values[], bool *valid, int point_array[3][13], int category, int *onescore);
int getSum(int values[]);
void checkValid(int point_array[3][13], bool *valid, bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee, int category);
void updateRepArray(int values[], int rep_array[2][6]);

int main(void)
{
    int values[5] = {};
    int repeats = 0;
    int category = 0;
    char rollagain;
    int onescore = 0;
    int rep_array[2][6] = {{1, 2, 3, 4, 5, 6}, {0, 0, 0, 0, 0, 0}};
    int point_array[3][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}, {0,0,0,0,0,0,0,0,0,0,0,0,0}};
    bool two = false, three = false, four = false;
    bool largeStraight = false, smallStraight = false, yahtzee = false;
    bool valid = false, correctCategory = true;

    int x=0;
    srand(time(0));

    while (x < 13)
    {    
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


        do {
            printf("Give a category: ");
            scanf("%d", &category);

            if (category < 1 || category > 13)
            {
                printf("Invalid category! Please enter a number between 1 and 13.\n");
                continue;
            }

            if (category > 0 && category < 7)
            {
                countScore(values, category, &onescore);
            } 
            else
            {
                updateRepArray(values, rep_array);
                getPatterns(rep_array, &two, &three, &four, &smallStraight, &largeStraight, &yahtzee, category);
            }

            if (point_array[2][category - 1] != 0)
            {
                printf("Points already given! Choose another category.\n");
            }
            else
            {
                checkValid(point_array, &valid, &two, &three, &four, &smallStraight, &largeStraight, &yahtzee, category);
                givePoints(values, &valid, point_array, category, &onescore);
                break; 
            }
        } while (1);


        for (int i = 0; i < 13; i++)
        {
            printf("%d ", point_array[0][i]);
        }
        printf("\n");

        for (int i = 0; i < 13; i++)
        {
            printf("%d ", point_array[1][i]);
        }
        printf("\n");

        for (int i = 0; i < 13; i++)
        {
            printf("%d ", point_array[2][i]);
        }
        printf("\n\n");

        repeats = 0;
        x++;
    }

    int full = 0;
    for(int i=0; i<13; i++)
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
    printf("How many dice do you want to reroll (1 to 5)?\n");
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
    *two = *three = *four = *smallStraight = *largeStraight = *yahtzee = false;

    for (int i = 0; i < 6; i++) {
        if (rep_array[1][i] == 2) *two = true;
        if (rep_array[1][i] == 3) *three = true;
        if (rep_array[1][i] == 4) *four = true;
        if (rep_array[1][i] == 5) *yahtzee = true;
    }

    for (int i = 0; i <= 2; i++)
    { 
        if (rep_array[1][i] && rep_array[1][i + 1] && rep_array[1][i + 2] && rep_array[1][i + 3])
        {
            *smallStraight = true; 
            if (i <= 1 && rep_array[1][i + 4])
            {
                *largeStraight = true;
            }
        }
    }

    if ((category == 7) && *three)
    {
        printf("Three of a kind.\n");
    }
    if ((category == 8) && *four)
    {
        printf("Four of a kind.\n");
    }
    if ((category == 9) && (*two && *three))
    {
        printf("Full house.\n");
    }
    if ((category == 10) && *smallStraight)
    {
        printf("Small Straight.\n");
    }
    if ((category == 11) && *largeStraight)
    {
        printf("Large Straight.\n");
    }
    if (category == 12)
    {
        printf("Chance.\n");
    }
    if ((category == 13) && *yahtzee)
    {
        printf("YAHTZEE!!\n");
    }
}

void givePoints(int values[], bool *valid, int point_array[3][13], int category, int *onescore)
{
    if (*valid)
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
    else
    {
        printf("The combination is not valid!\n");
        point_array[1][category-1] = 0;
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

void checkValid(int point_array[3][13], bool *valid, bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee, int category)
{
    *valid = false;
    {
        for (int i = 0; i < 7; i++)
        {
            if (i == category)
            {
                *valid = true;
            }
        }

        switch (category)
        {
        case 7:
            if (*three)
                *valid = true;
            break;

        case 8:
            if (*four)
                *valid = true;
            break;

        case 9:
            if (*two && *three)
                *valid = true;
            break;

        case 10:
            if (*smallStraight)
                *valid = true;
            break;

        case 11:
            if (*largeStraight)
                *valid = true;
            break;

        case 12:
            *valid = true;
            break;

        case 13:
            if (*yahtzee)
                *valid = true;
            break;
    
        default:
            break;
        }
    }
}

void updateRepArray(int values[], int rep_array[2][6])
{
    for (int i = 0; i < 6; i++)
    {
        rep_array[1][i] = 0;
    }

    for (int i = 0; i < 5; i++)
    {
        rep_array[1][values[i] - 1]++;
    }
}
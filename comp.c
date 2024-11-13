#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void rollDice(int values[]);
void printDice(int values[]);
void rerollDice(int values[]);
int getSum(int values[]);
void getPatterns(int rep_array[2][6], bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee);
void updateRepArray(int values[], int rep_array[2][6]);
void printPoints(int point_array[3][13]);

int main(void)
{
    int values[5] = {1,1,1,1,3};
    int repeats = 0;
    int category = 0;
    char rollagain;
    int onescore = 0;
    int rep_array[2][6] = {{1, 2, 3, 4, 5, 6}, {0, 0, 0, 0, 0, 0}};
    int point_array[3][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}, {0,0,0,0,0,0,0,0,0,0,0,0,0}};
    bool two = false, three = false, four = false;
    bool largeStraight = false, smallStraight = false, yahtzee = false;
    bool valid = false, correctCategory = true;

    int available_category[3][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}, {0,0,0,0,0,0,0,0,0,0,0,0,0}};
    int computer_points[2][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}};
    int available_points;
    bool hasValidPattern = false;
    int i = 0;


    int x=0;
    srand(time(0));

    while (x < 13)
    {
        rollDice(values);
        printf("Final dice values: ");
        printDice(values);

        // Update upper section categories with points based on the dice values
        for (int i = 1; i <= 6; i++) {
            int available_points = 0;
            for (int j = 0; j < 5; j++) {
                if (values[j] == i) {
                    available_points += i;
                }
            }
            available_category[1][i-1] = available_points;
            if (available_category[2][i-1] == 0) { 
                available_category[2][i-1] = 1;  // Mark as available
            }
        }

        // Update lower section categories based on dice patterns
        available_category[1][6] = available_category[1][7] = available_category[1][11] = getSum(values);
        updateRepArray(values, rep_array);
        getPatterns(rep_array, &two, &three, &four, &smallStraight, &largeStraight, &yahtzee);

        hasValidPattern = three || four || (two && three) || smallStraight || largeStraight || yahtzee;
        while (!hasValidPattern && i < 2)
        {
            printf("No valid pattern found; rerolling all dice.\n");
            rollDice(values);  // Reroll all dice
            printf("Rerolled values: ");
            printDice(values);
            available_category[1][6] = available_category[1][7] = available_category[1][11] = getSum(values);
            updateRepArray(values, rep_array);
            getPatterns(rep_array, &two, &three, &four, &smallStraight, &largeStraight, &yahtzee);
            i++;
        }

        // Set lower section category availability
        if (three && available_category[2][6] == 0) available_category[2][6] = 1;
        if (four && available_category[2][7] == 0) available_category[2][7] = 1;
        if (two && three && available_category[2][8] == 0) available_category[2][8] = 1;
        if (smallStraight && available_category[2][9] == 0) available_category[2][9] = 1;
        if (largeStraight && available_category[2][10] == 0) available_category[2][10] = 1;
        if (yahtzee && available_category[2][12] == 0) available_category[2][12] = 1;

        for (int i = 0; i < 13; i++) {
            printf("%d ", available_category[0][i]);
        }
        printf("\n");

        for (int i = 0; i < 13; i++) {
            printf("%d ", available_category[1][i]);
        }
        printf("\n");

        for (int i = 0; i < 13; i++) {
            printf("%d ", available_category[2][i]);
        }
        printf("\n\n");

        // Find the best scoring category
        int max_points = 0;
        int best_category = -1;
        for (int i = 0; i < 13; i++) {
            if (available_category[2][i] == 1 && available_category[1][i] > max_points) {
                max_points = available_category[1][i];
                best_category = i;
            }
        }

        // Fallback to a random category if no best scoring category is available
        if (best_category == -1) {
            int random_category;
            do {
                random_category = rand() % 13;
            } while (available_category[2][random_category] != 0);
            best_category = random_category;
            max_points = 0;  // Assign zero points for fallback category
        }

        // Score the chosen category and mark it as used
        point_array[1][best_category] = max_points;
        available_category[2][best_category] = 2;  // Mark as used
        available_category[1][best_category] = 0;  // Reset points to prevent reuse

        for (int i = 0; i < 13; i++) {
            printf("%d ", available_category[0][i]);
        }
        printf("\n");

        for (int i = 0; i < 13; i++) {
            printf("%d ", available_category[1][i]);
        }
        printf("\n");

        for (int i = 0; i < 13; i++) {
            printf("%d ", available_category[2][i]);
        }
        printf("\n\n");

        printPoints(point_array);

        x++;
    }
    printPoints(point_array);

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

int getSum(int values[])
{
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += values[i];
    }

    return sum;
}

void getPatterns(int rep_array[2][6], bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee)
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

void printPoints(int point_array[3][13])
{
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
}
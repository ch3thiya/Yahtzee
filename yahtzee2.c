/* 
Programmer: Chethiya Ravindranath
Last Changes Made On: 13/11/2024
Module: Programming Methodology

Yahtzee Game
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct{
int player_total;
int computer_total;
} TotalScores;

void rollDice(int values[]);
void printDice(int values[]);
void rerollDice(int values[]);
void getPatterns(int rep_array[2][6], bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee);
void countScore(int values[], int category, int *onescore);
void givePoints(int values[], bool *valid, int point_array[3][13], int category, int *onescore);
void checkValid(int point_array[3][13], bool *valid, bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee, int category);
void updateRepArray(int values[], int rep_array[2][6]);
void compReroll(int values[], int rep_array[2][6], int available_category[3][13], bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee);
void displayTable(int point_array[3][13], int computer_points[2][13], int available_category[3][13], int x);
void compPoints(int available_category[3][13], int computer_points[2][13]);
int getSum(int values[]);
TotalScores printFullPoints(int point_array[3][13], int computer_points[2][13], int available_category[3][13]);


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

    int available_category[3][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}, {0,0,0,0,0,0,0,0,0,0,0,0,0}};
    int computer_points[2][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}};
    int available_points;
    int x=0;
    bool hasValidPattern = false;
    int i = 0;
    int player_total = 0;
    int computer_total = 0;

    srand(time(0));

    printf("Welcome to Yahtzee!!\n\n");

    while (x < 13)
    {
        printf("Round : %d\n", x+1);
        printf("Your Turn\n\n");
        printf("Rolling dice...\n");
        rollDice(values);
        printf("Your dice values: ");   
        printDice(values);

        while (repeats < 2)
        {
            printf("Do you want to reroll? (y/n) ");
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

        do {
            printf("1. Ones\n2. Twos\n3. Threes\n4. Fours\n5. Fives\n6. Sixes\n");
            printf("7. Three of a kind\n8. Four of a kind\n9. Full House\n10. Small Straight\n11. Large Straight\n12. Chance\n13. Yahtzee\n");
            printf("Give a category (1 to 13):\n");
            scanf("%d", &category);

            if (category < 1 || category > 13)
            {
                printf("Invalid category! Please enter a number between 1 and 13.\n");
                continue;
            }

            if (point_array[2][category - 1] != 0)
            {
                printf("Points already assigned in this category! Choose another category.\n\n");
                continue;
            }
            else
            {
                if (category >= 1 && category <= 6)
                {
                    countScore(values, category, &onescore);
                } else
                {
                    updateRepArray(values, rep_array);
                    getPatterns(rep_array, &two, &three, &four, &smallStraight, &largeStraight, &yahtzee);
                }
                checkValid(point_array, &valid, &two, &three, &four, &smallStraight, &largeStraight, &yahtzee, category);
                givePoints(values, &valid, point_array, category, &onescore);
                point_array[2][category - 1] = 1;  // Mark as used
                break;
            }

        } while (1);
        repeats = 0;

        printf("\nComputer's Turn\n");
        rollDice(values);
        printf("Computer's dice values: ");
        printDice(values);

        two = three = four = smallStraight = largeStraight = yahtzee = false;

        for (int i = 1; i <= 6; i++) {
            int available_points = 0;
            for (int j = 0; j < 5; j++) {
                if (values[j] == i) {
                    available_points += i;
                }
            }
            available_category[1][i-1] = available_points;
            if (available_category[2][i-1] == 0)
            { 
                available_category[2][i-1] = 1;
            }  
        }

        compReroll(values, rep_array, available_category, &two, &three, &four, &smallStraight, &largeStraight, &yahtzee);
        compPoints(available_category, computer_points);

        displayTable(point_array, computer_points, available_category, x);
        printf("\n\n");

        x++;
    }

    return 0;
}

void rollDice(int values[])
{
    for (int i = 0; i < 5; i++)
    {
        values[i] = (rand() % 6) + 1;
    }
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

    printf("Enter the dice indexes you want to reroll (1 to 5):\n");
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
        printf("Scored %d in category %d\n", point_array[1][category-1],category);

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
    // Reset count array
    for (int i = 0; i < 6; i++)
    {
        rep_array[1][i] = 0;
    }

    // Count occurrences of each die value
    for (int i = 0; i < 5; i++)
    {
        rep_array[1][values[i] - 1]++;
    }
}

TotalScores printFullPoints(int point_array[3][13], int computer_points[2][13], int available_category[3][13])
{
    TotalScores totals = {0, 0};  // Initialize both totals to 0

    for (int i = 0; i < 13; i++) {
        // Calculate player points: If category is used, add points, else 0
        int player_point = point_array[2][i] == 1 ? point_array[1][i] : 0;

        // Calculate computer points: If category is marked as used, add points, else 0
        int computer_point = available_category[2][i] == 2 ? computer_points[1][i] : 0;

        // Add to totals
        totals.player_total += player_point;
        totals.computer_total += computer_point;
    }

    return totals; // Return the structure with both totals
}



void compReroll(int values[], int rep_array[2][6], int available_category[3][13], bool *two, bool *three, bool *four, bool *smallStraight, bool *largeStraight, bool *yahtzee) {
    int rerolls = 0;
    bool hasValidPattern = false;

    // Update pattern occurrences and check if there's a valid pattern initially
    updateRepArray(values, rep_array);
    getPatterns(rep_array, two, three, four, smallStraight, largeStraight, yahtzee);
    hasValidPattern = *three || *four || (*two && *three) || *smallStraight || *largeStraight || *yahtzee;

    // Reroll if no valid pattern and rerolls are allowed
    while (!hasValidPattern && rerolls < 2) {
        printf("Rerolling dice...\n");

        for (int i = 0; i < 5; i++) {
            int diceValue = values[i];
            bool keep = false;

            // Decide which dice to keep based on pattern matches
            if (*four || *yahtzee) {
                keep = true;
            } else if (*three) {
                keep = (rep_array[1][diceValue - 1] >= 3);
            } else if (*two) {
                keep = (rep_array[1][diceValue - 1] >= 2);
            } else if (*smallStraight || *largeStraight) {
                keep = true;
            }

            // Reroll dice not matching a required pattern
            if (!keep) {
                values[i] = (rand() % 6) + 1;  // Assign random dice value from 1 to 6
            }
        }

        printf("Rerolled values: ");
        printDice(values);

        // Update patterns after reroll
        updateRepArray(values, rep_array);
        getPatterns(rep_array, two, three, four, smallStraight, largeStraight, yahtzee);
        hasValidPattern = *three || *four || (*two && *three) || *smallStraight || *largeStraight || *yahtzee;

        rerolls++;
    }

    // Update available categories based on pattern found
    available_category[1][6] = available_category[1][7] = available_category[1][11] = getSum(values);
    if (*three && available_category[2][6] == 0) available_category[2][6] = 1;            // Three of a Kind
    if (*four && available_category[2][7] == 0) available_category[2][7] = 1;            // Four of a Kind
    if (*two && *three && available_category[2][8] == 0) available_category[2][8] = 1;   // Full House
    if (*smallStraight && available_category[2][9] == 0) available_category[2][9] = 1;   // Small Straight
    if (*largeStraight && available_category[2][10] == 0) available_category[2][10] = 1; // Large Straight
    if (*yahtzee && available_category[2][12] == 0) available_category[2][12] = 1;       // YAHTZEE
}


void displayTable(int point_array[3][13], int computer_points[2][13], int available_category[3][13], int x)
{
    const char *categories[] =
    {"Ones", "Twos", "Threes", "Fours", "Fives", "Sixes", "Three of a Kind", "Four of a Kind", "Full House", "Small Straight", "Large Straight", "Chance", "YAHTZEE"};
    
    printf("\n-----------------------------------------------\n");
    printf("| %-15s | %-10s | %-10s |\n", "Category", "Player", "Computer");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < 13; i++)
    {
        int player_point = point_array[2][i] == 1 ? point_array[1][i] : 0;
        int computer_point = available_category[2][i] == 2 ? computer_points[1][i] : 0;
        printf("| %-15s | %-10d | %-10d |\n", categories[i], player_point, computer_point);
    }

    printf("-----------------------------------------------\n");
    
    TotalScores totals = printFullPoints(point_array, computer_points, available_category);
    printf("| %-15s | %-10d | %-10d |\n", "Total Score", totals.player_total, totals.computer_total);
    printf("-----------------------------------------------\n");

    if (x == 12)
    {
        printf("Final Scores:\nPlayer: %d\nComputer: %d\n", totals.player_total, totals.computer_total);
        if (totals.player_total > totals.computer_total)
        {
            printf("Congratulations!! The winner is Player!\n");
        }
        else if (totals.computer_total > totals.player_total)
        {
            printf("Computer Wins!\n");
        }
        else
        {
            printf("It's a Tie!\n");
        }
    }
}

void compPoints(int available_category[3][13], int computer_points[2][13])
{
    int max_points = 0;
    int best_category = -1;
    for (int i = 0; i < 13; i++) {
        if (available_category[2][i] == 1 && available_category[1][i] > max_points) {
            max_points = available_category[1][i];
            best_category = i;
        }
    }

    if (best_category == -1) {
        int random_category;
        do {
            random_category = rand() % 13;
        } while (available_category[2][random_category] != 0);
        best_category = random_category;
        max_points = 0;  
    }

    computer_points[1][best_category] = max_points;
    available_category[2][best_category] = 2;
    available_category[1][best_category] = 0; 

    printf("Scored %d in category %d\n", max_points, available_category[0][best_category]);
}

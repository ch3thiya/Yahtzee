#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct{
int player_total;
int computer_total;
} TotalScores;

bool two = false, three = false, four = false;
bool largeStraight = false, smallStraight = false, yahtzee = false;
bool valid = false;
bool hasValidPattern = false;
int values[5] = {};
int rep_array[2][6] = {{1, 2, 3, 4, 5, 6}, {0, 0, 0, 0, 0, 0}};
int available_category[3][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}, {0,0,0,0,0,0,0,0,0,0,0,0,0}};
int computer_points[2][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}};
int point_array[3][13] = {{1,2,3,4,5,6,7,8,9,10,11,12,13}, {0,0,0,0,0,0,0,0,25,30,40,0,50}, {0,0,0,0,0,0,0,0,0,0,0,0,0}};

void rollDice(); // Rolls dice to generate random values
void printDice(); // Displays current dice values
void rerollDice(); // Allows the player to reroll selected dice
void countScore(int category, int *onescore); // Calculates score for a selected category
void updateRepArray(int rep_array[2][6]); // Updates array to count dice repititions
void getPatterns(int rep_array[2][6]); // Identifies patterns like pairs or straights in dice rolls
void checkValid(int point_array[3][13], int category); // Checks if a selected category is valid for scoring
void givePoints(int point_array[3][13], int category, int *onescore); // Adds points to player’s score
int getSum(int values[]); // Returns the sum of all dice values
void calcPoints(int available_category[3][13], int available_points); // Calculates points available for each category
void compPoints(int available_category[3][13], int computer_points[2][13]); // Function to determine the best category for the computer to score in
void compReroll(int values[], int rep_array[2][6], int available_category[3][13]); // Computer’s reroll strategy
void displayTable(int point_array[3][13], int computer_points[2][13], int available_category[3][13], int round); // Displays scoring table
void checkWinner(TotalScores totals); // Determines the winner
TotalScores getFullPoints(int point_array[3][13], int computer_points[2][13], int available_category[3][13]); // Calculates totals

int main(void)
{
    int repeats = 0; // Number of rerolls
    char rollagain; // Player input for reroll choice
    int onescore = 0; // Temporary score storage
    int category = 0; // Chosen category
    int available_points = 0; // Available points for computer
    int round = 0; // Current round number

    srand(time(0)); // Seed random number generator

    printf("\n| Y |  A |  H |  T |  Z |  E |  E |\n\n");

    while (round < 13) // Loop through 13 rounds
    {
        printf("# Round : %d\n", round+1);
        printf("# Your Turn\n\n");
        printf("Rolling dice...\n");
        rollDice(values);
        printf("Your dice values: ");   
        printDice(values);

        onescore = 0;
        available_points = 0;

        // Allow rerolls up to 2 times
        while (repeats < 2)
        {
            printf("Do you want to reroll? (y/n) ");
            scanf(" %c", &rollagain);

            if (rollagain == 'n')
            {
                break; // Don't reroll if player declines
            }
            else if (rollagain == 'y')
            {
                rerollDice(values); // Perform reroll
                repeats++;
            }
            else
            {
                printf("Invalid Input! Please enter 'y' or 'n'.\n");
            }
        }

        // Select category for scoring
        do {
            printf("1. Ones\n2. Twos\n3. Threes\n4. Fours\n5. Fives\n6. Sixes\n");
            printf("7. Three of a kind\n8. Four of a kind\n9. Full House\n10. Small Straight\n11. Large Straight\n12. Chance\n13. Yahtzee\n");
            printf("Give a category (1 to 13): ");
            scanf("%d", &category);

            if (category < 1 || category > 13)
            {
                printf("Invalid category! Please enter a number between 1 and 13.\n\n");
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
                    countScore(category, &onescore);
                } else
                {
                    updateRepArray(rep_array);
                    getPatterns(rep_array);
                }
                checkValid(point_array, category);
                givePoints(point_array, category, &onescore);
                point_array[2][category - 1] = 1; 
                break;
            }

        } while (1);
        repeats = 0;

        printf("-----------------------------------------------\n");
        printf("# Computer's Turn\n\n");
        rollDice(values);
        printf("Computer's dice values: ");
        printDice(values);

        two = three = four = smallStraight = largeStraight = yahtzee = false;

        calcPoints(available_category, available_points);

        compReroll(values, rep_array, available_category);
        compPoints(available_category, computer_points);

        displayTable(point_array, computer_points, available_category, round);
        printf("\n\n");

        round++; // Move to next round
    }    

    return 0;
}

// Rolls five dice to generate random values
void rollDice(int values[])
{
    for (int i = 0; i < 5; i++)
    {
        values[i] = (rand() % 6) + 1;
    }
}

// Prints dice values
void printDice(int values[])
{
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", values[i]);
    }
    printf("\n");
}

// Allows player to reroll selected dice
void rerollDice(int values[])
{
    int numRerolls;
    printf("How many dice do you want to reroll (1 to 5)? ");
    scanf("%d", &numRerolls);

    if (numRerolls < 1 || numRerolls > 5)
    {
        printf("Invalid number of rerolls. Please enter a number between 1 and 5.\n");
        return;
    }

    printf("Enter the dice indexes you want to reroll (1 to 5):\n");
    for (int i = 0; i < numRerolls; i++)
    {
        int rerollIndex;
        scanf("%d", &rerollIndex);

        if (rerollIndex < 1 || rerollIndex > 5)
        {
            printf("Invalid index %d. Please enter a number between 1 and 5.\n", rerollIndex);
            i--;
            continue;
        }

        values[rerollIndex - 1] = (rand() % 6) + 1;
    }

    printf("Rerolled dice values: ");
    printDice(values);
}

// Function to calculate the score for "Ones" through "Sixes" categories
void countScore(int category, int *onescore)
{
    int count = 0;
    // Count how many dice match the selected category (1-6)
    for (int i = 0; i < 5; i++)
    {
        if (values[i] == category)
        {
            count++;
        }
    }
    // Calculate score as the sum of matched dice values
    *onescore = category * count;
}

// Function to update the rep_array with repitition count of each die value (1-6)
void updateRepArray(int rep_array[2][6])
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

// Function to determine dice patterns (e.g., pairs, three-of-a-kind, straights) based on rep_array
void getPatterns(int rep_array[2][6])
{
    two = three = four = smallStraight = largeStraight = yahtzee = false;

    for (int i = 0; i < 6; i++)
    {
        if (rep_array[1][i] == 2) two = true;
        if (rep_array[1][i] == 3) three = true;
        if (rep_array[1][i] == 4) four = true;
        if (rep_array[1][i] == 5) yahtzee = true;
    }

    // Check for small and large straights
    for (int i = 0; i <= 2; i++)
    { 
        if (rep_array[1][i] && rep_array[1][i + 1] && rep_array[1][i + 2] && rep_array[1][i + 3])
        {
            smallStraight = true; 
            if (i <= 1 && rep_array[1][i + 4])
            {
                largeStraight = true;
            }
        }
    }
}

// Function to validate if the chosen category
void checkValid(int point_array[3][13], int category)
{
    valid = false;
    switch (category) {
        case 7: 
            if (three) valid = true;
            break;
        case 8: 
            if (four) valid = true;
            break;
        case 9:
            if (two && three) valid = true;
            break;
        case 10:
            if (smallStraight) valid = true;
            break;
        case 11:
            if (largeStraight) valid = true;
            break;
        case 12: 
            valid = true;
            break;
        case 13:
            if (yahtzee) valid = true;
            break;
        default:
            if (category >= 1 && category <= 6) valid = true;
            break;
    }
}

// Function to assign points for the chosen category if it is valid
void givePoints(int point_array[3][13], int category, int *onescore)
{
    if (valid)
    {
        if (category == 9 || category == 10 || category == 11 || category == 13)
        {
            point_array[2][category-1] = 1; // Mark category as used
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
        printf("Scored %d in category %d\n\n", point_array[1][category-1],category);
    }
    // If the category is not valid, award 0 points
    else
    {
        printf("The combination is not valid! You get 0 points.\n");
        point_array[1][category-1] = 0;
        point_array[2][category-1] = 1;
    }
}

// Function to calculate the sum of the dice values.
int getSum(int values[])
{
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += values[i];
    }

    return sum;
}

// Function to calculate points for the "Ones" to "Sixes" categories.
void calcPoints(int available_category[3][13], int available_points)
{
    for (int i = 1; i <= 6; i++)
    {
    int available_points = 0;
    // Loop through each dice value and add to available_points if matching.
    for (int j = 0; j < 5; j++)
    {
        if (values[j] == i)
        {
            available_points += i;
        }
    }
    // Store the calculated points in the available_category array.
    available_category[1][i-1] = available_points;

    // Mark the category as available
    if (available_category[2][i-1] == 0)
    { 
        available_category[2][i-1] = 1;
    }  
}}

// Function to simulate computer's reroll
void compReroll(int values[], int rep_array[2][6], int available_category[3][13])
{
    int rerolls = 0;
    bool hasValidPattern = false;

    // Update pattern occurrences and check if there's a valid pattern initially
    updateRepArray(rep_array);
    getPatterns(rep_array);
    hasValidPattern = three || four || (two && three) || smallStraight || largeStraight || yahtzee;

    // Reroll if no valid pattern and rerolls are allowed
    while (!hasValidPattern && rerolls < 2)
    {
        printf("Rerolling dice...\n");

        for (int i = 0; i < 5; i++)
        {
            int diceValue = values[i];
            bool keep = false;

            // Decide which dice to keep based on pattern matches
            if (four || yahtzee)
            {
                keep = true;
            }
            else if (three)
            {
                keep = (rep_array[1][diceValue - 1] >= 3);
            }
            else if (two)
            {
                keep = (rep_array[1][diceValue - 1] >= 2);
            }
            else if
            (smallStraight || largeStraight)
            {
                keep = true;
            }

            // Reroll dice not matching a required pattern
            if (!keep)
            {
                values[i] = (rand() % 6) + 1;
            }
        }

        printf("Rerolled values: ");
        printDice(values);

        // Update patterns after reroll
        updateRepArray(rep_array);
        getPatterns(rep_array);
        hasValidPattern = three || four || (two && three) || smallStraight || largeStraight || yahtzee;

        rerolls++;
    }

    // Update available categories based on pattern found
    available_category[1][6] = available_category[1][7] = available_category[1][11] = getSum(values);
    if (three && available_category[2][6] == 0) 
        available_category[2][6] = 1;            
    if (four && available_category[2][7] == 0) 
        available_category[2][7] = 1;            
    if (two && three && available_category[2][8] == 0) 
        available_category[2][8] = 1;  
    if (smallStraight && available_category[2][9] == 0) 
        available_category[2][9] = 1;   
    if (largeStraight && available_category[2][10] == 0) 
        available_category[2][10] = 1; 
    if (yahtzee && available_category[2][12] == 0) 
        available_category[2][12] = 1;   
}

// Function to determine the best category for the computer to score in.
void compPoints(int available_category[3][13], int computer_points[2][13])
{
    int max_points = 0;
    int best_category = -1;

    // Check for highest-scoring patterns and select the best category.
    if (available_category[2][12] == 1)
    {
        best_category = 12;
        max_points = 50;
    }
    else if (available_category[2][10] == 1)
    { 
        best_category = 10;
        max_points = 40; 
    }
    else if (available_category[2][9] == 1)
    { 
        best_category = 9;
        max_points = 30; 
    }
    else if (available_category[2][8] == 1)
    { 
        best_category = 8;
        max_points = 25; 
    }
    else if (available_category[2][6] == 1)
    { 
        best_category = 6;
        max_points = available_category[1][6];
    }
    else if (available_category[2][7] == 1)
    { 
        best_category = 7;
        max_points = available_category[1][7];
    }

    // If no pattern is found, select the category with the maximum points.
    if (best_category == -1)
    {
        for (int i = 0; i < 6; i++)
        {
            if (available_category[2][i] == 1 && available_category[1][i] > max_points)
            {
                max_points = available_category[1][i];
                best_category = i;
            }
        }
    }

    // If no best category was found, select a random available category.
    if (best_category == -1)
    {
        int random_category;
        do {
            random_category = rand() % 13;
        } while (available_category[2][random_category] != 0);
        best_category = random_category;
        max_points = 0;  
    }

    // Assign the points to the selected category and mark it as used.
    computer_points[1][best_category] = max_points;
    available_category[2][best_category] = 2;
    available_category[1][best_category] = 0; 

    printf("Scored %d in category %d\n", max_points, available_category[0][best_category]);
}

// Function to get the full score for both player and computer.
TotalScores getFullPoints(int point_array[3][13], int computer_points[2][13], int available_category[3][13])
{
    TotalScores totals = {0, 0};  
    int player_upper_score = 0;
    int computer_upper_score = 0;

    for (int i = 0; i < 13; i++)
    {
        // If category is used, add points, else 0

        int player_point = point_array[2][i] == 1 ? point_array[1][i] : 0;
        totals.player_total += player_point;

        int computer_point = available_category[2][i] == 2 ? computer_points[1][i] : 0;
        totals.computer_total += computer_point;

        // Track upper section (first six categories) points
        if (i < 6)
        {
            player_upper_score += player_point;
            computer_upper_score += computer_point;
        }
    }

    // Add bonus if upper section score exceeds 63 for player or computer
    if (player_upper_score > 63)
    {
        totals.player_total += 35;
        printf("Player received a bonus of 35 points for the upper section!\n");
    }
    if (computer_upper_score > 63)
    {
        totals.computer_total += 35;
        printf("Computer received a bonus of 35 points for the upper section!\n");
    }

    return totals;
}

// Displays scoring table
void displayTable(int point_array[3][13], int computer_points[2][13], int available_category[3][13], int round)
{
    const char *categories[] =
    {"Ones", "Twos", "Threes", "Fours", "Fives", "Sixes", "Three of a Kind", "Four of a Kind", "Full House", "Small Straight", "Large Straight", "Chance", "YAHTZEE"};
    
    printf("\n-----------------------------------------------\n");
    printf("| %-15s | %-10s | %-10s |\n", "Category", "Player", "Computer");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < 13; i++)
    {
        // If category is used, show points, else 0
        int player_point = point_array[2][i] == 1 ? point_array[1][i] : 0;
        int computer_point = available_category[2][i] == 2 ? computer_points[1][i] : 0;
        printf("| %-15s | %-10d | %-10d |\n", categories[i], player_point, computer_point);
    }

    printf("-----------------------------------------------\n");
    
    TotalScores totals = getFullPoints(point_array, computer_points, available_category);
    printf("| %-15s | %-10d | %-10d |\n", "Total Score", totals.player_total, totals.computer_total);
    printf("-----------------------------------------------\n");

    if (round == 12)
    {
        checkWinner(totals);
    }
}

// Determines the winner
void checkWinner(TotalScores totals)
{
    printf("\nFinal Scores:\nPlayer: %d\nComputer: %d\n", totals.player_total, totals.computer_total);
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
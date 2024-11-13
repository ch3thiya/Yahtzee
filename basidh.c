#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>  // Needed to get random number generation

#define NUM_DICE 5

int dice[5];
int option;
int combination;
int yes;
int count[6] = {0};
int humanscore[13] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int computerscore[13] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int max2, max1, sum, maxCount1, maxCount2;

void rollDice();
void combinationsAll();
void sumOFKind(int categ);
void nOFkind(int n, int place);
void fullHouse();
void smallStraight();
void largeStraight();
void Yahtzee();
void Chance();
void humanPart();
void computerPart();
int checkYahtzee();
void rollDicePC();
void countDice();
void compReroll();
void analyzeDice();
int compCategory();  // This returns an integer
void calculateScoreComp(int category);
void printDice();

void rollDice() {
    printf("Rolling Dice...\n");
    for (int i = 0; i < 5; i++) {
        dice[i] = rand() % 6 + 1;
        printf("%d ", dice[i]);
    }
    printf("\n");
}

void humanreRoll() {
    for (int i = 0; i < 2; i++) {  // Allow two re-roll attempts
        printf("Do you want to reroll? (Yes=1 / No=0): ");
        scanf("%d", &option);

        if (option == 1) {
            for (int j = 0; j < 5; j++) {  // Ask for each dice if the user wants to reroll it
                printf("Do you want to reroll Dice %d? (Yes=1 / No=0): ", j + 1);
                scanf("%d", &yes);
                if (yes == 1) {
                    dice[j] = rand() % 6 + 1;  // Re-roll the selected dice
                }
            }
            printf("New Dice Values: ");
            for (int j = 0; j < 5; j++) {
                printf("%d ", dice[j]);
            }
            printf("\n");
        } else if (option == 0) {
            break;  // Exit if the user doesn't want to reroll
        } else {
            printf("Invalid input, try again.\n");
        }
    }
    combinationsAll();
}

void combinationsAll() { // Printing the combinations and asking to select the combination

    printf("1. Ones\n2. Twos\n3. Threes\n4. Fours\n5. Fives\n6. Sixes\n");
    printf("7. Three of a kind\n8. Four of a kind\n9. Full House\n10. Small Straight\n11. Large Straight\n12. Yahtzee\n13. Chance\n");

    printf("Choose a combination:\n");
    scanf("%d", &combination);
    
    // Prevent updating a score if it's already set
    if (humanscore[combination - 1] != -1) {
        printf("Score for this combination is already set. Choose another one.\n");
        combinationsAll();
    }
    
    switch (combination) { // Assigning the marks for each combination
        case 1:
            printf("You chose Ones\n");
            sumOFKind(1);
            break;
        case 2:
            printf("You chose Twos\n");
            sumOFKind(2);
            break;
        case 3:
            printf("You chose Threes\n");
            sumOFKind(3);
            break;
        case 4:
            printf("You chose Fours\n");
            sumOFKind(4);
            break;
        case 5:
            printf("You chose Fives\n");
            sumOFKind(5);
            break;
        case 6:
            printf("You chose Sixes\n");
            sumOFKind(6);
            break;
        case 7:
            printf("Three of a Kind selected\n");
            nOFkind(3, 6);
            break;
        case 8:
            printf("Four of a Kind selected\n");
            nOFkind(4, 7);
            break;
        case 9:
            printf("Full House selected\n");
            fullHouse();
            break;
        case 10:
            printf("Small Straight selected\n");
            smallStraight();
            break;
        case 11:
            printf("Large Straight selected\n");
            largeStraight();
            break;
        case 12:
            printf("Yahtzee selected\n");
            Yahtzee();
            break;
        case 13:
            printf("Chance selected\n");
            Chance();
            break;
        default:
            printf("Invalid combination, try again.\n");
    }
}

// Function to calculate points 1 to 6
void sumOFKind(int categ) {
    int count = 0, value = 0;
    for (int i = 0; i < 5; i++) {
        if (dice[i] == categ) {
            count++;
        }
    }
    value = count * categ;
    printf("Points Obtained: %d\n", value);
    humanscore[categ - 1] = value;  // Set score for the selected category
}

// Calculate 3 of and 4 of a kind points
void nOFkind(int n, int place) {
    int count[6] = {0};
    int total = 0;
    int correct = 0; 

    // Count occurrences of each dice face
    for (int i = 0; i < 5; i++) {
        count[dice[i] - 1]++;
        total += dice[i];  // Calculate total of all dice
    }

    // Check if any value occurs n or more times
    for (int i = 0; i < 6; i++) {
        if (count[i] >= n) {
            correct = 1;
            break;  
        }
    }

    if (correct == 1) {
        printf("Points obtained: %d\n", total);
        humanscore[place] = total;
    } else {
        printf("Not a valid answer. Points obtained: 0\n");
        humanscore[place] = 0;
    }
}

void fullHouse() {
    int count[6] = {0};
    int setThree = 0;
    int setTwo = 0;

    // Count occurrences of each dice value
    for (int i = 0; i < 5; i++) {
        count[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (count[i] == 3) {
            setThree = 1;  // Three of a kind found
        }
        if (count[i] == 2) {
            setTwo = 1;  // Two of a kind found
        }
    }

    if (setThree && setTwo) {
        printf("Full House! Points obtained: 25\n");
        humanscore[8] = 25;
    } else {
        printf("Not a valid Full House. Points obtained: 0\n");
        humanscore[8] = 0;
    }
}

void smallStraight() {
    int count[6] = {0};

    // Count occurrences of each dice value
    for (int i = 0; i < 5; i++) {
        count[dice[i] - 1]++;
    }
    if ((count[0] == 1 && count[1] == 1 && count[2] == 1 && count[3] == 1) || // Check whether combination 1,2,3,4 available
        (count[1] == 1 && count[2] == 1 && count[3] == 1 && count[4] == 1) || // Check whether combination 2,3,4,5 available
        (count[2] == 1 && count[3] == 1 && count[4] == 1 && count[5] == 1))  // Check whether combination 3,4,5,6 available
    {
        printf("Small Straight! Points obtained: 30\n");
        humanscore[9] = 30;
    }
    else {
        printf("Not a valid Small straight. Points obtained: 0\n");
        humanscore[9] = 0;
    }
}

void largeStraight() {
    int count[6] = {0};

    // Count occurrences of each dice value
    for (int i = 0; i < 5; i++) {
        count[dice[i] - 1]++;
    }
    if ((count[0] == 1 && count[1] == 1 && count[2] == 1 && count[3] == 1 && count[4] == 1) || // Check whether combination 1,2,3,4,5 available
        (count[1] == 1 && count[2] == 1 && count[3] == 1 && count[4] == 1 && count[5] == 1)) {  // Check whether combination 2,3,4,5,6 available
        printf("Large Straight! Points obtained: 40\n");
        humanscore[10] = 40;
    }
    else {
        printf("Not a valid Large Straight. Points obtained: 0\n");
        humanscore[10] = 0;
    }
}

void Yahtzee() {
    int count[6] = {0};

    // Count occurrences of each dice value
    for (int i = 0; i < 5; i++) {
        count[dice[i] - 1]++;
    }

    if ((count[0] == 5 || count[1] == 5 || count[2] == 5 ||  // Checking whether there is any value 5 times
         count[3] == 5 || count[4] == 5 || count[5] == 5))
    {
        printf("Yahtzee! Points obtained: 50\n");
        humanscore[11] = 50;
    }
    else {
        printf("Not a valid Yahtzee. Points obtained 0\n");
        humanscore[11] = 0;
    }
}

void Chance() {
    int total = 0;

    for (int i = 0; i < 5; i++) {
        total += dice[i];  // Calculate total of all dice
    }
    printf("Chance! Points obtained: %d\n", total);
    humanscore[12] = total;
}

void humanPart() {
    rollDice();  // Roll the dice first
    humanreRoll();   // Allow for reroll and other options
}

void computerPart() {
    rollDicePC();
    for (int i = 0; i < 2; i++) {
        countDice();
        compReroll();
        printDice();
    }
    countDice();
    analyzeDice();
    int category = compCategory();
    printf("Computer Chosed: %d\n", category);
    calculateScoreComp(category);
}

void rollDicePC() {
    printf("Computer Rolling the Dice...\n");
    for (int i = 0; i < 5; i++) {
        dice[i] = rand() % 6 + 1;
        printf("%d ", dice[i]);
    }
    printf("\n");

}

// Function to check if a small straight exists
int checkSmallStraightSeq() {
    if (count[0] >= 1 && count[1] >= 1 && count[2] >= 1) return 1;  // 1,2,3
    if (count[1] >= 1 && count[2] >= 1 && count[3] >= 1) return 2;  // 2,3,4
    if (count[2] >= 1 && count[3] >= 1 && count[4] >= 1) return 3;   // 3,4,5
    if (count[3] >= 1 && count[4] >= 1 && count[5] >= 1) return 4;   // 4,5,6
    return 0;
}

int checkSmallStraight() {
    if (count[0] >= 1 && count[1] >= 1 && count[2] >= 1 && count[3] >= 1) return 1;  // 1,2,3,4
    if (count[1] >= 1 && count[2] >= 1 && count[3] >= 1 && count[4] >= 1) return 2;  // 2,3,4,5
    if (count[2] >= 1 && count[3] >= 1 && count[4] >= 1 && count[5] >= 1) return 3;   // 3,4,5,6
    return 0;
}

int checkLargeStraight() {
    return  (count[0] >= 1 && count[1] >= 1 && count[2] >= 1 && count[3] >= 1) ||  // 1,2,3,4,5
            (count[1] >= 1 && count[2] >= 1 && count[3] >= 1 && count[4] >= 1);   // 2,3,4,5,6
}

int checkYahtzee() {
    return ((count[0] == 5 || count[1] == 5 || count[2] == 5 ||  // Checking whether there is any value 5 times
             count[3] == 5 || count[4] == 5 || count[5] == 5));
}

int checkFullHouse() {

    int set1 = 0; int set2 = 0;

    for (int i = 0; i < 6; i++){
        if (count[i] == 2)
            set1 = 1;
        if (count[i] == 3)
            set2 = 1;
    }
    if (set1 && set2) return 1;
    return 0;
}     

// Function to count dice occurrences
void countDice() {

    for (int i = 0; i < 6; i++) count[i] = 0; // Reset count
    for (int i = 0; i < 5; i++) count[dice[i] - 1]++;
    printf("Counting the Array and marking the occurences: ");
    for (int i = 0; i < 6; i++) printf("%d ", count[i]);
    printf("\n");
}

int main() {
    srand(time(0));  // Seed the random number generator
    int compSum=0;
    int humSum=0;
    for (int i = 0; i < 13; i++) {
        computerscore[i] = -1;
        humanscore[i] = -1;
        
    }

    // Loop for 13 rounds (as there are 13 categories in Yahtzee)
    for (int i = 0; i < 13; i++) {
        printf("\nRound %d:\n", i + 1);
        humanPart();      // Human's turn

        system("clear");

        computerPart();   // Computer's turn
        
        printf("Computer Scores: ");
        
        for(int i = 0; i < 13; i++){
            
            printf("%d ", computerscore[i]);
        }
        printf("\n");
        
        
        printf("Human Scores: ");
        for(int i = 0; i < 13; i++){
            printf("%d ", humanscore[i]);   
        }
        printf("\n");
        
    }
    for(int i=0;i<13;i++){
        compSum+=computerscore[i];
        humSum+=humanscore[i];
    }

    printf("Total Marks Obtained by the Player is %d.\n",compSum);
    printf("Total Marks Obtained by the Player is %d.\n",humSum);
    
    if(compSum>humSum){
        printf("The Winners is the Computer");
    }
    else if(humSum>compSum){
        printf("The Winner is the Player");
    }
    else{
        printf("The Match is a tie");
    }
    return 0;
}
    


int compCategory() {

    if (checkYahtzee() && computerscore[11] == -1)
        return 12;

    if (checkLargeStraight() && computerscore[10] == -1)
        return 11;
    
    if (checkSmallStraight() && computerscore[9] == -1)
        return 10;

    if (checkFullHouse() && computerscore[8] == -1)
        return 9;
    
    // Check for Four of a Kind
    for (int i = 0; i < 6; i++) {
        if (count[i] == 4 && computerscore[7] == -1) 
            return 8;
    }
    // Check for Three of a Kind
    for (int i = 0; i < 6; i++) {
        if (count[i] == 3 && computerscore[6] == -1) 
            return 7;
    }
    if (computerscore[max1 - 1] == -1)
        return max1;

    if (computerscore[max2 - 1] == -1)
        return max2;
    
    // sacrificing 1,2,3
    for (int i = 1; i < 4; i++)
        if (computerscore[i - 1] == -1)   
            return i;
    // last option  
    for (int i = 1; i < 14; i++)
        if (computerscore[i - 1] == -1)   
            return i;    
        
    return 0;  // No combination found
}

void compReroll() {

    if (checkYahtzee() && computerscore[11] == -1)
        return;

    if (checkFullHouse() && computerscore[8] == -1)
        return;

    printf("Small straight: %d \n", checkSmallStraight());

    if (checkSmallStraight()){
        if (computerscore[10] != -1)
            return;
        else if (computerscore[9] == -1){
            int skip = 0;
            int type = checkSmallStraight();

            for (int i=0; i < 5; i++) {

                if (count[dice[i] - 1] == 2) {
                        if (skip)
                            dice[i] = rand() % 6 + 1;
                        skip++;
                    }
                if (type == 1)
                    if (dice[i] == 6) 
                        dice[i] = rand() % 6 + 1;
                
                if (type == 3)
                    if (dice[i] == 1) 
                        dice[i] = rand() % 6 + 1;
            }
        }
        return;
    }

    printf("Small straight sequence occured: %d \n", checkSmallStraightSeq());

    if (checkSmallStraightSeq() && (computerscore[9] == -1 ||computerscore[10] == -1) ){
        int skip = 0; 
        int type = checkSmallStraightSeq();

        for (int i=0; i < 5; i++) {

            if (count[dice[i] - 1] == 2) {
                    if (skip)
                        dice[i] = rand() % 6 + 1;;
                    skip++;
                }
            if (type == 1)
                if (dice[i] == 6 || dice[i] == 5) 
                    dice[i] = rand() % 6 + 1;
            
            if (type == 2)
                if (dice[i] == 6) 
                    dice[i] = rand() % 6 + 1;

            if (type == 3)
                if (dice[i] == 1) 
                    dice[i] = rand() % 6 + 1;
            
            if (type == 4)
                if (dice[i] == 1 || dice[i] == 2) 
                    dice[i] = rand() % 6 + 1;
            }
        return;
    }

    analyzeDice();

    // rolling for a full house
    if (computerscore[8] == -1){
        for (int i=0; i < 5; i++)
            if (dice[i] != max1 && dice[i] != max2)
                dice[i] = rand() % 6 + 1;
        return;
    }

    if (max1 > max2 && computerscore[max1 - 1] == -1){
        for (int i=0; i < 5; i++)
            if (dice[i] != max1)
                dice[i] = rand() % 6 + 1;
        return;
    }

    if (max2 > max1 && computerscore[max1 - 1] == -1){
        for (int i=0; i < 5; i++)
            if (dice[i] != max2)
                dice[i] = rand() % 6 + 1;
        return;
    }

    for (int i=6; i > 0; i--)
        if (computerscore[dice[i] - 1] == -1){
            for (int x = 0; x < 5; x++)
                dice[x] = rand() % 6 + 1;
            return;
        }
}

void analyzeDice() {

    max1 = 0; maxCount1 = 0; max2 = 0; maxCount2 = 0; sum = 0;

    // Iterate through the array to find the maximum value
    for (int i = 0; i < 6; i++) { 
        sum += dice[i]; 
        if (count[i] > maxCount1) {
            max1 = i + 1;
            maxCount1 = count[i];          
        } 
    }
    for (int i = 0; i < 6; i++) 
        if (count[i] == maxCount1 && i + 1 != max1) {
            max2 = i + 1;
            maxCount2 = count[i];          
        }  

    
    printf("Mostly occured number 1: %d \n", max1);
    printf("Mostly occured number 2: %d \n", max2);

}

void calculateScoreComp(int category) {
    int valid = 0;
    if (category < 7){
        computerscore[category - 1] = count[category - 1] * category;
    }
    else {
        switch (category) {
            case 7:  // Three of a kind
                for (int i = 0; i < 6; i++) {
                    if (count[i] >= 3) valid = 1;
                }
                if (valid) computerscore[6] = sum;
                else       computerscore[6] = 0;
                break;

            case 8:  // Four of a kind
                for (int i = 0; i < 6; i++) {
                    if (count[i] >= 4) valid = 1;
                }
                if (valid) computerscore[7] = sum;
                else       computerscore[7] = 0;
                break;

            case 9:  // Full house
                if (checkFullHouse()) {
                    computerscore[8] = 25;
                } else computerscore[8] = 0;
                break;

            case 10:  // Small straight
                if (checkSmallStraight()) {
                    computerscore[9] = 30;
                } else computerscore[9] = 0;
                break;

            case 11:  // Large straight
                if (checkLargeStraight()) {
                    computerscore[10] = 40;
                } else computerscore[10] = 0;
                break;

            case 12:  // Yahtzee
                if (checkYahtzee()) {
                    computerscore[11] = 50;
                } else computerscore[11] = 0;
                break;

            case 13:  // Chance
                computerscore[12] = sum;
                break;

            default:
                printf("Invalid category.\n");
                break;
            }
    }
}

void printDice() {
    printf("Current Dice: ");
    for(int i = 0; i < 5; i++){
        printf("%d ", dice[i]);
    }
    printf("\n");
}
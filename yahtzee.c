#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int values[5];
    int reroll;
    int repeats = 0;
    int category = 0;
    char rollagain;
    int count = 0;
    int numRerolls;
    int onescore;

    srand(time(0));

    for (int i = 0; i < 5; i++)
    {
        values[i] = (rand() % 6) + 1;
        printf("%d ", values[i]);
    }
    printf("\n");

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
            printf("How many dice do you want to reroll (1 to 5)? ");
            scanf("%d", &numRerolls);

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
            for (int i = 0; i < 5; i++) {
                printf("%d ", values[i]);
            }
            printf("\n");
            repeats++;
        }
        else
        {
            printf("Invalid Input! Please enter 'y' or 'n'.\n");
        }
    }

    printf("Final dice values: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", values[i]);
    }
    printf("\n");

    printf("Give a category: ");
    scanf("%d", &category);


    if (category > 0 && category < 7)
    {
        for (int i = 0; i < 5; i++)
        {
            if (values[i] == category)
            {
                count++;
            }
        }
        onescore = category * count;
        printf("The score is %d!\n", onescore);
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
            }
        }
    }

    return 0;
}

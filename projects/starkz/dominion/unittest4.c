/*
 * Author: Zachary Stark
 * Function Under Test: kingdomCards()
 *
 * Inputs:
 * int k1: a valid kingdom card identified in dominion.h enum CARD
 * int k2: a valid kingdom card identified in dominion.h enum CARD
 * int k3: a valid kingdom card identified in dominion.h enum CARD
 * int k4: a valid kingdom card identified in dominion.h enum CARD
 * int k5: a valid kingdom card identified in dominion.h enum CARD
 * int k6: a valid kingdom card identified in dominion.h enum CARD
 * int k7: a valid kingdom card identified in dominion.h enum CARD
 * int k8: a valid kingdom card identified in dominion.h enum CARD
 * int k9: a valid kingdom card identified in dominion.h enum CARD
 * int k10: a valid kingdom card identified in dominion.h enum CARD
 *
 * Output:
 * int*: returns array of integers formed from input
 *
 * Description:
 * Converts 10 individual numbers and puts them in order into an array
 * of size 10.
 */

#include "dominion_helpers.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

#define NUMELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define EQ(x, y) (fabs((x)-(y)) < 0.00005 ? (TRUE) : (FALSE))

int main(){  
    // placeholder variables
    int i;
    int* kingdomOutput;
    int numFail = 0;
    int numSuccess = 0;

    // variables for testing
    int kingdoms[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
    int expectedOut, receivedOut, invalidCard, invalidCard2;

    printf("TESTING: kingdomCards():\n\n");

    printf("SCENARIO: all kingdom cards are valid enum CARDS\n");

    kingdomOutput = kingdomCards(adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy);

    for (i = 0; i < NUMELEMS(kingdoms); i++)
    {
        expectedOut = kingdoms[i];
        receivedOut = kingdomOutput[i];
        if EQ(expectedOut, receivedOut)
        {
            printf("SUCCESS: at index %d expected %d, received %d\n", i, expectedOut, receivedOut);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: at index %d expected %d, received %d\n", i, expectedOut, receivedOut);
            numFail += 1;
        }
    }

    printf("\nSCENARIO: some kingdom cards are invalid (not enum CARDS)\n");

    invalidCard = treasure_map + 1;
    invalidCard2 = curse - 1;
    int kingdoms_error[10] = {invalidCard, invalidCard2, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};

    kingdomOutput = kingdomCards(invalidCard, invalidCard2, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy);

    for (i = 0; i < NUMELEMS(kingdoms_error); i++)
    {
        expectedOut = kingdoms_error[i];
        receivedOut = kingdomOutput[i];
        if EQ(expectedOut, receivedOut)
        {
            printf("SUCCESS: at index %d expected %d, received %d\n", i, expectedOut, receivedOut);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: at index %d expected %d, received %d\n", i, expectedOut, receivedOut);
            numFail += 1;
        }
    }
    
    printf("------------------------------------------------------------------------------------\n");
    printf("END RESULTS: total success: %d total failure: %d percent success: %f%%\n", numSuccess, numFail, 100*numSuccess/((float)(numSuccess + numFail)));
    printf("------------------------------------------------------------------------------------\n");

    return 0;
}

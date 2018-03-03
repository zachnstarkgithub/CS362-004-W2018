/*
 * Author: Zachary Stark
 * Function Under Test: fullDeckCount
 *
 * Inputs:
 * int player: number representing which player is of interest
 * int card: the card that is being searched for
 * struct gameState *state: struct with all game related information
 *
 * Output:
 * int: returns number of times card is found in deck, hand and discard pile (combined count)
 *
 * Description:
 * Iterates through deck, hand and discard pile and finds number of instances of the card specified.
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
    // game related variables
    struct gameState G;
    int seed = 1000;
    int numPlayers = 3;
    int kingdoms[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};

    // placeholder variables
    int j, p, countOutput;
    int numFail = 0;
    int numSuccess = 0;

    printf("TESTING: fullDeckCount():\n\n");

    printf("SCENARIO: There's none of card in deck, hand or discard\n");

    for (p = 0; p < numPlayers; p++)
    {
        memset(&G, 23, sizeof(struct gameState)); //clear the game state G

        initializeGame(numPlayers, kingdoms, seed, &G); //initialize a new game

        G.handCount[p] = MAX_HAND;
        G.deckCount[p] = MAX_DECK;
        G.discardCount[p] = MAX_DECK;

        for (j = 0; j < MAX_HAND; j++)
        {
            G.hand[p][j] = copper;
        }
        for (j = 0; j < MAX_DECK; j++)
        {
            G.deck[p][j] = copper;
            G.discard[p][j] = copper;
        }

        countOutput = fullDeckCount(p, silver, &G);

        if EQ(countOutput, 0)
        {
            printf("SUCCESS: expected %d and received %d\n", 0, countOutput);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: expected %d and received %d\n", 0, countOutput);
            numFail += 1;
        }


    }

    printf("\nSCENARIO: there's a moderate amount of card in deck, hand or discard\n");
    for (p = 0; p < numPlayers; p++)
    {
        memset(&G, 23, sizeof(struct gameState)); //clear the game state G

        initializeGame(numPlayers, kingdoms, seed, &G); //initialize a new gam

        G.handCount[p] = MAX_HAND;
        G.deckCount[p] = MAX_DECK;
        G.discardCount[p] = MAX_DECK;

        for (j = 0; j < MAX_HAND; j++)
        {
            G.hand[p][j] = copper;
        }
        for (j = 0; j < MAX_DECK; j++)
        {
            G.deck[p][j] = copper;
            G.discard[p][j] = copper;
        }

        G.hand[p][32] = silver;
        G.hand[p][90] = silver;
        G.deck[p][190] = silver;
        G.deck[p][490] = silver;
        G.discard[p][230] = silver;

        countOutput = fullDeckCount(p, silver, &G);

        if EQ(countOutput, 5)
        {
            printf("SUCCESS: expected %d and received %d\n", 5, countOutput);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: expected %d and received %d\n", 5, countOutput);
            numFail += 1;
        }
    }  

    printf("\nSCENARIO: there's the most of card possible in deck, hand or discard\n");
    for (p = 0; p < numPlayers; p++)
    {
        memset(&G, 23, sizeof(struct gameState)); //clear the game state G

        initializeGame(numPlayers, kingdoms, seed, &G); //initialize a new gam

        G.handCount[p] = MAX_HAND;
        G.deckCount[p] = MAX_DECK;
        G.discardCount[p] = MAX_DECK;

        for (j = 0; j < MAX_HAND; j++)
        {
            G.hand[p][j] = copper;
        }
        for (j = 0; j < MAX_DECK; j++)
        {
            G.deck[p][j] = copper;
            G.discard[p][j] = copper;
        }

        countOutput = fullDeckCount(p, copper, &G);

        if EQ(countOutput, 1500)
        {
            printf("SUCCESS: expected %d and received %d\n", 1500, countOutput);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: expected %d and received %d\n", 1500, countOutput);
            numFail += 1;
        }
    }

    printf("\nSCENARIO: the deck, hand and discard are empty\n");
    for (p = 0; p < numPlayers; p++)
    {
        memset(&G, 23, sizeof(struct gameState)); //clear the game state G

        initializeGame(numPlayers, kingdoms, seed, &G); //initialize a new gam

        G.handCount[p] = 0;
        G.deckCount[p] = 0;
        G.discardCount[p] = 0;

        countOutput = fullDeckCount(p, copper, &G);

        if EQ(countOutput, 0)
        {
            printf("SUCCESS: expected %d and received %d\n", 0, countOutput);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: expected %d and received %d\n", 0, countOutput);
            numFail += 1;
        }
    }

    printf("------------------------------------------------------------------------------------\n");
    printf("END RESULTS: total success: %d total failure: %d percent success: %f%%\n", numSuccess, numFail, 100*numSuccess/((float)(numSuccess + numFail)));
    printf("------------------------------------------------------------------------------------\n");

    return 0;
}

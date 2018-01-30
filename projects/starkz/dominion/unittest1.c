/*
 * Author: Zachary Stark
 * Function Under Test: drawCard
 *
 * Inputs:
 * int player: number representing which player is drawing a card
 * struct gameState *state: struct with all game related information
 *
 * Output:
 * int: returns 0 if all went well, -1 if deck and discard pile are empty
 *
 * Description:
 * Check if deck is empty, if so, shuffle discard pile into deck.
 * Then draw card from deck
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
    int p, r, i, j, k;
    int numFail = 0;
    int numSuccess = 0;

    // variables for testing
    int maxHandCount = MAX_HAND - 1;
    int commonHandCount = 5;
    int emptyHandCount = 0;
    int handCounts[3] = {maxHandCount, commonHandCount, emptyHandCount};

    int maxDeckCount = MAX_DECK;
    int commonDeckCount = 10;
    int emptyDeckCount = 0;
    int nonEmptyDeckCounts[2] = {maxDeckCount, commonDeckCount};

    int maxDiscardCount = MAX_DECK;
    int commonDiscardCount = 10;
    int emptyDiscardCount = 0;
    int nonEmptyDiscardCounts[2] = {maxDiscardCount, commonDiscardCount};
    int discardCounts[3] = {maxDiscardCount, commonDiscardCount, emptyDiscardCount};

    printf("TESTING: drawCard():\n\n");

    printf("SCENARIO: deck is not empty\n");
    for (p = 0; p < numPlayers; p++)
    {
        for (i = 0; i < NUMELEMS(handCounts); i++)
        {
            for (j = 0; j < NUMELEMS(nonEmptyDeckCounts); j++)
            {
                for (k = 0; k < NUMELEMS(discardCounts); k++)
                {
                    memset(&G, 23, sizeof(struct gameState));       // clear the game state G
                    initializeGame(numPlayers, kingdoms, seed, &G);    // initialize a new game
                    G.handCount[p] = handCounts[i];
                    G.deckCount[p] = nonEmptyDeckCounts[j];
                    G.discardCount[p] = discardCounts[k];

                    r = drawCard(p, &G);

                    if EQ(G.handCount[p], handCounts[i] + 1)
                    {
                        printf("SUCCESS: player %d hand count went from %d to %d\n", p, handCounts[i], G.handCount[p]);
                        numSuccess += 1;
                    }
                    else
                    {
                        printf("FAILURE: player %d hand count went from %d to %d\n", p, handCounts[i], G.handCount[p]);
                        numFail += 1;
                    }

                    if EQ(G.deckCount[p], nonEmptyDeckCounts[j] - 1)
                    {
                        printf("SUCCESS: player %d deck count went from %d to %d\n", p, nonEmptyDeckCounts[j], G.deckCount[p]);
                        numSuccess += 1;
                    }
                    else
                    {
                        printf("FAILURE: player %d deck count went from %d to %d\n", p, nonEmptyDeckCounts[j], G.deckCount[p]);
                        numFail += 1;
                    }

                    if EQ(G.discardCount[p], discardCounts[k])
                    {
                        printf("SUCCESS: player %d discard count went from %d to %d\n", p, discardCounts[k], G.discardCount[p]);
                        numSuccess += 1;
                    }
                    else
                    {
                        printf("FAILURE: player %d discard count went from %d to %d\n", p, discardCounts[k], G.discardCount[p]);
                        numFail += 1;
                    }

                    if EQ(r, 0)
                    {
                        printf("SUCCESS: drawCard returned %d and it should be %d\n", r, 0);
                        numSuccess += 1;
                    }
                    else
                    {
                        printf("FAILURE: drawCard returned %d and it should be %d\n", r, 0);
                        numFail += 1;
                    }
                }
            }
        }
    }

    printf("\nSCENARIO: deck is empty but discard pile has cards\n");
    for (p = 0; p < numPlayers; p++)
    {
        for (i = 0; i < NUMELEMS(handCounts); i++)
        {
            for (k = 0; k < NUMELEMS(nonEmptyDiscardCounts); k++)
            {
                memset(&G, 23, sizeof(struct gameState));       // clear the game state G
                initializeGame(numPlayers, kingdoms, seed, &G);    // initialize a new game
                G.handCount[p] = handCounts[i];
                G.deckCount[p] = emptyDeckCount;
                G.discardCount[p] = nonEmptyDiscardCounts[k];

                r = drawCard(p, &G);

                if EQ(G.handCount[p], handCounts[i] + 1)
                {
                    printf("SUCCESS: player %d hand count went from %d to %d\n", p, handCounts[i], G.handCount[p]);
                    numSuccess += 1;
                }
                else
                {
                    printf("FAILURE: player %d hand count went from %d to %d\n", p, handCounts[i], G.handCount[p]);
                    numFail += 1;
                }

                if EQ(G.deckCount[p], nonEmptyDiscardCounts[k] - 1)
                {
                    printf("SUCCESS: player %d deck count went from %d to %d (should be %d)\n", p, emptyDeckCount, G.deckCount[p], nonEmptyDiscardCounts[k] - 1);
                    numSuccess += 1;
                }
                else
                {
                    printf("FAILURE: player %d deck count went from %d to %d (should be %d)\n", p, nonEmptyDeckCounts[j], G.deckCount[p], nonEmptyDiscardCounts[k] - 1);
                    numFail += 1;
                }

                if EQ(G.discardCount[p], 0)
                {
                    printf("SUCCESS: player %d discard count went from %d to %d (should be %d)\n", p, nonEmptyDiscardCounts[k], G.discardCount[p], 0);
                    numSuccess += 1;
                }
                else
                {
                    printf("FAILURE: player %d discard count went from %d to %d (should be %d)\n", p, nonEmptyDiscardCounts[k], G.discardCount[p], 0);
                    numFail += 1;
                }

                if EQ(r, 0)
                {
                    printf("SUCCESS: drawCard returned %d and it should be %d\n", r, 0);
                    numSuccess += 1;
                }
                else
                {
                    printf("FAILURE: drawCard returned %d and it should be %d\n", r, 0);
                    numFail += 1;
                }
            }
        }
    }

    printf("\nSCENARIO: deck and discard pile are empty\n");
    for (p = 0; p < numPlayers; p++)
    {
        for (i = 0; i < NUMELEMS(handCounts); i++)
        {
            memset(&G, 23, sizeof(struct gameState));       // clear the game state G
            initializeGame(numPlayers, kingdoms, seed, &G);    // initialize a new game
            G.handCount[p] = handCounts[i];
            G.deckCount[p] = emptyDeckCount;
            G.discardCount[p] = emptyDiscardCount;

            r = drawCard(p, &G);

            if EQ(G.handCount[p], handCounts[i])
            {
                printf("SUCCESS: player %d hand count went from %d to %d, unchanged\n", p, handCounts[i], G.handCount[p]);
                numSuccess += 1;
            }
            else
            {
                printf("FAILURE: player %d hand count went from %d to %d, shouldn't change\n", p, handCounts[i], G.handCount[p]);
                numFail += 1;
            }

            if EQ(G.deckCount[p], emptyDeckCount)
            {
                printf("SUCCESS: player %d deck count went from %d to %d, stayed at 0\n", p, emptyDeckCount, G.deckCount[p]);
                numSuccess += 1;
            }
            else
            {
                printf("FAILURE: player %d deck count went from %d to %d, should stay at 0\n", p, emptyDeckCount, G.deckCount[p]);
                numFail += 1;
            }

            if EQ(G.discardCount[p], emptyDiscardCount)
            {
                printf("SUCCESS: player %d discard count went from %d to %d, stayed at 0\n", p, emptyDiscardCount, G.discardCount[p]);
                numSuccess += 1;
            }
            else
            {
                printf("FAILURE: player %d discard count went from %d to %d, should stay at 0\n", p, emptyDiscardCount, G.discardCount[p]);
                numFail += 1;
            }

            if EQ(r, -1)
            {
                printf("SUCCESS: drawCard returned %d and it should be %d\n", r, -1);
                numSuccess += 1;
            }
            else
            {
                printf("FAILURE: drawCard returned %d and it should be %d\n", r, -1);
                numFail += 1;
            }
        }
    }

    printf("\nSCENARIO: User draws 3 cards but deck has 2 cards\n");
    for (p = 0; p < numPlayers; p++)
    {
        memset(&G, 23, sizeof(struct gameState));       // clear the game state G
        initializeGame(numPlayers, kingdoms, seed, &G);    // initialize a new game
        G.handCount[p] = 5;
        G.deckCount[p] = 2;
        G.deck[p][0] = cutpurse;
        G.deck[p][1] = embargo;
        G.discardCount[p] = 1;
        G.discard[p][0] = sea_hag;

        r = drawCard(p, &G);

        if EQ(G.hand[p][5], embargo)
        {
            printf("SUCCESS: player %d drew embargo\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d didn't draw embargo. drew %d instead\n", p, G.hand[p][5]);
            numFail += 1;
        }

        if EQ(G.deckCount[p], 1)
        {
            printf("SUCCESS: player %d deck lost one card\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d deck didn't lose a card, instead has %d (should be %d)\n", p, G.deckCount[p], 1);
            numFail += 1;
        }
        
        if EQ(G.discardCount[p], 1)
        {
            printf("SUCCESS: player %d has the same discard pile\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d has a different discard pile of size %d (should be %d) \n", p, G.discardCount[p], 1);
            numFail += 1;
        }

        if EQ(r, 0)
        {
            printf("SUCCESS: drawCard returned %d and it should be %d\n", r, 0);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: drawCard returned %d and it should be %d\n", r, 0);
            numFail += 1;
        }

        r = drawCard(p, &G);
        
        if EQ(G.hand[p][6], cutpurse)
        {
            printf("SUCCESS: player %d drew cutpurse\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d didn't draw cutpurse. drew %d instead\n", p, G.hand[p][6]);
            numFail += 1;
        }

        if EQ(G.deckCount[p], 0)
        {
            printf("SUCCESS: player %d deck lost one card\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d deck didn't lose a card, instead has %d (should be %d) \n", p, G.deckCount[p], 0);
            numFail += 1;
        }
        
        if EQ(G.discardCount[p], 1)
        {
            printf("SUCCESS: player %d has the same discard pile\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d has a different discard pile of size %d (should be %d)\n", p, G.discardCount[p], 1);
            numFail += 1;
        }

        if EQ(r, 0)
        {
            printf("SUCCESS: drawCard returned %d and it should be %d\n", r, 0);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: drawCard returned %d and it should be %d\n", r, 0);
            numFail += 1;
        }

        r = drawCard(p, &G);

        if EQ(G.hand[p][7], sea_hag)
        {
            printf("SUCCESS: player %d drew sea_hag\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d didn't draw sea_hag. drew %d instead\n", p, G.hand[p][7]);
            numFail += 1;
        }

        if EQ(G.deckCount[p], 0)
        {
            printf("SUCCESS: player %d deck lost one card\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d deck didn't lose a card, instead has %d (should be %d)\n", p, G.deckCount[p], 0);
            numFail += 1;
        }
        
        if EQ(G.discardCount[p], 0)
        {
            printf("SUCCESS: player %d has an empty discard pile\n", p);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: player %d has a different discard pile of size %d (should be %d)\n", p, G.discardCount[p], 0);
            numFail += 1;
        }

        if EQ(r, 0)
        {
            printf("SUCCESS: drawCard returned %d and it should be %d\n", r, 0);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: drawCard returned %d and it should be %d\n", r, 0);
            numFail += 1;
        }
    }

    printf("------------------------------------------------------------------------------------\n");
    printf("END RESULTS: total success: %d total failure: %d percent success: %f%%\n", numSuccess, numFail, 100*numSuccess/((float)(numSuccess + numFail)));
    printf("------------------------------------------------------------------------------------\n");

    return 0;
}

/*
 * Author: Zachary Stark
 * Card to be tested: Embargo
 *
 * Description:
 * When embargo is played, the user selects a supply pile to put an embargo
 * token on it. If someone tries buying a card from a pile with embargo
 * tokens, they receive a curse for each token on the pile. The embargo
 * card is trashed after being played.
 */

#include "dominion_helpers.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

#define NUMELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define EQ(x, y) (fabs((x)-(y)) < 0.00005 ? (TRUE) : (FALSE))

int numFail = 0;
int numSuccess = 0;

void oracle(int received, int expected)
{
    if EQ(expected, received)
    {
        printf("SUCCESS: expected %d, received %d\n", expected, received);
        numSuccess += 1;
    }
    else
    {
        printf("FAILURE: expected %d, received %d\n", expected, received);
        numFail += 1;
    }
}

int cardCount(int deck[], int targetCard, int size)
{
    int card;
    int numTimes = 0;
    for (card = 0; card < size; card++)
    {
        if (EQ(deck[card], targetCard))
        {
            numTimes += 1;
        }
    }
    return numTimes; 
}

int curseCount(struct gameState *state, int player)
{
    int deckCurse, handCurse, discardCurse;
    deckCurse = cardCount(state->deck[player], curse, state->deckCount[player]); 
    handCurse = cardCount(state->hand[player], curse, state->handCount[player]); 
    discardCurse = cardCount(state->discard[player], curse, state->discardCount[player]); 
    return deckCurse + handCurse + discardCurse;

}

int main(){
    // game related variables
    struct gameState G;
    int seed = 1000;
    int numPlayers = 3;
    int kingdoms[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};

    // placeholder variables
    int p, r, i, j;
    int handPos;
    int startEmbargoCount, startCurseCount, startCoins;
    // variables for testing

    printf("TESTING: Embargo:\n\n");

    printf("SCENARIO: Have number of embargo tokens on a pile less than or equal to the total number of curses in play:\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int numCurses = G.supplyCount[curse];

    for(i = 0; i < (int)(numCurses/numPlayers) + 1; i++)
    {
        for(p = 0; p < numPlayers; p++)
        {
            // set coins
            for(j = 0; j < 4; j++)
            {
                G.hand[p][j] = copper;
            }
            G.coins = 4;
            G.handCount[p] = 5;

            handPos = G.handCount[p] - 1;
            G.hand[p][handPos] = embargo;

            // capture starting location
            startEmbargoCount = G.embargoTokens[smithy];
            startCoins = G.coins;

            printf("STARTING CONDITIONS:\n"
                    "player is %d\n"
                    "embargo tokens on smithy: %d\n"
                    "coins: %d\n",
                    p, startEmbargoCount, startCoins);

            playCard(handPos, smithy, -1, -1, &G);

            printf("embargo count should increase by 1\n"); 
            oracle(G.embargoTokens[smithy], startEmbargoCount + 1);
            printf("\n");

            printf("coin count should increase by 2\n"); 
            oracle(G.coins, startCoins + 2);
            printf("\n");

            printf("embargo card shouldn't be in playedcards\n");
            oracle(cardCount(G.playedCards, embargo, G.playedCardCount), 0);
            printf("\n");

            printf("embargo card shouldn't be in deck\n");
            oracle(cardCount(G.deck[p], embargo, G.deckCount[p]), 0);
            printf("\n");

            printf("embargo card shouldn't be in hand\n");
            oracle(cardCount(G.hand[p], embargo, G.handCount[p]), 0);
            printf("\n");

            printf("embargo card shouldn't be in discard\n");
            oracle(cardCount(G.discard[p], embargo, G.discardCount[p]), 0);
            printf("\n");

            endTurn(&G);
        }
    }

    printf("SCENARIO: Buy a card with 1, 4, 5 and 20 embargo tokens on it.\n");

    int embargoTokens[3] = {1, 4, 5};
    for(i = 0; i < 2; i++)
    {
        for(p = 0; p < numPlayers; p++)
        {
            G.embargoTokens[smithy] = embargoTokens[p];
            G.coins = 4; //cost of smithy

            // capture starting location
            startEmbargoCount = G.embargoTokens[smithy];
            startCurseCount = curseCount(&G, p);

            printf("STARTING CONDITIONS:\n"
                    "player is %d\n"
                    "embargo tokens on smithy: %d\n"
                    "curse count: %d\n",
                    p, startEmbargoCount, startCurseCount);

            buyCard(smithy, &G); 

            printf("number of curse cards should increase by number of embargo tokens.\n");
            oracle(curseCount(&G, p), startCurseCount + startEmbargoCount);
            printf("\n");

            endTurn(&G);
        }
    }

    p = 0;

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    //maximize number of tokens on a supply pile.
    G.embargoTokens[smithy] = G.supplyCount[curse];
    G.coins = 4; //cost of smithy card

    startEmbargoCount = G.embargoTokens[smithy];
    startCurseCount = curseCount(&G, p);

    printf("STARTING CONDITIONS:\n"
            "player is %d\n"
            "embargo tokens on smithy: %d\n"
            "curse count: %d\n",
            p, startEmbargoCount, startCurseCount);

    buyCard(smithy, &G); 

    printf("number of curse cards should increase by number of embargo tokens.\n");
    oracle(curseCount(&G, p), startCurseCount + startEmbargoCount);
    printf("\n");


    printf("SCENARIO: Attempt to place embargo on supply card not in play:\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    handPos = G.handCount[p] - 1;
    G.hand[p][handPos] = embargo;

    r = playCard(handPos, feast, -1, -1, &G); 

    printf("play card should return error because feast isn't in the kingdom");
    oracle(r, -1);
    printf("\n");

    printf("------------------------------------------------------------------------------------\n");
    printf("END RESULTS: total success: %d total failure: %d percent success: %f%%\n", numSuccess, numFail, 100*numSuccess/((float)(numSuccess + numFail)));
    printf("------------------------------------------------------------------------------------\n");

    return 0;
}

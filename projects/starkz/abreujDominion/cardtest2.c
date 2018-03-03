/*
 * Author: Zachary Stark
 * Card to be tested: Adventurer
 *
 * Description:
 * When Adventurer is played, the user draws cards from their decks until
 * two treasure cards are drawn at which point the drawing stops. If only
 * one or no cards are drawn, then that's all you get. 
 */

#include "dominion_helpers.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

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

int treasureCount(int deck[], int size)
{
    int card;
    int numTimes = 0;
    for (card = 0; card < size; card++)
    {
        if (EQ(deck[card], copper) || EQ(deck[card], silver) || EQ(deck[card], gold))
        {
            numTimes += 1;
        }
    }
    return numTimes; 
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

void setDeckSize(struct gameState *state, int targetSize, int player, int cardToFill)
{
    int i;
    for(i = 0; i < targetSize; i++)
    {
        state->deck[player][i] = cardToFill; 
    }
    for(i = targetSize; i < MAX_DECK; i++)
    {
        state->deck[player][i] = -1;
    }
    state->deckCount[player] = targetSize;
}

void setDiscardSize(struct gameState *state, int targetSize, int player, int cardToFill)
{
    int i;
    for(i = 0; i < targetSize; i++)
    {
        state->discard[player][i] = cardToFill; 
    }
    for(i = targetSize; i < MAX_DECK; i++)
    {
        state->discard[player][i] = -1;
    }
    state->discardCount[player] = targetSize;
}

void setHandSize(struct gameState *state, int targetSize, int player, int cardToFill)
{
    int i;
    for(i = 0; i < targetSize; i++)
    {
        state->hand[player][i] = cardToFill; 
    }
    for(i = targetSize; i < MAX_HAND; i++)
    {
        state->hand[player][i] = -1;
    }
    state->handCount[player] = targetSize;
}

void addDeckNonTreasures(struct gameState *state, int numNotTreasures, int player)
{
    int i;
    for(i = state->deckCount[player]; i < state->deckCount[player] + numNotTreasures; i++)
    {
        state->deck[player][i] = duchy;
    }
    state->deckCount[player] += numNotTreasures;
}

int main(){
    // game related variables
    struct gameState G;
    int seed = 1000;
    int numPlayers = 3;
    int kingdoms[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};

    // placeholder variables
    int p, i, j, k;
    int handPos;
    int startHandTreasCount, startDeckTreasCount, startDiscardTreasCount, numNotTreasure; 
    int startDiscardCount;

    // variables for testing

    printf("TESTING: Adventurer:\n\n");

    printf("SCENARIO: Deck has 2 or more treasure cards:\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int deckCounts[3] = {MAX_DECK, 5, 2};
    int handCounts[3] = {MAX_HAND - 1, 5, 1};
    int discardCounts[3] = {MAX_DECK - 1, 5, 0};
    for(i = 0; i < NUMELEMS(deckCounts); i++)
    {
        for(j = 0; j < NUMELEMS(handCounts); j++)
        {
            for(k = 0; k < NUMELEMS(discardCounts); k++)
            {
                for(p = 0; p < numPlayers; p++)
                {
                    setHandSize(&G, handCounts[j], p, copper); 
                    setDeckSize(&G, deckCounts[i], p, silver); 
                    setDiscardSize(&G, discardCounts[k], p, gold);

                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = adventurer;
                    
                    numNotTreasure = 0;
                    if(deckCounts[i] < MAX_DECK)
                    {
                        numNotTreasure = 2;
                        addDeckNonTreasures(&G, numNotTreasure, p);
                    }

                    // capture starting location
                    startDeckTreasCount = treasureCount(G.deck[p], G.deckCount[p]);
                    startHandTreasCount = treasureCount(G.hand[p], G.handCount[p]);
                    startDiscardTreasCount = treasureCount(G.discard[p], G.discardCount[p]);
                    startDiscardCount = G.discardCount[p];

                    playCard(handPos, -1, -1, -1, &G);

                    printf("STARTING CONDITIONS:\n"
                            "player is %d\n"
                            "hand treasures: %d\n"
                            "deck treasures: %d\n"
                            "discard treasures: %d\n"
                            "number of non treasures in deck: %d\n",
                            p, startHandTreasCount, startDeckTreasCount, startDiscardTreasCount, numNotTreasure);

                    printf("treasure hand count should increase by 2\n"); 
                    oracle(treasureCount(G.hand[p], G.handCount[p]), startHandTreasCount + 2);
                    printf("\n");

                    printf("treasure deck count should decrease by 2\n"); 
                    oracle(treasureCount(G.deck[p], G.deckCount[p]), startDeckTreasCount - 2);
                    printf("\n");

                    printf("treasure discard count should stay the same\n"); 
                    oracle(treasureCount(G.discard[p], G.discardCount[p]), startDiscardTreasCount);
                    printf("\n");

                    printf("discard count should increase by the number of non treasures\n"); 
                    oracle(G.discardCount[p], startDiscardCount + numNotTreasure);
                    printf("\n");

                    printf("adventurer card should be in playedcards\n");
                    oracle(cardCount(G.playedCards, adventurer, G.playedCardCount), 1);
                    printf("\n");

                    endTurn(&G);
                }
            }
        }
    }

    printf("SCENARIO: Deck has fewer than 2 treasure cards but discard pile has at least 2 treasure cards:\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int newDeckCounts[2] = {1, 0};
    int newHandCounts[3] = {MAX_HAND - 2, 5, 1};
    int newDiscardCounts[3] = {MAX_DECK - 1, 5, 3};
    memcpy(deckCounts, newDeckCounts, sizeof(deckCounts));
    memcpy(handCounts, newHandCounts, sizeof(handCounts));
    memcpy(discardCounts, newDiscardCounts, sizeof(discardCounts));
    for(i = 0; i < NUMELEMS(deckCounts); i++)
    {
        for(j = 0; j < NUMELEMS(handCounts); j++)
        {
            for(k = 0; k < NUMELEMS(discardCounts); k++)
            {
                for(p = 0; p < numPlayers; p++)
                {
                    setHandSize(&G, handCounts[j], p, copper); 
                    setDeckSize(&G, deckCounts[i], p, silver); 
                    setDiscardSize(&G, discardCounts[k], p, gold); 

                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = adventurer;

                    numNotTreasure = 2;
                    addDeckNonTreasures(&G, numNotTreasure, p);

                    // capture starting location
                    startDeckTreasCount = treasureCount(G.deck[p], G.deckCount[p]);
                    startHandTreasCount = treasureCount(G.hand[p], G.handCount[p]);
                    startDiscardTreasCount = treasureCount(G.discard[p], G.discardCount[p]);
                    startDiscardCount = G.discardCount[p];

                    playCard(handPos, -1, -1, -1, &G);

                    printf("STARTING CONDITIONS:\n"
                            "player is %d\n"
                            "hand treasures: %d\n"
                            "deck treasures: %d\n"
                            "discard treasures: %d\n"
                            "number of non treasures: %d\n",
                            p, startHandTreasCount, startDeckTreasCount, startDiscardTreasCount, numNotTreasure);

                    printf("treasure hand count should increase by 2\n"); 
                    oracle(treasureCount(G.hand[p], G.handCount[p]), startHandTreasCount + 2);
                    printf("\n");

                    printf("treasure deck count should be the discard treasure count minus (2 - deck treasure count)\n"); 
                    oracle(treasureCount(G.deck[p], G.deckCount[p]), startDiscardTreasCount - (2 - startDeckTreasCount));
                    printf("\n");

                    printf("treasure discard count should be 0\n"); 
                    oracle(treasureCount(G.discard[p], G.discardCount[p]), 0);
                    printf("\n");

                    printf("discard count should be the number of non treasures\n"); 
                    oracle(G.discardCount[p], numNotTreasure);
                    printf("\n");

                    printf("adventurer card should be in playedcards\n");
                    oracle(cardCount(G.playedCards, adventurer, G.playedCardCount), 1);
                    printf("\n");

                    endTurn(&G);
                }
            }
        }
    }
    printf("SCENARIO: Deck and discard pile collectively have less than 2 treasure cards:\n");
    //This section can't be played, it causes an infinite loop with dominion. Adventurer isn't programmed
    //to expect fewer than 2 treasure cards being in the entire deck and discard pile collectively.
    /*
    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int newDeckCounts1[1] = {0};
    int newHandCounts1[3] = {MAX_HAND - 2, 5, 1};
    int newDiscardCounts1[2] = {1, 0};
    memcpy(deckCounts, newDeckCounts1, sizeof(deckCounts));
    memcpy(handCounts, newHandCounts1, sizeof(handCounts));
    memcpy(discardCounts, newDiscardCounts1, sizeof(discardCounts));
    for(i = 0; i < NUMELEMS(deckCounts); i++)
    {
        for(j = 0; j < NUMELEMS(handCounts); j++)
        {
            for(k = 0; k < NUMELEMS(discardCounts); k++)
            {
                for(p = 0; p < numPlayers; p++)
                {
                    setHandSize(&G, handCounts[j], p, copper); 
                    setDeckSize(&G, deckCounts[i], p, silver); 
                    setDiscardSize(&G, discardCounts[k], p, gold); 

                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = adventurer;

                    numNotTreasure = 2;
                    addDeckNonTreasures(&G, numNotTreasure, p);

                    // capture starting location
                    startDeckTreasCount = treasureCount(G.deck[p], G.deckCount[p]);
                    startHandTreasCount = treasureCount(G.hand[p], G.handCount[p]);
                    startDiscardTreasCount = treasureCount(G.discard[p], G.discardCount[p]);
                    startDiscardCount = G.discardCount[p];

                    // can't play the card, it causes an infinite loop
                    playCard(handPos, -1, -1, -1, &G);

                    printf("STARTING CONDITIONS:\n"
                            "player is %d\n"
                            "hand treasures: %d\n"
                            "deck treasures: %d\n"
                            "discard treasures: %d\n"
                            "number of non treasures: %d\n",
                            p, startHandTreasCount, startDeckTreasCount, startDiscardTreasCount, numNotTreasure);

                    printf("treasure hand count should increase by the number of treasures in discard and deck\n"); 
                    oracle(treasureCount(G.hand[p], G.handCount[p]), startHandTreasCount + startDiscardTreasCount);
                    printf("\n");

                    printf("treasure deck count should be 0\n"); 
                    oracle(treasureCount(G.deck[p], G.deckCount[p]), 0);
                    printf("\n");

                    printf("treasure discard count should be 0\n"); 
                    oracle(treasureCount(G.discard[p], G.discardCount[p]), 0);
                    printf("\n");

                    printf("discard count should be the number of non treasures\n"); 
                    oracle(G.discardCount[p], numNotTreasure);
                    printf("\n");

                    printf("adventurer card should be in playedcards\n");
                    oracle(cardCount(G.playedCards, adventurer, G.playedCardCount), 1);
                    printf("\n");

                    endTurn(&G);
                }
            }
        }
    }*/


    printf("------------------------------------------------------------------------------------\n");
    printf("END RESULTS: total success: %d total failure: %d percent success: %f%%\n", numSuccess, numFail, 100*numSuccess/((float)(numSuccess + numFail)));
    printf("------------------------------------------------------------------------------------\n");

    return 0;
}

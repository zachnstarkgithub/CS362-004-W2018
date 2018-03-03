/*
 * Author: Zachary Stark
 * Card to be tested: Village
 *
 * Description:
 * When village is played, the user draws one card and gains two actions.
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

int main(){
    // game related variables
    struct gameState G;
    int seed = 1000;
    int numPlayers = 3;
    int kingdoms[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};

    // placeholder variables
    int p, i, j, k;
    int handPos;
    int startHandCount, startDeckCount, startDiscardCount, startNumActions; 

    // variables for testing

    printf("TESTING: Village:\n\n");

    printf("SCENARIO: Deck has 1 or more cards:\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int numActions[3] = {1, 2, INT_MAX - 2};
    int deckCounts[3] = {MAX_DECK, 5, 1};
    int handCounts[3] = {MAX_HAND, 5, 1};
    int discardCounts[3] = {MAX_DECK, 5, 0};
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
                    G.numActions = numActions[p];

                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = village;

                    G.deck[p][G.deckCount[p]-1] = duchy;
                    
                    // capture starting location
                    startDeckCount = G.deckCount[p];
                    startHandCount = G.handCount[p];
                    startDiscardCount = G.discardCount[p];
                    startNumActions = G.numActions;

                    playCard(handPos, -1, -1, -1, &G);

                    printf("STARTING CONDITIONS:\n"
                            "player is %d\n"
                            "hand count: %d\n"
                            "deck count: %d\n"
                            "discard count: %d\n"
                            "number of actions: %d\n",
                            p, startHandCount, startDeckCount, startDiscardCount, startNumActions);

                    printf("hand count should stay the same\n"); 
                    oracle(G.handCount[p], startHandCount);
                    printf("\n");

                    printf("deck count should decrease by 1\n"); 
                    oracle(G.deckCount[p], startDeckCount - 1);
                    printf("\n");

                    printf("discard count should stay the same\n"); 
                    oracle(G.discardCount[p], startDiscardCount);
                    printf("\n");

                    printf("there should be a duchy in your hand\n"); 
                    oracle(cardCount(G.hand[p], duchy, G.handCount[p]), 1);
                    printf("\n");

                    printf("there shouldn't be a duchy in your deck\n"); 
                    oracle(cardCount(G.deck[p], duchy, G.deckCount[p]), 0);
                    printf("\n");

                    printf("there shouldn't be a duchy in your discard\n"); 
                    oracle(cardCount(G.discard[p], duchy, G.discardCount[p]), 0);
                    printf("\n");

                    printf("village card should be in playedcards\n");
                    oracle(cardCount(G.playedCards, village, G.playedCardCount), 1);
                    printf("\n");

                    printf("number of actions should increase by 2 and then decrease by 1\n");
                    oracle(G.numActions, startNumActions + 1);
                    printf("\n");

                    endTurn(&G);
                }
            }
        }
    }

    printf("SCENARIO: Deck has no cards but discard pile has at least 1.\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int newDeckCounts[1] = {0};
    int newHandCounts[3] = {MAX_HAND, 5, 1};
    int newDiscardCounts[3] = {MAX_DECK, 5, 1};
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
                    G.numActions = numActions[p];

                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = village;

                    // capture starting location
                    startDeckCount = G.deckCount[p];
                    startHandCount = G.handCount[p];
                    startDiscardCount = G.discardCount[p];
                    startNumActions = G.numActions;

                    playCard(handPos, -1, -1, -1, &G);

                    printf("STARTING CONDITIONS:\n"
                            "player is %d\n"
                            "hand count: %d\n"
                            "deck count: %d\n"
                            "discard count: %d\n"
                            "number of actions: %d\n",
                            p, startHandCount, startDeckCount, startDiscardCount, startNumActions);

                    printf("hand count should stay the same\n"); 
                    oracle(G.handCount[p], startHandCount);
                    printf("\n");

                    printf("deck count should be the discard count decreased by 1\n"); 
                    oracle(G.deckCount[p], startDiscardCount - 1);
                    printf("\n");

                    printf("discard count should be 0\n"); 
                    oracle(G.discardCount[p], 0);
                    printf("\n");

                    printf("village card should be in playedcards\n");
                    oracle(cardCount(G.playedCards, village, G.playedCardCount), 1);
                    printf("\n");

                    printf("number of actions should increase by 2 and then decrease by 1\n");
                    oracle(G.numActions, startNumActions + 1);
                    printf("\n");

                    endTurn(&G);
                }
            }
        }
    }
    printf("SCENARIO: Deck and discard pile are collectively empty:\n");
   
    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int newDeckCounts1[1] = {0};
    int newHandCounts1[3] = {MAX_HAND - 2, 5, 1};
    int newDiscardCounts1[1] = {0};
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
                    G.numActions = numActions[p];

                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = village;

                    // capture starting location
                    startDeckCount = G.deckCount[p];
                    startHandCount = G.handCount[p];
                    startDiscardCount = G.discardCount[p];
                    startNumActions = G.numActions;

                    playCard(handPos, -1, -1, -1, &G);

                    printf("STARTING CONDITIONS:\n"
                            "player is %d\n"
                            "hand count: %d\n"
                            "deck count: %d\n"
                            "discard count: %d\n"
                            "number of actions: %d\n",
                            p, startHandCount, startDeckCount, startDiscardCount, startNumActions);

                    printf("hand count should decrease by 1 from playing village\n"); 
                    oracle(G.handCount[p], startHandCount - 1);
                    printf("\n");

                    printf("deck count should be 0\n"); 
                    oracle(G.deckCount[p], 0);
                    printf("\n");

                    printf("discard count should be 0\n"); 
                    oracle(G.discardCount[p], 0);
                    printf("\n");

                    printf("village card should be in playedcards\n");
                    oracle(cardCount(G.playedCards, village, G.playedCardCount), 1);
                    printf("\n");

                    printf("number of actions should increase by 2 and then decrease by 1\n");
                    oracle(G.numActions, startNumActions + 1);
                    printf("\n");

                    endTurn(&G);
                }
            }
        }
    }


    printf("------------------------------------------------------------------------------------\n");
    printf("END RESULTS: total success: %d total failure: %d percent success: %f%%\n", numSuccess, numFail, 100*numSuccess/((float)(numSuccess + numFail)));
    printf("------------------------------------------------------------------------------------\n");

    return 0;
}

/*
 * Author: Zachary Stark
 * Card to be tested: Smithy
 *
 * Description:
 * When Smithy is played, the user draws 3 cards
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
    if(state->deckCount[player] < targetSize)
    {
        for(i = state->deckCount[player]; i < targetSize; i++)
        {
            state->deck[player][i] = cardToFill; 
        }
    }
    else if(state->deckCount[player] > targetSize)
    {
        for(i = targetSize; i < state->deckCount[player]; i++)
        {
            state->deck[player][i] = -1;
        }
    }
    state->deckCount[player] = targetSize;
}

void setDiscardSize(struct gameState *state, int targetSize, int player, int cardToFill)
{
    int i;
    if(state->discardCount[player] < targetSize)
    {
        for(i = state->discardCount[player]; i < targetSize; i++)
        {
            state->discard[player][i] = cardToFill; 
        }
    }
    else if(state->discardCount[player] > targetSize)
    {
        for(i = targetSize; i < state->discardCount[player]; i++)
        {
            state->discard[player][i] = -1;
        }
    }
    state->discardCount[player] = targetSize;
}

void setHandSize(struct gameState *state, int targetSize, int player, int cardToFill)
{
    int i;
    if(state->handCount[player] < targetSize)
    {
        for(i = state->handCount[player]; i < targetSize; i++)
        {
            state->hand[player][i] = cardToFill; 
        }
    }
    else if(state->handCount[player] > targetSize)
    {
        for(i = targetSize; i < state->handCount[player]; i++)
        {
            state->hand[player][i] = -1;
        }
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
    int p, r, i, j, k;
    int handPos;
    int startHandCount, startDeckCount, startDiscardCount; 
    int startDeckSmithies, startDiscardSmithies, startHandSmithies;

    // variables for testing

    printf("TESTING: Smithy:\n\n");

    printf("SCENARIO: Deck has 3 or more cards:\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int deckCounts[3] = {MAX_DECK, 5, 3};
    int handCounts[3] = {MAX_HAND - 2, 5, 1};
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
                    setDeckSize(&G, deckCounts[i], p, smithy); 
                    setDiscardSize(&G, discardCounts[k], p, copper); 
                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = smithy;

                    // capture starting location
                    startHandCount = G.handCount[p];
                    startDeckCount = G.deckCount[p];
                    startDiscardCount = G.discardCount[p];
                    startDeckSmithies = cardCount(G.deck[p], smithy, G.deckCount[p]);
                    startDiscardSmithies = cardCount(G.discard[p], smithy, G.discardCount[p]);
                    startHandSmithies = cardCount(G.hand[p], smithy, G.handCount[p]);

                    playCard(handPos, -1, -1, -1, &G);

                    for(r = G.handCount[p] - 3; r < G.handCount[p]; r++)
                    {
                        if(EQ(G.hand[p][r], smithy))
                        {
                            startHandSmithies++;
                            startDeckSmithies--;
                        }
                    }
                    printf("STARTING CONDITIONS:\n"
                            "player is %d\n"
                            "hand count: %d\n"
                            "deck count: %d\n"
                            "discard count: %d\n"
                            "hand smithies: %d\n"
                            "deck smithies: %d\n"
                            "discard smithies: %d\n",
                            p, startHandCount, startDeckCount, startDiscardCount,
                            startHandSmithies, startDeckSmithies, startDiscardSmithies); 

                    // should be 2 more than original hand count. draw 3 cards, discard
                    // the Smithy card.
                    printf("\n");
                    printf("hand count should increase by 2\n"); 
                    oracle(G.handCount[p], startHandCount + 2);
                    printf("\n");

                    // should be 3 less than original deck count. draw 3 cards 
                    printf("deck count should decrease by 3\n"); 
                    oracle(G.deckCount[p], startDeckCount - 3);
                    printf("\n");

                    // should be the same as the original discard count. discard smithy
                    printf("discard count should increase by 1\n"); 
                    oracle(G.discardCount[p], startDiscardCount);
                    printf("\n");

                    // smithy should not be in hand after playing it
                    printf("smithy hand count should decrease by 1\n"); 
                    oracle(cardCount(G.hand[p], smithy, G.handCount[p]), startHandSmithies - 1);
                    printf("\n");

                    // smithy should not be in deck after playing it
                    printf("smithy deck count should stay the same\n"); 
                    oracle(cardCount(G.deck[p], smithy, G.deckCount[p]), startDeckSmithies);
                    printf("\n");

                    // smithy should be in playedcards pile after playing it
                    printf("smithy discard count should increase by 1\n"); 
                    oracle(cardCount(G.playedCards, smithy, G.playedCardCount), 1);
                    printf("\n");

                    endTurn(&G);
                }
            }
        }
    }

    printf("SCENARIO: Deck has fewer than 3 cards but discard pile has at least 3 cards:\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int newDeckCounts[2] = {2, 0};
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
                    setHandSize(&G, handCounts[j], p, smithy); 
                    setDeckSize(&G, deckCounts[i], p, smithy); 
                    setDiscardSize(&G, discardCounts[k], p, copper); 
                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = smithy;

                    // capture starting location
                    startHandCount = G.handCount[p];
                    startDeckCount = G.deckCount[p];
                    startDiscardCount = G.discardCount[p];
                    startDiscardSmithies = cardCount(G.discard[p], smithy, G.discardCount[p]);
                    startHandSmithies = cardCount(G.hand[p], smithy, G.handCount[p]);

                    playCard(handPos, -1, -1, -1, &G);

                    for(r = G.handCount[p] - 3; r < G.handCount[p]; r++)
                    {
                        if(EQ(G.hand[p][r], smithy))
                        {
                            startHandSmithies++;
                        }
                    }
                    printf("STARTING CONDITIONS:\n"
                            "player is %d\n"
                            "hand count: %d\n"
                            "deck count: %d\n"
                            "discard count: %d\n"
                            "hand smithies: %d\n"
                            "discard smithies: %d\n",
                            p, startHandCount, startDeckCount, startDiscardCount,
                            startHandSmithies, startDiscardSmithies); 

                    // should be 2 more than original hand count. draw 3 cards, discard
                    // the Smithy card.
                    printf("\n");
                    printf("hand count should increase by 2\n"); 
                    oracle(G.handCount[p], startHandCount + 2);
                    printf("\n");

                    // should be what's left after drawing 3 cards 
                    printf("deck count should be discard count - (3 - deck count)\n"); 
                    oracle(G.deckCount[p], startDiscardCount - (3 - startDeckCount));
                    printf("\n");

                    // should be 0 because it got taken for the deck
                    printf("discard count should be 0\n"); 
                    oracle(G.discardCount[p], 0);
                    printf("\n");

                    // smithy should not be in hand after playing it
                    printf("smithy hand count should decrease by 1\n"); 
                    oracle(cardCount(G.hand[p], smithy, G.handCount[p]), startHandSmithies - 1);
                    printf("\n");

                    // smithy should be in discard pile after playing it
                    printf("smithy played cards count should be 1 for the one smithy card played\n"); 
                    oracle(cardCount(G.playedCards, smithy, G.playedCardCount), 1);
                    printf("\n");

                    endTurn(&G);
                }
            }
        }
    }
    printf("SCENARIO: Deck and discard pile collectively have less than 3 cards:\n");

    memset(&G, 23, sizeof(struct gameState));
    initializeGame(numPlayers, kingdoms, seed, &G);

    int newDeckCounts1[2] = {1, 0};
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
                    setHandSize(&G, handCounts[j], p, smithy); 
                    setDeckSize(&G, deckCounts[i], p, smithy); 
                    setDiscardSize(&G, discardCounts[k], p, copper); 
                    
                    handPos = G.handCount[p] - 1;
                    G.hand[p][handPos] = smithy;

                    r = playCard(handPos, -1, -1, -1, &G);

                    printf("play card should return an error\n"); 
                    oracle(r, -1);
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

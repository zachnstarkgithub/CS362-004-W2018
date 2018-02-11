/*
 * Author: Zachary Stark
 * Card to be tested: Smithy
 *
 * Description:
 * When Smithy is played, the user draws 3 cards
 */

#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

#define NUMELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define EQ(x,y) (fabs((x)-(y)) < 0.00005 ? (TRUE) : (FALSE))

void printOutcome(int expected, int received)
{
    if EQ(received, expected)
    {
        printf("SUCCESS: received %d, expected %d\n", received, expected);
    }
    else
    {
        printf("FAILURE: received %d, expected %d\n", received, expected);
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
            numTimes ++;
        }
    }
    return numTimes;
}

void oracle(int player, struct gameState *prev, struct gameState *post)
{
    // acquire variables of interest of game before calling smithy
    int prevHandCount = prev->handCount[player];
    int prevDeckCount = prev->deckCount[player];
    int prevDiscardCount = prev->discardCount[player];
    int prevSmithyPlayedCardCount = cardCount(prev->playedCards, smithy, prev->playedCardCount);

    // acquire variables of interest of game after calling smithy
    int postHandCount = post->handCount[player];
    int postDeckCount = post->deckCount[player];
    int postDiscardCount = post->discardCount[player];
    int postSmithyPlayedCardCount = cardCount(post->playedCards, smithy, post->playedCardCount);

    // as defined by smithy
    int numCardsToDraw = 3;

    int prevCardCount, postCardCount;

    // print previous state
    printf("\n\nPREVIOUS STATE:\n"
           "player: %d\n"
           "hand count: %d\n"
           "deck count: %d\n"
           "discard count: %d\n"
           "smithy count in played cards: %d\n\n\n",
           player, prevHandCount, prevDeckCount,
           prevDiscardCount, prevSmithyPlayedCardCount);

    if(prevDeckCount >= numCardsToDraw)
    {
        printf("hand count should increase by cards drawn and decrease "
               "by 1 for the lost smithy card\n");
        printOutcome(prevHandCount + numCardsToDraw - 1, postHandCount);

        printf("deck count should decrease by cards drawn\n");
        printOutcome(prevDeckCount - numCardsToDraw, postDeckCount);

        printf("discard count should be the same\n");
        printOutcome(prevDiscardCount, postDiscardCount);

        printf("played cards should now include a smithy\n");
        printOutcome(prevSmithyPlayedCardCount + 1, postSmithyPlayedCardCount);
        
        printf("top 3 cards of previous deck should be in hand\n");
        prevCardCount = cardCount(prev->hand[player], prev->deck[player][prev->deckCount[player] - 1], prev->handCount[player]);
        postCardCount = cardCount(post->hand[player], prev->deck[player][prev->deckCount[player] - 1], post->handCount[player]);

        printOutcome(prevCardCount + 1, postCardCount);

        prevCardCount = cardCount(prev->hand[player], prev->deck[player][prev->deckCount[player] - 2], prev->handCount[player]);
        postCardCount = cardCount(post->hand[player], prev->deck[player][prev->deckCount[player] - 2], post->handCount[player]);

        printOutcome(prevCardCount + 1, postCardCount);
        
        prevCardCount = cardCount(prev->hand[player], prev->deck[player][prev->deckCount[player] - 3], prev->handCount[player]);
        postCardCount = cardCount(post->hand[player], prev->deck[player][prev->deckCount[player] - 3], post->handCount[player]);

        printOutcome(prevCardCount + 1, postCardCount);
    }
    else if(prevDeckCount < numCardsToDraw && prevDiscardCount > (numCardsToDraw - prevDeckCount))
    {
        printf("hand count should increase by cards drawn and decrease "
               "by 1 for the lost smithy card\n");
        printOutcome(prevHandCount + numCardsToDraw - 1, postHandCount);

        printf("deck count should be"
               "discard count - (num cards drawn - deck count)\n");
        printOutcome(prevDiscardCount - (numCardsToDraw - prevDeckCount), postDeckCount);

        printf("discard count should be 0\n");
        printOutcome(0, postDiscardCount);

        printf("played cards should now include a smithy\n");
        printOutcome(prevSmithyPlayedCardCount + 1, postSmithyPlayedCardCount);
    }
    else
    {
        printf("hand count should increase by cards available and decrease "
               "by 1 for the lost smithy card\n");
        printOutcome(prevHandCount + prevDeckCount + prevDiscardCount - 1, postHandCount);

        printf("deck count should be 0\n");
        printOutcome(0, postDeckCount);

        printf("discard count should be 0\n");
        printOutcome(0, postDiscardCount);

        printf("played cards should now include a smithy\n");
        printOutcome(prevSmithyPlayedCardCount + 1, postSmithyPlayedCardCount);
    }
}

int generateRandomBetween(int min, int max)
{
    return min + floor(Random() * (max - min + 1));
}

int makeRandomGame(struct gameState *game)
{
    int i, p;
    int maxDeck, maxDiscard;
    int minDeck, minDiscard;
    int smithyIndex;
    int randNum;
    
    for (i = 0; i < sizeof(struct gameState); i++)
    {
        ((char*)game)[i] = floor(Random() * 256);
    }
    game->numPlayers = generateRandomBetween(2, MAX_PLAYERS);
    game->whoseTurn = generateRandomBetween(0, game->numPlayers - 1);
    p = game->whoseTurn;

    randNum = floor(Random() * 10);
    if (randNum > 3)
    {
        // deck has at least 3 cards which will mean the hand draws
        // only from the deck.
        maxDeck = MAX_DECK;
        minDeck = 3;
        
        maxDiscard = MAX_DECK;
        minDiscard = 0;
    }
    else if (randNum <= 3 && randNum > 0)
    {
        // deck has less than 3 cards but the discard pile has enough
        // to compensate 
        maxDeck = 2;
        minDeck = 0;
        
        maxDiscard = MAX_DECK;
        minDiscard = 3;
    }
    else 
    {
        // deck has less than 3 cards and so does discard pile. user
        // will draw less than 3 cards potentially
        maxDeck = 2;
        minDeck = 0;

        maxDiscard = 3;
        minDiscard = 0;
    } 

    game->deckCount[p] = generateRandomBetween(minDeck, maxDeck);
    game->discardCount[p] = generateRandomBetween(minDiscard, maxDiscard);

    // the 1 is so there's room for the smithy in the hand and the - 2
    // is so there's room for the 3 cards to be drawn and one smithy card
    // to be removed
    game->handCount[p] = generateRandomBetween(1, MAX_HAND - 2);
    
    // the - 1 is so there's room for the smithy to be put here.
    game->playedCardCount = generateRandomBetween(0, MAX_DECK - 1);

    smithyIndex = generateRandomBetween(0, game->handCount[p] - 1);
    game->hand[p][smithyIndex] = smithy;

    if(game->numActions <= 0)
    {
        game->numActions = generateRandomBetween(1, INT_MAX);
    }

    return smithyIndex;
}

void deepCopyPrevToPost(struct gameState* prev, struct gameState *post)
{
    int i;
    for (i = 0; i < sizeof(struct gameState); i++)
    {
        ((char*)post)[i] = ((char*)prev)[i];
    }
}

int main()
{
    // postG is after smithy, prevG is before smithy 
    struct gameState postG, prevG;
    int smithyIndexInHand, player;

    // timing the while loop variables
    time_t endwait;
    time_t start = time(NULL);
    time_t seconds = 10;  // how long the while loop will last

    // setup random numbers
    SelectStream(2);
    PutSeed(3);

    endwait = start + seconds;

    while(start < endwait)
    { 
        smithyIndexInHand = makeRandomGame(&prevG);
        player = prevG.whoseTurn;

        // this means action phase
        prevG.phase = 0;

        deepCopyPrevToPost(&prevG, &postG);

        playCard(smithyIndexInHand, -1, -1, -1, &postG);

        oracle(player, &prevG, &postG);

        start = time(NULL);
    }

    return 0;
}

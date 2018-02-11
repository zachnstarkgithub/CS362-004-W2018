/*
 * Author: Zachary Stark
 * Card to be tested: Adventurer
 *
 * Description:
 * When Adventurer is played, the user draws cards from the deck until two
 * treasures are found and added to the deck. The cards drawn from the deck
 * are discarded. If the deck runs out of cards, the discard gets shuffled
 * and used as the deck. If only one treasure is found, you only get one
 * treasure.
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
            numTimes++;
        }
    }
    return numTimes;
}

int treasureCount(int deck[], int size)
{
    int card;
    int numTimes = 0;
    for (card = 0; card < size; card++)
    {
        if (EQ(deck[card], copper) || EQ(deck[card], silver) || EQ(deck[card], gold))
        {
            numTimes++;
        }
    }
    return numTimes;
}

void oracle(int player, struct gameState *prev, struct gameState *post)
{
    // acquire variables of interest of game before calling adventurer
    int prevAdventurerPlayedCardCount = cardCount(prev->playedCards, adventurer, prev->playedCardCount);
    int prevTreasHandCount = treasureCount(prev->hand[player], prev->handCount[player]);
    int prevTreasDeckCount = treasureCount(prev->deck[player], prev->deckCount[player]);
    int prevTreasDiscardCount = treasureCount(prev->discard[player], prev->discardCount[player]);
    int prevDiscardCount = prev->discardCount[player];
    int prevDeckCount = prev->deckCount[player];

    // acquire variables of interest of game after calling adventurer
    int postAdventurerPlayedCardCount = cardCount(post->playedCards, adventurer, post->playedCardCount);
    int postTreasHandCount = treasureCount(post->hand[player], post->handCount[player]);
    int postTreasDeckCount = treasureCount(post->deck[player], post->deckCount[player]);
    int postTreasDiscardCount = treasureCount(post->discard[player], post->discardCount[player]);
    int postDiscardCount = post->discardCount[player];
    int postDeckCount = post->deckCount[player];

    // as defined by adventurer
    int numTreasToDraw = 2;

    // print previous state
    printf("\n\nPREVIOUS STATE:\n"
           "player: %d\n"
           "hand treasure: %d\n"
           "deck treasure: %d\n"
           "discard treasure: %d\n"
           "hand count: %d\n"
           "deck count: %d\n"
           "discard count: %d\n"
           "adventurer count in played cards: %d\n\n\n",
           player, prevTreasHandCount, prevTreasDeckCount,
           prevTreasDiscardCount, prev->handCount[player],
           prev->deckCount[player], prevDiscardCount,
           prevAdventurerPlayedCardCount);

    if(prevTreasDeckCount >= numTreasToDraw)
    {
        printf("number of treasures in hand should increase by 2\n");
        printOutcome(prevTreasHandCount + 2, postTreasHandCount);
        
        printf("number of treasures in deck should decrease by 2\n");
        printOutcome(prevTreasDeckCount - 2, postTreasDeckCount);

        printf("number of treasures in discard should stay the same\n");
        printOutcome(prevTreasDiscardCount, postTreasDiscardCount);

        printf("adventurer should be in the played count pile\n");
        printOutcome(prevAdventurerPlayedCardCount + 1, postAdventurerPlayedCardCount);
        
        printf("discard count should increase by all the non treasure cards\n");
        printOutcome(prevDiscardCount + (prevDeckCount - postDeckCount) - 2, postDiscardCount);
    }
    //else if(prevTreasDeckCount < numTreasToDraw && prevTreasDeckCount > (numTreasToDraw - prevTreasDeckCount))
    else
    {
        printf("number of treasures in hand should increase by 2\n");
        printOutcome(prevTreasHandCount + 2, postTreasHandCount);
        
        printf("number of treasures in deck should be discard treasure - (num treas to draw - deck treasure)\n");
        printOutcome(prevTreasDiscardCount - (numTreasToDraw - prevTreasDeckCount), postTreasDeckCount);

        printf("number of treasures in discard should be 0\n");
        printOutcome(0, postTreasDiscardCount);

        printf("adventurer should be in the played count pile\n");
        printOutcome(prevAdventurerPlayedCardCount + 1, postAdventurerPlayedCardCount);

        printf("discard count should increase by all the non treasure cards\n");
        printOutcome(prevDeckCount + (prevDiscardCount - postDeckCount) - 2, postDiscardCount);
    }
    //This scenario was for when treasure count in deck and discard were combined less than 2. Sadly,
    //this causes an infinite loop so it is not tested
    /*else
    {
        printf("number of treasures in hand should increase by what's available\n");
        printOutcome(prevTreasHandCount + prevTreasDeckCount + prevTreasDiscardCount, postTreasHandCount);
        
        printf("number of treasures in deck should be 0\n");
        printOutcome(0, postTreasDeckCount);

        printf("number of treasures in discard should be 0\n");
        printOutcome(0, postTreasDiscardCount);

        printf("adventurer should be in the played count pile\n");
        printOutcome(prevAdventurerPlayedCardCount + 1, postAdventurerPlayedCardCount);
    }*/
}

int generateRandomBetween(int min, int max)
{
    return min + floor(Random() * (max - min + 1));
}

void setDeckTreas(struct gameState *state, int numTreas, int player)
{
    int i;
    int madeOnePass = FALSE;
    if(state->deckCount[player] < numTreas)
    {
        return;
    }
    while(numTreas > 0)
    {
        for(i = 0; i < state->deckCount[player]; i++)
        {
            if(madeOnePass == FALSE)
            {
                if(floor(Random() * state->deckCount[player]) < numTreas)
                {
                    state->deck[player][i] = generateRandomBetween(copper, gold);
                    numTreas--;
                }
                else
                {
                    if(state->deck[player][i] <= gold && state->deck[player][i] >= copper)
                    {
                        state->deck[player][i] = generateRandomBetween(adventurer, INT_MAX);
                    }
                }
            }
            else
            {
                if(state->deck[player][i] > gold || state->deck[player][i] < copper)
                {
                    state->deck[player][i] = generateRandomBetween(copper, gold);
                    numTreas--;
                }

            }

        }
        madeOnePass = TRUE;
    }

}

void setDiscardTreas(struct gameState *state, int numTreas, int player)
{
    int i;
    int madeOnePass = FALSE;
    if(state->discardCount[player] < numTreas)
    {
        return;
    }
    while(numTreas > 0)
    {
        for(i = 0; i < state->discardCount[player]; i++) {
            if(madeOnePass == FALSE)
            {
                if(floor(Random() * state->discardCount[player]) < numTreas)
                {
                    state->discard[player][i] = generateRandomBetween(copper, gold);
                    numTreas--;
                }
                else
                {
                    if(state->discard[player][i] <= gold && state->discard[player][i] >= copper)
                    {
                        state->discard[player][i] = generateRandomBetween(adventurer, INT_MAX);
                    }
                }
            }
            else
            {
                if(state->discard[player][i] > gold || state->discard[player][i] < copper)
                {
                    state->discard[player][i] = generateRandomBetween(copper, gold);
                    numTreas--;
                }

            }

        }
        madeOnePass = TRUE;
    }
}

int makeRandomGame(struct gameState *game)
{
    int i, p;
    int maxDeckTreas, maxDiscardTreas, discardTreas;
    int minDeckTreas, minDiscardTreas, deckTreas;
    int adventurerIndex;
    int randNum;

    for (i = 0; i < sizeof(struct gameState); i++)
    {
        ((char*)game)[i] = floor(Random() * 256);
    }
    game->numPlayers = generateRandomBetween(2, MAX_PLAYERS);
    game->whoseTurn = generateRandomBetween(0, game->numPlayers - 1);
    p = game->whoseTurn;

    randNum = floor(Random() * 10);
    // MAX_DECK / 2 is necessary because adventurer has a temphand
    // that's only MAX_DECK big but if deck and discard combined are 
    // larger than MAX_DECK, then temphand gets overloaded and there's 
    if (randNum > 3)
    {
        // deck has at least 2 treasures which will mean the hand draws
        // only from the deck.
        maxDeckTreas = MAX_DECK/2;
        minDeckTreas = 2;

        maxDiscardTreas = MAX_DECK/2;
        minDiscardTreas = 0;
    }
    // else if (randNum <= 3 && randNum > 0)
    else
    {
        // deck has less than 2 treasures but the discard pile has enough
        // to compensate 
        maxDeckTreas = 1;
        minDeckTreas = 0;

        maxDiscardTreas = MAX_DECK/2;
        minDiscardTreas = 2;
    }
    /*else 
    {
        // deck has less than 2 treasures and so does discard pile. user
        // will draw less than 2 treasures potentially
        // TODO: this is not an option, it will crash adventurer
        maxDeckTreas = 1;
        minDeckTreas = 0;

        maxDiscardTreas = 1;
        minDiscardTreas = 0;
    }*/
    deckTreas = generateRandomBetween(minDeckTreas, maxDeckTreas);
    discardTreas = generateRandomBetween(minDiscardTreas, maxDiscardTreas);

    // MAX_DECK / 2 is necessary because adventurer has a temphand
    // that's only MAX_DECK big but if deck and discard combined are 
    // larger than MAX_DECK, then temphand gets overloaded and there's 
    // a seg fault. 
    game->deckCount[p] = generateRandomBetween(deckTreas, MAX_DECK/2);
    game->discardCount[p] = generateRandomBetween(discardTreas, MAX_DECK/2);

    setDeckTreas(game, deckTreas, p);
    setDiscardTreas(game, discardTreas, p);

    // the 1 is so there's room for the adventurer in the hand and the - 2
    // is so there's room for the 2 treasures to be drawn and one adventurer card
    // to be removed
    game->handCount[p] = generateRandomBetween(1, MAX_HAND - 1);

    // the - 1 is so there's room for the adventurer to be put here.
    game->playedCardCount = generateRandomBetween(0, MAX_DECK - 1);

    adventurerIndex = generateRandomBetween(0, game->handCount[p] - 1);
    game->hand[p][adventurerIndex] = adventurer;

    if(game->numActions <= 0)
    {
        game->numActions = generateRandomBetween(1, INT_MAX);
    }

    return adventurerIndex;
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
    // postG is after adventurer, prevG is before adventurer 
    struct gameState postG, prevG;
    int adventurerIndexInHand, player;

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
        adventurerIndexInHand = makeRandomGame(&prevG);
        player = prevG.whoseTurn;

        // this means action phase
        prevG.phase = 0;

        deepCopyPrevToPost(&prevG, &postG);

        playCard(adventurerIndexInHand, -1, -1, -1, &postG);

        oracle(player, &prevG, &postG);

        start = time(NULL);
    }

    return 0;
}

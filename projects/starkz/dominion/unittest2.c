/*
 * Author: Zachary Stark
 * Function Under Test: drawCost
 *
 * Inputs:
 * int cardNumber: number representing which card cost is needed
 *
 * Output:
 * int: returns cost to buy [number 0-8]
 *
 * Description:
 * Implements a lookup table connecting card names to costs
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
    int i, costOutput;
    int numFail = 0;
    int numSuccess = 0;

    // testing variables
    int cardsToTest[27] = 
    {
        curse,
        estate,
        duchy,
        province,
        copper,
        silver,
        gold,
        adventurer,
        council_room,
        feast,
        gardens,
        mine,
        remodel,
        smithy,
        village,
        baron,
        great_hall,
        minion,
        steward,
        tribute,
        ambassador,
        cutpurse,
        embargo,
        outpost,
        salvager,
        sea_hag,
        treasure_map
    };

    int cardCosts[27] =
    {
        0, //curse
        2, //estate
        5, //duchy
        8, //province
        0, //copper
        3, //silver
        6, //gold
        6, //adventurer
        5, //council_room
        4, //feast
        4, //gardens
        5, //mine
        4, //remodel
        4, //smithy
        3, //village
        4, //baron
        3, //great_hall
        5, //minion
        3, //steward
        5, //tribute
        3, //ambassador
        4, //cutpurse
        2, //embargo
        5, //outpost
        4, //salvager
        4, //sea_hag
        4  //treasure_map
    };

    printf("TESTING: getCost():\n\n");

    printf("SCENARIO: requesting valid card\n");
    for (i = 0; i < NUMELEMS(cardsToTest); i++)
    {
        costOutput = getCost(cardsToTest[i]);
        if EQ(costOutput, cardCosts[i])
        {
            printf("SUCCESS: expected %d and received %d\n", cardCosts[i], costOutput);
            numSuccess += 1;
        }
        else
        {
            printf("FAILURE: expected %d and received %d\n", cardCosts[i], costOutput);
            numFail += 1;
        }

    }

    printf("\nSCENARIO: requesting invalid card\n");
    costOutput = getCost(treasure_map + 1);
    if EQ(costOutput, -1)
    {
        printf("SUCCESS: expected %d and received %d\n", -1, costOutput);
        numSuccess += 1;
    }
    else
    {
        printf("FAILURE: expected %d and received %d\n", -1, costOutput);
        numFail += 1;
    }
    costOutput = getCost(curse - 1);
    if EQ(costOutput, -1)
    {
        printf("SUCCESS: expected %d and received %d\n", -1, costOutput);
        numSuccess += 1;
    }
    else
    {
        printf("FAILURE: expected %d and received %d\n", -1, costOutput);
        numFail += 1;
    }

    printf("------------------------------------------------------------------------------------\n");
    printf("END RESULTS: total success: %d total failure: %d percent success: %f%%\n", numSuccess, numFail, 100*numSuccess/((float)(numSuccess + numFail)));
    printf("------------------------------------------------------------------------------------\n");

    return 0;
}

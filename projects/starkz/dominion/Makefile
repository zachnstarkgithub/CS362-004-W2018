CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

random1: randomtestcard1.c dominion.o rngs.o
	gcc -o random1 randomtestcard1.c -g dominion.o rngs.o $(CFLAGS)
	
	echo "Results for random test generator for Smithy" > randomtestcard1.out
	random1 >> randomtestcard1.out
	gcov -b -c dominion.c >> randomtestcard1.out

random2: randomtestcard2.c dominion.o rngs.o
	gcc -o random2 randomtestcard2.c -g dominion.o rngs.o $(CFLAGS)
	
	echo "Results for random test generator for Village" > randomtestcard2.out
	random2 >> randomtestcard2.out
	gcov -b -c dominion.c >> randomtestcard2.out

random3: randomtestadventurer.c dominion.o rngs.o
	gcc -o random3 randomtestadventurer.c -g dominion.o rngs.o $(CFLAGS)
	
	echo "Results for random test generator for Adventurer" > randomtestadventurer.out
	random3 >> randomtestadventurer.out
	gcov -b -c dominion.c >> randomtestadventurer.out

all: playdom player testDrawCard testBuyCard badTestDrawCard random1 random2 random3

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe random1 badTestDrawCard random2 random3 randomtestcard1.out randomtestcard2.out randomtestadventurer.out

all : mainTest

mainTest :  test_main.o conversion.o toolBox.o calcul.o
	gcc -Wall -pg  -o mainTest test_main.o conversion.o toolBox.o calcul.o -lm


test_main.o : test_main.c conversion.c conversion.h toolBox.c toolBox.h calcul.c calcul.h
	gcc -pg -c test_main.c 

calcul.o : conversion.c conversion.h toolBox.c toolBox.h calcul.c calcul.h
	gcc -pg -c calcul.c
	
conversion.o : toolBox.c toolBox.h conversion.c conversion.h
	gcc -pg -c conversion.c

toolBox.o : toolBox.c toolBox.h 
	gcc -pg -c toolBox.c


	
clean :
	rm -f*.o mainTest
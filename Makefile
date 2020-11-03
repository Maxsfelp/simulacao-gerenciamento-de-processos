all: commander manager clean

commander: commander.o 
	gcc -Wall commander.o -o commander

manager: manager.o
	gcc -Wall manager.o -o manager

commander.o: commander.c
	gcc -Wall -c commander.c

manager.o: manager.c manager.h
	gcc -Wall -c manager.c

clean:
	rm *.o

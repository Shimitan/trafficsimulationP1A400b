simulation.out: main.c statistics.o network.o
	gcc main.c statistics.o network.o -o simulation.out

statistics.o: statistics.c
	gcc -c statistics.c

network.o: network.c
	gcc -c network.c
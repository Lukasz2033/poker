
all: hello

hello: main.o evaluate_hand.o table.o interface.o
	gcc main.c evaluate_hand.c table.c interface.c -o poker -lncurses

main.o: main.c
	gcc -c main.c

evaluate_hand.o: evaluate_hand.c
	gcc -c evaluate_hand.c

table.o: table.c
	gcc -c table.c

interface.o: interface.c
	gcc -c interface.c

clean:
	rm *o poker

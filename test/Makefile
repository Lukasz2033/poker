
all: hello

hello: main.o evaluate_hand.o table.o
	g++ main.c evaluate_hand.c table.c -o poker

main.o: main.c
	g++ -c main.c

evaluate_hand.o: evaluate_hand.c
	g++ -c evaluate_hand.c

table.o: table.c
	g++ -c table.c

clean:
	rm *o hello

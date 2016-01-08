#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 30
#define HEIGHT 10

#define NSUITS 4
#define NFACES 13

#define POKER           7
#define FULLHOUSE       6  
#define FLUSH           5
#define STRAIGHT        4
#define THREE           3  
#define DPAIR           2
#define PAIR            1
#define NONE            0


//char* suits[NSUITS] = {"hearts","spades","clubs","diamonds"};
//char* faces[NFACES] = {"ace","two","three","four","five","six","seven","eight","nine","ten","jack","queen","king"};

typedef unsigned char   u8;
typedef unsigned short  u16;

typedef struct card{
        u8 suit;
        u8 face;
}card;

typedef struct node{ // change name to player

        u8 number;
        u16 credit;
        u8 state;
	u8 stake;
        card hand[2];
        struct node *next;

}node;

typedef struct stats{ // change name to dealer

        u8 plys_in_game;
        u8 state;
        u8 bigblind;
        u8 stake;
        int mony;
        u8 folded;
	u8 cards_on_table;
        card hand[5];
        node *small, *big, *rise;

}stats;

/**
typedef struct game{
	
	node  player[6]; 	
	stats dealer;

}game;
**/

typedef struct retu{

	u8 hand;
	u8 tie_break[5];

}retu;



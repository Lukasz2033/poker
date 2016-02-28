#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define FOLD  1
#define CALL  2
#define RISE  3

#define WIDTH 20
#define HEIGHT 7

#define NSUITS 4
#define NFACES 13

#define INVALID		10
#define POKER           7
#define FULLHOUSE       6  
#define FLUSH           5
#define STRAIGHT        4
#define THREE           3  
#define DPAIR           2
#define PAIR            1
#define NONE            0

#define FLOP		0
#define TURN		1
#define RIVER		2

typedef unsigned char   u8;
typedef unsigned short  u16;


//char* suits[NSUITS] = {"hearts","spades","clubs","diamonds"};
//char* faces[NFACES] = {"ace","two","three","four","five","six","seven","eight","nine","ten","jack","queen","king"};


char *ret_suit(u8 num);
char *ret_face(u8 num);




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
	u8 hand_name;
	WINDOW *win;
        struct node *next;

}node;

typedef struct stats{ // change name to dealer

	node *head; 
	u8 plys_in_game;
        //u8 state;
        u8 bigblind;
        //u8 stake;
        int money_to_play;
        u8 folded;
	u8 cards_on_table;
        card shared_cards[5];
        node *small, *big, *last;

}stats;

typedef struct game_data{
	
	node  player[6]; 	
	stats dealer;

}game_data;

typedef struct retu{

	u8 hand;
	u8 tie_break[5];

}retu;



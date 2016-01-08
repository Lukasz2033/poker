#include "header.h"
#include "table.h"
		//	      0	       1       2          3 	
char* suits[NSUITS] = {"hearts","spades","clubs","diamonds"};
		//	   0	   1	  2	 3     4       5       6      7     8      9     10      11    12
char* faces[NFACES] = {"two","three","four","five","six","seven","eight","nine","ten","jack","queen","king","ace"};

char *ret_suit(u8 num){
	return suits[num]; 
}

char *ret_face(u8 num){
	return faces[num]; 
} 


void init_window(stats *s){
	
	u8 c;
	for(c=0; c<s->plys_in_game; c++){
                wplay[c] = newwin(HEIGHT, WIDTH, (c/4)*2*HEIGHT, (WIDTH*c)%(4*WIDTH));

        }
        ppw = wplay;
        wstats = newwin(10, 100, 8, 0);
        commands = newwin(3, 120, 30, 0);
        debug = newwin(20,100, 35, 0);

}

void delete_window(){

	delwin(wstats);
        delwin(commands);
        delwin(debug);
        endwin();
	
}

void draw_wplys(node *ply, WINDOW **wply, stats *s){
	u8 c;
	node *curr;
	curr = ply;
	for(c=0; c<s->plys_in_game; c++){	
		mvwprintw(wply[curr->number-1], 0, 0, "Player %d\n",curr->number);	
		mvwprintw(wply[curr->number-1], 1, 0, "Cash: %d\n", curr->credit);	
		mvwprintw(wply[curr->number-1], 2, 0, "Stake: %d\n", curr->stake);	
		mvwprintw(wply[curr->number-1], 3, 0, "State: %d\n", curr->state);	
		mvwprintw(wply[curr->number-1], 4, 0, "1st card: %s/%s \n", ret_suit(curr->hand[0].suit), ret_face(curr->hand[0].face));	
		mvwprintw(wply[curr->number-1], 5, 0, "2nd card: %s/%s \n", ret_suit(curr->hand[1].suit), ret_face(curr->hand[1].face));			
		wrefresh(wply[c]);	
		curr = curr->next;
	}	

}

void draw_wstats(stats *s, WINDOW *wstats){
	mvwprintw(wstats, 0, 20, "Ply remained: %d\n",s->plys_in_game);	
	mvwprintw(wstats, 1, 20, "State: %d\n",s->state);	
	mvwprintw(wstats, 2, 20, "Big: %d\n",s->bigblind);	
	mvwprintw(wstats, 3, 20, "Stake: %d\n",s->stake);	
	mvwprintw(wstats, 4, 20, "Total: %d\n",s->mony);	

	
	u8 c;
	for(c=0; c<s->cards_on_table; c++){ 
		
		mvwprintw(wstats, c, 40, "%s/%s\n",ret_suit(s->hand[c].suit), ret_face(s->hand[c].face));				
	}
	wrefresh(wstats);	
	
}



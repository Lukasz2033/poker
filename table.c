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


window win;


void init_window(stats *s){
	
	u8 c;
	for(c=0; c<s->plys_in_game; c++){
                win.wplay[c] = newwin(HEIGHT, WIDTH, (c/4)*2*HEIGHT, (WIDTH*c)%(4*WIDTH));

        }
        win.ppw = win.wplay;
        win.wstats = newwin(10, 100, 8, 0);
        win.commands = newwin(3, 120, 30, 0);
        win.debug = newwin(20,100, 35, 0);

}

void delete_window(){
	
	u8 c;
  	
	for(c=0; c<6; c++){
                delwin(win.wplay[c]);
        }
	
	delwin(win.wstats);
        delwin(win.commands);
        delwin(win.debug);
        endwin();
	
}

void draw_table(node *ply, stats *s){

	u8 c;
	
	node *curr;
	curr = ply;
	
	for(c=0; c<s->plys_in_game; c++){	
		mvwprintw(win.wplay[curr->number-1], 0, 0, "Player %d\n",curr->number);	
		mvwprintw(win.wplay[curr->number-1], 1, 0, "Cash: %d\n", curr->credit);	
		mvwprintw(win.wplay[curr->number-1], 2, 0, "Stake: %d\n", curr->stake);	
		mvwprintw(win.wplay[curr->number-1], 3, 0, "State: %d\n", curr->state);	
		mvwprintw(win.wplay[curr->number-1], 4, 0, "1st card: %s/%s \n", ret_suit(curr->hand[0].suit), ret_face(curr->hand[0].face));	
		mvwprintw(win.wplay[curr->number-1], 5, 0, "2nd card: %s/%s \n", ret_suit(curr->hand[1].suit), ret_face(curr->hand[1].face));			
		wrefresh(win.wplay[c]);	
		curr = curr->next;
	}	
	
	mvwprintw(win.wstats, 0, 20, "Ply remained: %d\n",s->plys_in_game);	
	mvwprintw(win.wstats, 1, 20, "State: %d\n",s->state);	
	mvwprintw(win.wstats, 2, 20, "Big: %d\n",s->bigblind);	
	mvwprintw(win.wstats, 3, 20, "Stake: %d\n",s->stake);	
	mvwprintw(win.wstats, 4, 20, "Total: %d\n",s->mony);	

	
	for(c=0; c<s->cards_on_table; c++){ 
		
		mvwprintw(win.wstats, c, 40, "%s/%s\n",ret_suit(s->hand[c].suit), ret_face(s->hand[c].face));				
	}
	wrefresh(win.wstats);	

}


// Command windows control

int get_player_decision1(u8 number){
	
	int dec;

	mvwprintw(win.commands, 0, 0, "Player %d Chose between FOLD (1), CALL (2) or RAISE (3)\n", number);
        mvwscanw(win.commands, 1, 0, "%d", &dec);
        mvwprintw(win.commands, 1, 0, "");
	
	switch(dec) {
	
		case 1:
        		mvwprintw(win.commands, 0, 0, "Decision: FOLD\n");
		break;
		
		case 2:
        		mvwprintw(win.commands, 0, 0, "Decision: CALL\n");
		break;
		
		case 3:
        		mvwprintw(win.commands, 0, 0, "Decision: RAISE\n");
		break;

	}
        wrefresh(win.commands);

	return dec;
}


int get_player_decision2(u8 number){

        int dec;

        mvwprintw(win.commands, 0, 0, "Player %d How much?\n", number);
        mvwprintw(win.commands, 1, 0, "");
        mvwscanw(win.commands, 1, 0, "%d", &dec);
        mvwprintw(win.commands, 2, 0, "Player raised by  %d\n", dec);
        wrefresh(win.commands);

        return dec;
}

void display_message1(u8 number){

	 mvwprintw(win.commands, 0, 0, "Try again!\n");
	//mvwprintw(win.commands, 0, 0, "Player %d Chose between FOLD (1), CALL (2) or RAISE (3)\n", number);
         mvwprintw(win.commands, 1, 0, ""); 
	 mvwprintw(win.commands, 2, 0, "");
	 wrefresh(win.commands);

}

void display_message2(u8 number){

	 mvwprintw(win.commands, 0, 0, "Incorrect value \n");
 	// mvwprintw(win.commands, 0, 0, "Player %d Chose between FOLD (1), CALL (2) or RAISE (3)\n", number);
         mvwprintw(win.commands, 1, 0, ""); 
	 mvwprintw(win.commands, 2, 0, "");
         wrefresh(win.commands);

}

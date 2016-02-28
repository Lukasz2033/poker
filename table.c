#include "header.h"
#include "table.h"


// Function prototypes
void debug_one_line(char *message);

// Create structure with all windows

void init_window(stats *s){

	initscr();
        echo();
        
	node *player = s->head;

	u8 c;
	for(c=0; c<s->plys_in_game; c++){
                player->win  = newwin(HEIGHT, WIDTH,    (c/3)*3*HEIGHT, (WIDTH*c)%(3*WIDTH));
        	player = player->next;
	}
      
        win.wstats = newwin(6, 60, 10, 0);
        win.commands = newwin(15, 60, 30, 0); 
        win.debug = newwin(50, 60, 0, 70);
	//win.debug = newwin(20,100, 35, 0);
}

void delete_window(stats *s){
	
	u8 c;
  	node *player = s->head;

	for(c=0; c<s->plys_in_game; c++){
                delwin(player->win);
		player = player->next;
        }
	
	delwin(win.wstats);
        delwin(win.commands);
        delwin(win.debug);
        endwin();	
}

void draw_table(stats *s)
{

	u8 c;
	
	node *curr;
	//curr = ply;
	curr = s->head;

	for(c=0; c<s->plys_in_game; c++){	
		mvwprintw(curr->win, 0, 0, "Player %d\n", curr->number);	
		mvwprintw(curr->win, 1, 0, "Cash: %d\n", curr->credit);	
		mvwprintw(curr->win, 2, 0, "Stake: %d\n", curr->stake);	
		
		switch(curr->state){
			case FOLD: mvwprintw(curr->win, 3, 0, "Last decision: FOLD\n");	
			break;
			case CALL: mvwprintw(curr->win, 3, 0, "Last decision: CALL\n");	
			break;
			case RISE: mvwprintw(curr->win, 3, 0, "Last decision: RISE\n");	
			break;
		}
		mvwprintw(curr->win, 4, 0, "1st: %s/%s \n", ret_suit(curr->hand[0].suit), ret_face(curr->hand[0].face));	
		mvwprintw(curr->win, 5, 0, "2nd: %s/%s \n", ret_suit(curr->hand[1].suit), ret_face(curr->hand[1].face));			
		
		char *hand_name ="";
		
		switch(curr->hand_name){
			case 10:hand_name = "????"; break;	
			case 7: hand_name = "Poker"; break;
			case 6: hand_name = "Fullhouse"; break;
			case 5: hand_name = "Flush"; break;
			case 4: hand_name = "Straight"; break;
			case 3: hand_name = "Three"; break;
			case 2: hand_name = "Double Pair"; break;
			case 1: hand_name = "Pair"; break;
			case 0: hand_name = "Nothing"; break;		
		}
		mvwprintw(curr->win, 6, 0, "Hand: %s\n", hand_name);			

		
		wrefresh(curr->win);	
		curr = curr->next;
	}	
	
	mvwprintw(win.wstats, 0, 20, "Players: %d\n", s->folded);	
	mvwprintw(win.wstats, 1, 20, "Big: %d\n", s->bigblind);	
	mvwprintw(win.wstats, 2, 20, "Total: %d\n", s->money_to_play);	
	//mvwprintw(win.wstats, 1, 20, "State: %d\n", s->state);	
	//mvwprintw(win.wstats, 3, 20, "Stake: %d\n", s->stake);	
	
	for(c=0; c<s->cards_on_table; c++){ 
		mvwprintw(win.wstats, c, 40, "%s/%s\n", ret_suit(s->shared_cards[c].suit), ret_face(s->shared_cards[c].face) );
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
	
		case FOLD: mvwprintw(win.commands, 0, 0, "Decision: FOLD\n");
		break;
		case CALL: mvwprintw(win.commands, 0, 0, "Decision: CALL\n");
		break;
		case RISE: mvwprintw(win.commands, 0, 0, "Decision: RISE\n");
		break;

	}
        wrefresh(win.commands);

	return dec;
}


int get_player_decision2(u8 number, char *message){

        int dec;

        mvwprintw(win.commands, 0, 0, "Player %d How much?\n", number);
        mvwprintw(win.commands, 1, 0, "");
        mvwscanw(win.commands, 1, 0, "%d", &dec);
        mvwprintw(win.commands, 2, 0, message, dec);
        wrefresh(win.commands);

        return dec;
}

void display_message(u8 number, char *message){

	 mvwprintw(win.commands, 0, 0, "Player %d Chose between FOLD (1), CALL (2) or RAISE (3)\n", number);
         mvwprintw(win.commands, 1, 0, ""); 
	 mvwprintw(win.commands, 2, 0, message);
	 wrefresh(win.commands);
}

void debug_one_line(char *message){
	
	static u8 line=0, entry=0;
	char str[100];
 
	sprintf(str, "Debug: ");
	mvwprintw(win.debug, 0, 0, str);	
	
	//mvwprintw(win.debug, line, 0, message);
	sprintf(str, "%2d: %s", entry, message);
	mvwprintw(win.debug, line+1, 0, str);	
	line=(++line)%49;
	entry++;
	wrefresh(win.debug);
}

void debug_print_hand(u8 player, u8 hand){

	char *hand_name = "";
	switch(hand){
		case 10: hand_name = "Invalid"; break;	
		case 7: hand_name = "Poker"; break;
		case 6: hand_name = "Fullhouse"; break;
		case 5: hand_name = "Flush"; break;
		case 4: hand_name = "Straight"; break;
		case 3: hand_name = "Three"; break;
		case 2: hand_name = "Double Pair"; break;
		case 1: hand_name = "Pair"; break;
		case 0: hand_name = "Nothing"; break;		
	}
	char str[100];	
        sprintf(str, "player: %d, hand: %s\n", player, hand_name );
        debug_one_line(str); 
	wrefresh(win.debug);	

}
void stop_game(){
	
	int dec;

	mvwprintw(win.commands, 0, 0, "Press something to start next game\n");
        mvwscanw(win.commands, 1, 0, "%d", &dec);
        wrefresh(win.commands);

}


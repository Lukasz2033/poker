#include "header.h"
#include "table.h"

//extern void init_window(stats *s);
// Replace with 
// extern void update_window();
//extern void draw_table(node *ply, stats *s);
//extern void draw_wplys(node *ply, stats *s);
//extern void draw_wstats(stats *s);
//extern int get_player_decision1(u8 number);
//extern int get_player_decision2(u8 number);
//extern void display_message(u8 number, char *message);
//extern void display_message2(u8 number);
//extern void delete_window();

//extern u8 evaluate(card *hand, retu *ret);
//extern u8 compare_res(retu *new, retu *best);

card dec[52];

void *init_players(stats *s);
//node *reload(node *head);
//void show(node *head, stats *s);
//void show(card *hand);
void rm(u8 index, stats *s);
void add(stats *s);
void dealer(node *ply, u8 bid);
void process_player_input(node *ply, stats *s);
//node *game(node *head, stats *s, WINDOW **wplay);
void game(stats *s);
void init_deck(card *dec);




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


void main(){

	stats s;	
	s.plys_in_game = 6;
	s.bigblind = 20;
	s.cards_on_table = 0;
	
	init_players(&s);
	init_window(&s);		
	
	while(1){	
		init_deck(dec);
		s.cards_on_table = 0;
		game(&s);
	}

}

void init_deck(card *dec){
	srand(time(NULL));
	u8 i, j, c=0, r;
	card temp;	
	for(i =0; i<4; i++){
		for(j=0; j<13; j++){
			dec[c].suit = i;
			dec[c].face = j; 
			c++;
		}
	}
	for(i=0; i<52; i++){ //Shuffle
		r = rand()%51;
		temp = dec[r];
		dec[r] = dec[i];
		dec[i] = temp;
	}
}

void *init_players(stats *s){
	node *current;	
	u8 c;
	s->head = (node*)malloc(sizeof(node));
	current = s->head;
	current->number=1;
	current->credit=200;
	current->stake=0;
	current->hand_name=10;
	
	for(c=2; c<(s->plys_in_game)+1; c++){	
		current->next = (node*)malloc(sizeof(node));
		current = current->next;		
		current->number = c;
		current->credit=200;
		current->stake=0;
		current->hand_name=10;
	
	}
	current->next=s->head;
}


void dealer(node *ply, u8 bid){
	ply->credit -= (bid-ply->stake);
	ply->stake = bid;	
}

void process_player_input(node *player, stats *status){
	
	u8 decision;
	u16 rise;
	
	if(player->state != FOLD){

		//decision = get_player_decision1(player->number);
		decision = get_input(win.commands);
		
		u8 number_of_tries = 0;  // number of tries	
		while( decision < 1 || decision > 3 ){
			if(number_of_tries <10){	
				
				display_message(player->number, "Try again!\n");
				//decision = get_player_decision1(player->number);
				decision = get_input(win.commands);
			}	
			else{ // If input was incorrect too many times default to FOLD 	
				decision = FOLD;
				break;
			}	
		}	
		
		if(decision == FOLD){
			player->state = FOLD;
			status->folded--;
		}
		else if(decision == CALL){
			dealer(player, status->money_to_play);
			player->state = CALL;	
		}
		else if(decision == RISE){
			
			rise = get_player_decision2(player->number, "Player raised\n");
			while(rise <= 0){ // Make sure rise is not equal or less than 0
				rise = get_player_decision2(player->number, "Invalid value, must be above 0\n");
			}
			status->money_to_play += rise;
			dealer(player, status->money_to_play);	
			status->last = player; // Update last player to the one that rised!
			player->state = RISE;				
		}
	}
}

void evaluate_hands(stats *status){
	
	u8 hand_index, current_player;
	

	card hand[7];
	retu r, new, best;	
	
	node *player;
	player = status->head;

	char str[100];			
	
	// Collect all cards shared cards into one array
	for(hand_index=0; hand_index<5; hand_index++){
		hand[hand_index] = status->shared_cards[hand_index];
		//sprintf(str, "player %d cards -> %s/%s\n", player->number, 	ret_suit(hand[hand_index].suit),
		//								ret_face(hand[hand_index].face));	
		//debug_one_line(str);	
	}
	
	for(current_player=0; current_player<(status->plys_in_game); current_player++){
		if(player->state != FOLD){
			
			for(hand_index=0; hand_index<5; hand_index++){
				hand[hand_index] = status->shared_cards[hand_index];
				//sprintf(str, "player %d cards -> %s/%s\n", player->number, 	ret_suit(hand[hand_index].suit),
				//								ret_face(hand[hand_index].face));	
				//debug_one_line(str);	
			}				
			// Update players cards
			for(hand_index=0; hand_index<2; hand_index++){
				hand[5+hand_index] = player->hand[hand_index];
				//sprintf(str, "player %d cards -> %s/%s\n", player->number, 	ret_suit(hand[5+hand_index].suit),
				//								ret_face(hand[5+hand_index].face));	
				//debug_one_line(str);	
			}
			player->hand_name = evaluate_test(hand);			
			//debug_print_hand(player->number, new.hand);
		}		
		player = player->next;			
	}	
}

//void prepare_new_game(status 

void showdown(stats *s, u8 *win){
	
	u8 c, begin=0, v, e, i = 0;
	

	card hand[7];
	retu r, new, best;	
	
	node *player;
	player = s->head;

	char str[100];		
	
	for(c=0; c<10; c++){
		win[c] = 222;
	}
	
	while(player->state == FOLD){ // Start with first player that did not fold
		debug_one_line("Some players folded\n");	
		begin++;
		player = player->next;
	}				
	
	// Collect all cards together shared cards and indvidual player cards
	for(c=0; c<5; c++){
		hand[c] = s->shared_cards[c];
		sprintf(str, "player %d cards -> %s/%s\n", player->number, 	ret_suit(hand[c].suit),
										ret_face(hand[c].face));	
		debug_one_line(str);	
	}
	for(c=0; c<2; c++){
		hand[5+c] =  player->hand[c];
		sprintf(str, "player %d cards -> %s/%s\n", player->number, 	ret_suit(hand[5+c].suit),
										ret_face(hand[5+c].face));
		debug_one_line(str);	
	}

	
	sprintf(str, "Begining with player %d\n", player->number );
	debug_one_line(str);	
	
	evaluate(hand, &best);	
	win[i] = player->number;
	
	debug_print_hand(player->number, best.hand);
	
	begin++;
	player = player->next;
		
	for(begin; begin<(s->plys_in_game); begin++){
		if(player->state != FOLD){	
			for(c=0; c<2; c++){
				hand[5+c] = player->hand[c];
			}

			evaluate(hand, &new);			
			debug_print_hand(player->number, new.hand);
			e = compare_res(&new, &best); // Compare two results and returns 0 if one is better and 1 if we have a tie
			
//Uncomment		wprintw(debug,"Decision:%d\n\n",e);	
			
			if(e == 0){ // new higher
				for(v=0; v<10; v++){
					win[c] = 222;
				}		
				i=0;
				win[i] = player->number;
			}
			else if(e == 1){ // a tie
				i++;
				win[i] = player->number;
			}

			player = player->next;	
		}
		else{
			player = player->next;		
		}
	}	
}


void game(stats *status){
	
	u8 player, round, card_for_player, cards_on_table=0, card_from_deck=0, first;
	u16 stake=0; 

	
	node *head = status->head;

	 // Hand out cards 	
	for(player=0; player<status->plys_in_game; player++){ // Go through all players
		for(card_for_player=0; card_for_player<2; card_for_player++){ // Hand out 3 cards (floop)
			head->hand[card_for_player] = dec[card_from_deck++];
		}
		head=head->next; // Go to the next player
	}

	status->money_to_play = 0;  // Minimum bid to stay in the game
	node *curr = head;  // This pointer keeps track of active player in current round ( keep track of big/small blindes )

	status->last = head->next;		// round ends with big blind unless someone rise	
	status->money_to_play = status->bigblind;  	// minimum call equal to big blind

	// Big & Small blinds	
	dealer(head, status->money_to_play); 	// take big blind		
	curr = head->next ;			// begin with small blind
	dealer(curr, status->money_to_play/2);	// take small blind

	//show(curr, s);
	
	status->folded = status->plys_in_game; // Keep track of how many plyers are sitll in the game
	
	round=FLOP;	
	first=1; // variable is used to 
	
	draw_table(status);
	
	// Flop -> Turn -> River 	
	while(round <= RIVER && status->folded > 1){  // Go over Flop, River making sure that more than one ply in the game
				
		while((curr != status->last || first) && status->folded > 1){
			first=0;
			if(curr->state != FOLD)
				process_player_input(curr, status);		
			draw_table(status);	
			curr = curr->next;
		}

		if(status->folded > 1){ // If there are at least 2 players in the game
			// Put cards on table
			if(round == FLOP){  // On flop put three cards on the table
				for(cards_on_table=0; cards_on_table<3; cards_on_table++){
					status->shared_cards[cards_on_table] = dec[card_from_deck++];
					char str[150];
					
					sprintf(str, "[game]Flop: %s, %s", ret_suit(status->shared_cards[cards_on_table].suit),
								 ret_face(status->shared_cards[cards_on_table].face));
					debug_one_line(str);	
					
				}
				status->cards_on_table = 3;
			}
			else if(round == TURN || round == RIVER){ // On turn and river put one card on the table
			
				char *round_n = " ";	
				if(round == TURN) 
					round_n = "Turn";	
				else if(round == RIVER)
					round_n = "River";	
					
				status->shared_cards[status->cards_on_table] = dec[card_from_deck++];
			
				char str[150];	
				sprintf(str, "[game] %s: %s, %s",round_n, ret_suit(status->shared_cards[status->cards_on_table].suit),
							 ret_face(status->shared_cards[status->cards_on_table].face));
				debug_one_line(str);	
				
				status->cards_on_table++;
			}	
			draw_table(status);	
			
			if(round <= RIVER) // dont ask if all cards are on the table
				process_player_input(curr, status);  // Begin with
			
			draw_table(status);	
			curr = curr->next; // Ask player who raised last time here 
					   // cos round finishes when it gets here
			round++;
		}
		
		//char str[15];
		//sprintf(str, "Folded: %d", status->folded);
		//debug_one_line(str);	
	}





// End of game, find who won
	debug_one_line("[game] Showdown\n");	
	
	evaluate_hands(status);
	
	//u8 win[10];	
	//showdown(status, win);
		
	//for(card_for_player=0; card_for_player<2; card_for_player++){
// Uncomment		wprintw(debug,"\nWinners: %d\n", win[card_for_player]);			
// Uncomment		wrefresh(debug);	
	//}
	
	// Collect stakes and check who takes it
	curr = head;

	for(player=0; player<status->plys_in_game; player++){
		stake += curr->stake;	
		curr->state=0;	
		curr->stake=0;
		curr=curr->next;	
	}	

	//u8 wi=0;
	//while(win[wi] != 222){
	//	wi++;
	//}
	//stake /= wi;
	/**
	curr = head;	
	for(player=0; player<status->plys_in_game; player++){
		for(card_for_player=0; card_for_player<10; card_for_player++){
			if(win[card_for_player] !=  222 && win[card_for_player] == curr->number)
				curr->credit += stake;	
		}
		curr = curr->next;
	}
	**/
	debug_one_line("[game] New game\n");	
	draw_table(status);
	
	stop_game();	
	status->head = head->next;	
}














// ADD & REMOVE nodes
void rm(u8 index, stats *s){

	node **head;
	head = &s->head;
	node *prev, *curr;
	prev = *head;
	curr = (**head).next;
	
		
	u8 c;
	for(c=0; c<(s->plys_in_game); c++){
		if(curr->number == index){
			if(*head == curr)
				*head = curr->next;				
			prev->next = curr->next;
			free(curr);
			s->plys_in_game--;
		}
		else{
			prev = curr;
			curr = curr->next;	
		}
	}
	head = 0;
}

void add(stats *s){
	
	node *head = s->head;
	node *prev, *curr, *new;
	prev = head;
	curr = head->next;
	
	u8 c;
	for(c=0; c<(s->plys_in_game); c++){
		if(prev->number < curr->number){
			if(prev->number != curr->number-1){ 	// check for gaps
				new = (node*)malloc(sizeof(node));
				prev->next = new;
				new->next = curr;
				new->number = prev->number+1;
				s->plys_in_game++;				
				break; 	
			}
		}
		else if(prev->number > curr->number){ 	 // check if last one 
			new = (node*)malloc(sizeof(node));
			prev->next = new;
			new->next = curr;
		}
	}
}

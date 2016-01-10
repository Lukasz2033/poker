#include "header.h"
#include "table.h"

#define FOLD  1
#define CALL  2
#define RISE  3

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

node *init(stats *s);
node *reload(node *head);
//void show(node *head, stats *s);
//void show(card *hand);
void rm(node **head, u8 index, stats *s);
void add(node *head, stats *s);
void dealer(node *ply, u8 bid);
void ask_ply(node *ply, stats *s);
//node *game(node *head, stats *s, WINDOW **wplay);
node *game(node *head, stats *s);
void init_dec(card *dec);

int main(){

	u8 c;	
	stats s;	
	s.plys_in_game = 3;
	s.bigblind = 20;
	s.cards_on_table = 0;

	initscr();
	echo();

	init_window(&s);
	
	node *head;	
	head = init(&s);
	
	for(c=0; c<2;c++){	
		init_dec(dec);
		s.cards_on_table = 0;
		//head = game(head, &s, ppw);
		head = game(head, &s);

	}
			
	return 0;
}

void init_dec(card *dec){
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

node *init(stats *s){
	node *head, *current;	
	u8 c;
	head = (node*)malloc(sizeof(node));
	current = head;
	current->number=1;
	current->credit=200;
	current->stake=0;
	for(c=2; c<(s->plys_in_game)+1; c++){	
		current->next = (node*)malloc(sizeof(node));
		current = current->next;		
		current->number = c;
		current->credit=200;
		current->stake=0;	
	}
	current->next=head;
return head;
}


/*
void process_plys(u8 instruction){

	switch()
		case 0:
			#hand cards
		case 1:
			#collect stakes
		case 2:
			#
*/

void dealer(node *ply, u8 bid){
	ply->credit -= (bid-ply->stake);
	ply->stake = bid;	
}
	

//showdown

void ask_ply(node *player, stats *status){
	
	u8 decision;
	u16 rise;
	
	if(player->state != FOLD){

		decision = get_player_decision1(player->number);
		
		u8 number_of_tries = 0;  // number of tries	
		while( decision < 1 || decision > 3 ){
			if(number_of_tries <10){	
				
				display_message(player->number, "Try again!\n");
				decision = get_player_decision1(player->number);
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
			dealer(player, status->mony);
			player->state = CALL;	
		}
		else if(decision == RISE){
			
			rise = get_player_decision2(player->number, "Player raised\n");
			while(rise <= 0){ // Make sure rise is not equal or less than 0
				rise = get_player_decision2(player->number, "Invalid value, must be above 0\n");
			}
			status->mony += rise;
			dealer(player, status->mony);	
			status->rise = player;
			player->state = RISE;				
		}
	}
}

//Uncomment
/*
void show(card *hand){
	u8 c;
	for(c=0;c<7;c++){
		wprintw(debug,"%d,", hand[c].face);
	}
	wrefresh(debug);		
	
}
*/

void showdown(node *ply, stats *s, u8 *win){
	u8 c, pl, v, e, i = 0;
	card hand[7];
	retu r, new, best;	
	node *p;
	p = ply;

	for(c=0; c<10; c++){
		win[c] = 222;
	}
	
	while(p->state == FOLD){ // Start with first player that id not folded
		printf("fold\n");
		pl++;
		p = p->next;
	}				
	for(c=0; c<5; c++){
		hand[c] = s->hand[c];
	}
	for(v=0; v<2; v++){
		hand[5+v] =  p->hand[v];
	}

	//show(hand);
	evaluate(hand, &best);	
	win[i] = p->number;
// uncomment	wprintw(debug,"\nP: %d, N: %d\n\n",p->number, best.hand);	
	pl++;
	p = p->next;
		
	for(pl; pl<(s->plys_in_game-1); pl++){
		if(p->state != FOLD){	
			for(v=0; v<5; v++){
				hand[v] = s->hand[v];
			}
			for(v=0; v<2; v++){
				hand[5+v] = p->hand[v];
			}
			//show(hand);
			evaluate(hand, &new);
//Uncomment		wprintw(debug,"\nP: %d, N: %d\n\n",p->number, new.hand);	
//Uncomment			wrefresh(debug);	
			
			e = compare_res(&new, &best);
			
//Uncomment		wprintw(debug,"Decision:%d\n\n",e);	
			
			if(e == 0){ // new higher
				for(v=0; v<10; v++){
					win[c] = 222;
				}		
				i=0;
				win[i] = p->number;
			}
			else if(e == 1){ // a tie
				i++;
				win[i] = p->number;
			}
//Uncomment		wrefresh(debug);		
			p = p->next;	
		}
		else
			p = p->next;		
	}	
}

//node *game(node *head, stats *s, WINDOW **wplay){
node *game(node *head, stats *s){
	
	// Variables
	// c - iterates through players
	// v - iterates through players hand
	// z - iterates through deck
	// ---------------> 0 <-----------------	

	u8 c, v, z=0, river, first;
	u16 stake; 

	for(c=0; c<s->plys_in_game;c++){ // Hand cards
		for(v=0; v<2; v++){
			head->hand[v] = dec[z];
			z++;
		}
		head=head->next;
	}

	s->mony = 0;
	node *curr;

	s->rise = head->next;		// round ends with big blid if no one rise	
	s->mony = s->bigblind;  	// minimum call equal to big blind
	
	curr = head;
	dealer(head, s->mony); 		// take big blind	
	
	curr = head->next ;		// begin with small blind
	dealer(curr, s->mony/2);	// take small blind
	//show(curr, s);
	
	s->folded = s->plys_in_game;
	
	c=0;	
	river=0;
	first=1;

	draw_table(head, s);		
	//draw_wplys(head, s);	
	//draw_wstats(s);
	
	while(c < 4 && s->folded != 1){  // Go over Flop, River making sure that more than one ply in the game
		while(curr != s->rise || first){
			first=0;
			if(curr->state != FOLD)
				ask_ply(curr, s);	
			
			draw_table(head, s);	
			//draw_wstats(s);		
			//draw_wplys(head, s);	
			curr = curr->next;
		}
		if(s->folded != 1){
			if(c == 0){ // River
				for(v=0; v<3; v++){
					s->hand[v] = dec[z];
					z++;
					s->cards_on_table = 3;
				}
			}
			else if(c>0 && c<3){ // Flop and Turn
				s->hand[v] = dec[z];
				v++;
				z++;
				s->cards_on_table++;
			}
			
			draw_table(head, s);	
			//draw_wstats(s);			
			//draw_wplys(head, s);
			if(c<3) // dont ask if all cards are on the table
				ask_ply(curr, s);  // Begin with
			
			draw_table(head, s);	
			//draw_wstats(s);			
			//draw_wplys(head, s);	
			curr = curr->next; // Ask player who raised last time here cos round finishes when it gets here
			c++;
		}
	}
	
	//printf("Showdown\n");
	u8 win[10];	
	showdown(head, s, win);
		
	for(v=0; v<2; v++){
// Uncomment		wprintw(debug,"\nWinners: %d\n", win[v]);			
// Uncomment		wrefresh(debug);	
	}
	
	// Collect stakes and check who takes it
	curr = head;

	printf("end0\n");
	for(c=0; c<s->plys_in_game; c++){
		stake += curr->stake;	
		curr->state=0;	
		curr->stake=0;
		curr=curr->next;	
	}	

	u8 wi=0;
	//printf("end1\n");
	while(win[wi] != 222){
		wi++;
	}
	stake /= wi;
	printf("%d\n",stake);

	curr = head;	
	//printf("end2\n");
	for(c=0; c<s->plys_in_game; c++){
		for(v=0; v<10; v++){
			if(win[v] !=  222 && win[v] == curr->number)
				curr->credit += stake;	
		}
		curr = curr->next;
	}

	draw_table(head, s);	
	//draw_wplys(head, s);	
	//draw_wstats(s);
	
	//printf("end3\n");
	//while(1){
		
	//}
	return head->next;	// Shift big blind by one	
}

void rm(node **head, u8 index, stats *s){

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

void add(node *head, stats *s){
	
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

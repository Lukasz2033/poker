#include "header.h"

#define FOLD  1
#define CALL  2
#define RISE  3

extern void draw_wplys(node *ply, WINDOW **wply, stats *s);
extern void draw_wstats(stats *s, WINDOW *wstats);
extern u8 evaluate(card *hand, retu *ret);
extern u8 compare_res(retu *new, retu *best);

card dec[52];

node *init(stats *s);
node *reload(node *head);
//void show(node *head, stats *s);
void show(card *hand);
void rm(node **head, u8 index, stats *s);
void add(node *head, stats *s);
void dealer(node *ply, u8 bid);
void ask_ply(node *ply, stats *s);
node *game(node *head, stats *s, WINDOW **wplay);
void init_dec(card *dec);

WINDOW **ppw, *wplay[3], *commands, *wstats, *debug;	

int main(){

	u8 c;	
	stats s;	
	s.plys_in_game = 3;
	s.bigblind = 20;
	s.cards_on_table = 0;

	initscr();
	echo();

	for(c=0; c<s.plys_in_game; c++){
		wplay[c] = newwin(HEIGHT, WIDTH, (c/4)*2*HEIGHT, (WIDTH*c)%(4*WIDTH));
		
	}
	ppw = wplay;
	wstats = newwin(10, 100, 8, 0);
	commands = newwin(3, 120, 30, 0);
	debug = newwin(20,100, 35, 0);


	node *head;	
	head = init(&s);
	
	for(c=0; c<2;c++){	
		init_dec(dec);
		s.cards_on_table = 0;
		head = game(head, &s, ppw);
	}
	
	for(c=0; c<6; c++){
		delwin(wplay[c]);
	}

	delwin(wstats);
	delwin(commands);	
	delwin(debug);
	endwin(); 
		
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

void ask_ply(node *ply, stats *s){
	int ris;
	//printf("ask_ply\n");
	if(ply->state != FOLD){
		//printf("FOLD\n");
		int dec;
		//printf("Player %d Chose between FOLD (1), CALL (2) or RAISE (3)\n", ply->number);
		mvwprintw(commands, 0, 0, "Player %d Chose between FOLD (1), CALL (2) or RAISE (3)\n", ply->number);  
		mvwscanw(commands, 1, 0, "%d", &dec);
		wrefresh(commands);
		//mvwprintw(commands, 1, 0, "%d\n", dec);  	
		u8 c = 0;  // number of tries	
		while( dec < 1 || dec > 3 ){
			if(c <10){	
				//printf("Player %d Chose between FOLD (1), CALL (2) or RAISE (3)\n", ply->number);
				mvwprintw(commands, 0, 0, "Player %d Chose between FOLD (1), CALL (2) or RAISE (3)\n", ply->number);  
				mvwprintw(commands, 2, 0, "Try again!\n");  
				wrefresh(commands);
				
				//scanf("%d", &dec);
				mvwscanw(commands, 1, 0, "%d", &dec);
				mvwprintw(commands, 2, 0, "Decision: %d\n", dec);  
				wrefresh(commands);					
			}
			else{
				dec = FOLD; // If input was incorrect too many times default to FOLD
				break;
			}	
		}	
		if(dec == FOLD){
			ply->state = FOLD;
			s->folded--;
		}
		else if(dec == CALL){
			dealer(ply, s->mony);
			ply->state = CALL;	
		}
		else if(dec == RISE){
			//printf("How much?\n");
			mvwprintw(commands, 0, 0, "Player %d: How much?\n", ply->number);  
			mvwscanw(commands, 1, 0, "%d", &ris);
			wrefresh(commands);
			
			//scanf("%d", &ris);
			while(ris <= 0){ // Make sure rise is not equal or less than 0
				//printf("How much?\n");
				mvwprintw(commands, 0, 0, "Player %d: How much?\n", ply->number);  
				wrefresh(commands);	
				mvwscanw(commands, 1, 0, "%d", &ris);	
				//scanf("%d", &ris);	
			}
			s->mony += ris;
			dealer(ply, s->mony);	
			s->rise = ply;
			ply->state = RISE;				
		}
		else
			mvwprintw(commands, 0, 0, "Incorrect value \n");  	
			wrefresh(commands);
			//printf("Incorrect value \n");
	}
}

void show(card *hand){
	u8 c;
	for(c=0;c<7;c++){
		wprintw(debug,"%d,", hand[c].face);
	}
	wrefresh(debug);		
	
}

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

	show(hand);
	evaluate(hand, &best);	
	win[i] = p->number;
	wprintw(debug,"\nP: %d, N: %d\n\n",p->number, best.hand);	
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
			show(hand);
			evaluate(hand, &new);
			wprintw(debug,"\nP: %d, N: %d\n\n",p->number, new.hand);	
			wrefresh(debug);	
			
			e = compare_res(&new, &best);
			
			wprintw(debug,"Decision:%d\n\n",e);	
			
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
			wrefresh(debug);		
			p = p->next;	
		}
		else
			p = p->next;		
	}	
}

node *game(node *head, stats *s, WINDOW **wplay){
	
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

	draw_wplys(head, wplay, s);	
	draw_wstats(s, wstats);
	
	while(c < 4 && s->folded != 1){
		while(curr != s->rise || first){
			first=0;
			if(curr->state != FOLD)
				ask_ply(curr, s);	
			
			draw_wstats(s, wstats);		
			draw_wplys(head, wplay, s);	
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
			draw_wstats(s, wstats);			
			draw_wplys(head, wplay, s);
			if(c<3) // dont ask if all cards are on the table
				ask_ply(curr, s);  // Begin with	
			draw_wstats(s, wstats);			
			draw_wplys(head, wplay, s);	
			curr = curr->next; // Ask player who raised last time here cos round finishes when it gets here
			c++;
		}
	}
	
	//printf("Showdown\n");
	u8 win[10];	
	showdown(head, s, win);
		
	for(v=0; v<2; v++){
		wprintw(debug,"\nWinners: %d\n", win[v]);			
		wrefresh(debug);	
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
	
	draw_wplys(head, wplay, s);	
	draw_wstats(s, wstats);
	
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
			new->number = prev->number+1; 	
			s->plys_in_game++;	
			break;			
		}
		prev=prev->next;
		curr=curr->next;
	}
}



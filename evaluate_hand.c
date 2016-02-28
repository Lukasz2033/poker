#include "header.h"

/*
void init_dec(card *dec);
void shuffle();
void sort(card *hand, u8 size, u8 by);
u8 evaluate(card *hand, retu *r);
u8 compare_res(retu *new, retu *best);


int main(){

	u8 c, v, r, tie;
	
	init_dec(dec);
	shuffle();
	card hand[7];

	for(c=0;c<7;c++){
		hand[c] = dec[c];					
	}
	sort(hand, 7, 0);
		
	for(c=0;c<7;c++){
		printf("card %d: %d, %d\n", c, hand[c].suit, hand[c].face);
	}

	retu ret, best, new;
	
	// Asign default values
	ret.hand = 999;
	for(c=0;c<5;c++){
		ret.tie_break[c] = 999;
	}
	
	evaluate(hand, &best);
	printf("Hand: %d\n",best.hand);
	u8 n =1, m;
	// Create a few hands and compare them
	printf("enter\n");
	for(v=0; v<2; v++){
		for(c=n*7; c<(n*7)+7; c++){
			//printf("%d\n",c);
			hand[c-(n*7)] = dec[c];					
		}
		printf("New card\n");	
		
		for(m=0;m<7;m++){
			printf("card %d: %d, %d\n", m, hand[m].suit, hand[m].face);
		}
		
		ret.hand = 999;
		for(m=0;m<5;m++){
			ret.tie_break[m] = 999;
		}
				
		evaluate(hand, &new);	
		printf("Hand: %d\n", new.hand);
		r = compare_res(&new, &best);

		printf("return: %d\n", r);
		//if(r == 1)
			//tie
		n++;	
	}
	//printf("Hand: %d\nHigh: %d\nSeco: %d\nThir: %d\nFort: %d\nFift: %d\n", ret.hand, ret.tie_break[0], ret.tie_break[1], ret.tie_break[2], ret.tie_break[3], ret.tie_break[4]);
	
	return 0;
}

void init_dec(card *dec){
	u8 i, j, c=0;
	for(i =0; i<4; i++){
		for(j=0; j<13; j++){
			dec[c].suit = i;
			dec[c].face = j;				
			c++;
		}
	}
}

void shuffle(void){
	srand(time(NULL));
	int i, r, seed;
	card temp;
	u8 t = 10;
	for(i=0; i<52; i++){
		r = rand()%51;
		temp = dec[r];
		dec[r] = dec[i];
		dec[i] = temp;
	}
}
*/

void sort(card *hand, u8 size, u8 by){
	
	u8 i, j, c;
	card temp;
	for(i=0; i<size; i++)
		for(j=0; j<size-1-i; j++){
			if(by == 0){
				if(hand[j].face < hand[j+1].face){
					temp = hand[j+1];
					hand[j+1] = hand[j];
					hand[j] = temp;
				}	
			}
			else if(by == 1){
				if(hand[j].suit < hand[j+1].suit){
					temp = hand[j+1];
					hand[j+1] = hand[j];
					hand[j] = temp;
				}			
			}		
		}
}

u8 evaluate_test(card *hand){
	
	u8 same, c, v, extract[5];	
	char str[100];

	static u8 player=1;

	for(c=0; c<7; c++){
		sprintf(str, "[evaluate_test] player %d: cards -> %s/%s\n", player, ret_suit(hand[c].suit), ret_face(hand[c].face));     
		//sprintf(str, "[evaluate_test] player %d\n", player);     		
		debug_one_line(str);	
	}
	player++;


			
	// Check if POKER
	same =0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face){
			same++;
			//printf("prv %d, cur %d, same %d, prev_card: %d, curr_card: %d\n",c-1, c, same, hand[c].face, hand[c-1].face );	
		}	
		else
			same=0;
		if(same == 3){
			return POKER;
		}
	}
	
	// Check if FULLHOUSE
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;	
		else
			same=0;
		if(same == 2){
			same=0;
			for(c=1;c<7;c++){
				if(hand[c].face == hand[c-1].face)
					same++;
				else
					same=0;	
				if((same == 1) && (hand[c].face != hand[c+1].face)){
					return  FULLHOUSE;
				}
			}
		}
	}
	
	// Check if FLUSH
	same = 0;
	card flush[5]; 
	sort(hand, 7, 1); // Sort by suit	
	for(c=1;c<7;c++){
		if( hand[c-1].suit == hand[c].suit )
			same++;
		else
			same=0;

		if(same == 4){
			for(v=0; v<5; v++){
				flush[v] = hand[c-v];  // Extract flush cards
			}
			//printf("Flush\n");
			sort(flush, 5, 0); // Sort by face
			return FLUSH;
		}
	}
	
	// Check if STRAIGHT
	same =0;	
	sort(hand, 7, 0);	
	for(c=1;c<7;c++){
		if( hand[c-1].face == hand[c].face+1 )
			same++;
		else
			same=0;
		if(same == 4){
			return STRAIGHT;
		}
	}

	// Check if THREE
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;	
		else
			same=0;
		if(same == 2){
			return THREE;
		}
	}
	
	// Check if DPAIR	
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;
		else
			same=0;
		if(same == 1){
			same = 0;

			c+=2;
			for(c;c<7;c++){
				if(hand[c].face == hand[c-1].face)
					same++;	
				else
					same=0;
				if(same == 1){
					return DPAIR;	
				}
			}		
		}
	}
	
	// Check if PAIR	
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;
		else
			same=0;
		//printf("OUT: prv %d, cur %d, same %d\n",c-1, c, same);
		if(same == 1){
			//printf("PAIR\n"); 	
			return PAIR;	
		}
	}	
	
	return NONE;
	
}

u8 evaluate(card *hand, retu *ret){
	
	u8 same, c, v, extract[5];	

		
	// Check if POKER
	same =0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face){
			same++;
			//printf("prv %d, cur %d, same %d, prev_card: %d, curr_card: %d\n",c-1, c, same, hand[c].face, hand[c-1].face );	
		}	
		else
			same=0;
		if(same == 3){
			//printf("poker\n");
			ret->hand = POKER;
			ret->tie_break[0] = hand[c].face; 
			return 0;
		}
	}
	
	// Check if FULLHOUSE
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;	
		else
			same=0;
		//printf("prv %d, cur %d, same %d, prev_card: %d, curr_card: %d\n",c-1, c, same, hand[c].face, hand[c-1].face );	
		if(same == 2){
			same=0;
			//printf("THREE!!!!!\n");
			for(c=1;c<7;c++){
				if(hand[c].face == hand[c-1].face)
					same++;
				else
					same=0;	
				if((same == 1) && (hand[c].face != hand[c+1].face)){
					//printf("Full\n");
					ret->hand = FULLHOUSE;
					ret->tie_break[0] = hand[c].face; // Record value in case of a tie 	
					return 0;
				}
			}
		}
	}
	
	// Check if FLUSH
	same = 0;
	card flush[5]; 
	sort(hand, 7, 1); // Sort by suit	
	for(c=1;c<7;c++){
		if( hand[c-1].suit == hand[c].suit )
			same++;
		else
			same=0;

		if(same == 4){
			for(v=0; v<5; v++){
				flush[v] = hand[c-v];  // Extract flush cards
			}
			//printf("Flush\n");
			sort(flush, 5, 0); // Sort by face
			ret->hand = FLUSH;
			ret->tie_break[0] = flush[0].face;
			ret->tie_break[1] = flush[1].face;
			ret->tie_break[2] = flush[2].face;
			ret->tie_break[3] = flush[3].face;
			ret->tie_break[4] = flush[4].face;			
			return 0;
		}
	}
	
	// Check if STRAIGHT
	same =0;	
	sort(hand, 7, 0);	
	for(c=1;c<7;c++){
		if( hand[c-1].face == hand[c].face+1 )
			same++;
		else
			same=0;
		//printf("Straight: %d\n", same);
		if(same == 4){
			ret->hand = STRAIGHT;
			ret->tie_break[0] = hand[c-4].face; // Record highest card in case of a tie
			ret->tie_break[1] = 255;		
			ret->tie_break[2] = 255;		
			ret->tie_break[3] = 255;		
			ret->tie_break[4] = 255;		
			
			return 0;
		}
	}

	// Check if THREE
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;	
		else
			same=0;
		//printf("prv %d, cur %d, same %d, prev_card: %d, curr_card: %d\n",c-1, c, same, hand[c].face, hand[c-1].face );	
		if(same == 2){
			ret->hand = THREE;
			ret->tie_break[0]  = hand[c].face; 
			ret->tie_break[1] = 255;		
			ret->tie_break[2] = 255;		
			ret->tie_break[3] = 255;		
			ret->tie_break[4] = 255;		
			
			return 0;
			//printf("THREE!!!!!\n");
		}
	}
	
	// Check if DPAIR	
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;
		else
			same=0;
		//printf("OUT: prv %d, cur %d, same %d\n",c-1, c, same);
		if(same == 1){
			ret->tie_break[0]  = hand[c].face; 	
			same = 0;

			c+=2;
			for(c;c<7;c++){
				if(hand[c].face == hand[c-1].face)
					same++;	
				else
					same=0;
				//printf("IN: prv %d, cur %d, same %d\n",c-1, c, same);
				if(same == 1){
					//printf("DPAIR\n");
					ret->hand = DPAIR;	
					ret->tie_break[1] = hand[c].face;
					v=2;	
					for(c=0; c<7; c++){
						if(hand[c].face != ret->tie_break[0] && hand[c].face != ret->tie_break[1]){
							ret->tie_break[v] = hand[c].face; // Side card
							v++;
						}
						if(v == 3){
							ret->tie_break[3] = 255;		
							ret->tie_break[4] = 255;		
							return 0;
						}
					}
				}
			}		
		}
	}
	
	// Check if PAIR	
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;
		else
			same=0;
		//printf("OUT: prv %d, cur %d, same %d\n",c-1, c, same);
		if(same == 1){
			//printf("PAIR\n"); 	
			ret->hand = PAIR;	
			ret->tie_break[0]  = hand[c].face;
			v=1; // Index of tie breaker
			for(c=0;c<7;c++){
				if(hand[c].face != ret->tie_break[0]){ // Find highest card which is not the same as the pair
					ret->tie_break[v] = hand[c].face; // Side card	
					v++;
				}

				if(v == 3){ // Only 5 cards are can be used to break a tie	
					ret->tie_break[3] = 255;		
					ret->tie_break[4] = 255;		
					return 0;	
				}
			}
		}
	}	
	debug_one_line("None");	
	ret->hand = NONE;
	
	ret->tie_break[0]  = hand[0].face; 
	ret->tie_break[1] = 255;		
	ret->tie_break[2] = 255;		
	ret->tie_break[3] = 255;		
	ret->tie_break[4] = 255;		
	return 0;		
}

u8 compare_res(retu *new, retu *best){
	
	u8 c;

	if(new->hand > best->hand){
		best = new;
		return 0;
	}
	else if(new->hand < best->hand){
		return 2;		
	}
	else if(new->hand == best->hand){
		c = 0;
		while(new->tie_break[c] != 255 && c < 5){ // make sure tie_break is valid and we are inside the array
			if(new->tie_break[c] > best->tie_break[c]){
				best = new;
				return 0;
			}
			else if(new->tie_break[c] < best->tie_break[c]){
				return 2; 
			}
			c++;
		}
	}
	// A Tie 
	return 1;
	
}

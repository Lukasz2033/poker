#include <stdio.h>
#include <stdlib.h>

#define NONE		0
#define POKER 		3
#define FULLHOUSE 	4
#define FLUSH		5
#define STRAIGHT	6
#define THREE 		7
#define DPAIR 		8
#define PAIR 		9

typedef unsigned int u8;

typedef struct card{
	u8 suit;
	u8 face;
}card;

void init_dec(card *dec);
void shuffle();
void sort(card *hand, u8 by);
u8 evaluate(card *hand);

card dec[52];

int main(){

	init_dec(dec);
	shuffle();

	u8 c;
	card hand[7];
	//u8 su[] = {11, 10, 9, 8, 7, 0, 0}; 

	for(c=0;c<7;c++){
		//hand[c].suit = 1;
		hand[c] = dec[c];
		//printf("card %d: %d, %d\n", c, hand[c].suit, hand[c].face);
	}
	
	sort(hand, 0);
	//printf("\n");
		
	for(c=0;c<7;c++){
		printf("card %d: %d, %d\n", c, hand[c].suit, hand[c].face);
	}

	u8 ret;
	ret = NONE;
	ret = evaluate(hand);
	
	printf("Hand: %d\n", ret);
	
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

void sort(card *hand, u8 by){
	
	u8 i, j, c;
	card temp;
	for(i=0; i<7; i++)
		for(j=0; j<7-1-i; j++){
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

u8 evaluate(card *hand){
	
	u8 same, c;
	u8 straight=0, flush=0, three=0;
	
	same = 0;
	sort(hand, 1);	
	for(c=1;c<7;c++){
		if( hand[c-1].suit == hand[c].suit )
			same++;
		else
			same=0;
		
		if(same == 4)
			flush = 1;	
	}
	
	same =0;	
	sort(hand, 0);	
	for(c=1;c<7;c++){
		if( hand[c-1].face == hand[c].face+1 )
			same++;
		else
			same=0;
		//printf("Straight: %d\n", same);
		if(same == 4){
			straight = 1;
			break;
		}
	}

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
			return POKER;
		}
	}
	
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
					return FULLHOUSE;
				}
			}
	
		three = 1;
		}
	}
	
	if(flush == 1)
		return FLUSH;
	else if(straight == 1)
		return STRAIGHT;
	else if(three == 1)
		return THREE;
	
	same=0;
	for(c=1;c<7;c++){
		if(hand[c].face == hand[c-1].face)
			same++;
		else
			same=0;
		//printf("OUT: prv %d, cur %d, same %d\n",c-1, c, same);
		if(same == 1){
			same = 0;
			c+=2;
			for(c;c<7;c++){
				if(hand[c].face == hand[c-1].face)
					same++;	
				else
					same=0;
				//printf("IN: prv %d, cur %d, same %d\n",c-1, c, same);
				if(same == 1){
					//printf("2\n");
					return DPAIR;
				}
			}
			//printf("1\n");
			return PAIR;			
		}
	}		
	return 0;
}

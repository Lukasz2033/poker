#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include "header.h"

#define TAB   9
#define ENTER 10
#define ESC  27
#define BS  127

#define NUMBER 3
#define INPUT_LINE 0
#define HELP_LINE 1
#define HELP_SIZE 10
#define PROMPT_SIZE 7

char command[20];
char ch, *list_of_choises[] = { "fold", "call", "rise"};	


int get_input(WINDOW *cmd){


	// Initialise window
	//WINDOW *cmd;
	//initscr();
	//noecho();
	//raw();

	int cursor_position=0;
	
	while(1){
	
		// Update input
		mvwprintw(cmd, INPUT_LINE, PROMPT_SIZE, "");	
		wclrtoeol(cmd);	
		mvwprintw(cmd, INPUT_LINE, PROMPT_SIZE, "%s", command);
		
		// Draw prompt
		mvwprintw(cmd, INPUT_LINE, 0, "prompt>");	
		wrefresh(cmd);
		
		// Process input
		ch = mvwgetch(cmd, INPUT_LINE, cursor_position+PROMPT_SIZE);
		
		int c, line=0, matches=0, index, ret;

		switch((int)ch){
			case(BS):
				if(cursor_position > 0){
					command[--cursor_position]=0;
				}
				break;
			case(TAB): 
				// if TAB was pressed compare current input against list of commands.	
				// if more then one commands match display all possibilities.
				// if onle one command match then do auto completition.
			
				//mvwprintw(cmd, 1, 0, "%d\n", strncmp(command, "fff", strlen(command)));
				//mvwprintw(cmd, 1, 0, "%d\n", strncmp(command, list_of_choises[c], strlen(command)));
				
				for(c=0; c<HELP_SIZE; c++){
					mvwprintw(cmd, HELP_LINE+((c)%HELP_SIZE), 0, "");	
					wclrtoeol(cmd);	
				}	
				
				
				for(c=0; c<NUMBER; c++){
					if(strncmp(command, list_of_choises[c], strlen(command)) == 0){	
						mvwprintw(cmd, HELP_LINE+((line)%HELP_SIZE), 0, "");	
						wclrtoeol(cmd);		
						mvwprintw(cmd, HELP_LINE+((line)%HELP_SIZE), 0, "%s", list_of_choises[c]);		
					
						line++;
						matches++;
						index=c;
					} 
				}	
				
				
				if(matches==0){
					mvwprintw(cmd, HELP_LINE, 0, "No command found\n");	
				}	
				else if(matches==1){
					// Auto complite the command if only one match found
					for(c=strlen(command); c<strlen(list_of_choises[index]); c++){
						command[c] = list_of_choises[index][c];
						cursor_position=strlen(list_of_choises[index]);
					}
				}
				break;
			case(ENTER):
				// Process command when "ENTER" is pressed
				ret=99;
				if(strcmp(command, "fold") == 0){
					ret=FOLD;
				}
				else if(strcmp(command, "call") == 0){
					ret=CALL;
				}
				else if(strcmp(command, "rise") == 0){
					ret=RISE;
				}
				// reset command
				while(cursor_position > 0){
					command[--cursor_position]=0;
				}

				//nocbreak();
				//echo();
				return ret;
				break;
			default:
				command[cursor_position]=ch;
				cursor_position++;
				break;
		}
		wrefresh(cmd);
	
	}
}

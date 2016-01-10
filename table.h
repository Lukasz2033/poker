#include <ncurses.h>

typedef struct window{
	
	WINDOW **ppw, *wplay[3], *commands, *wstats, *debug;

}window;

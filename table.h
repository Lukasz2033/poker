#include <ncurses.h>

typedef struct window{
	
	WINDOW *commands, *wstats, *debug;

}window;

window win;


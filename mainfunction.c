#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <menu.h>
#include "window.h"

#define MENU_OPTIONS 10

char *menu_options[] = {
	"Open",
	"New",
	"Edit",
	"undo",
	"replace",
	"delete",
	"Save",
	"Save As",
	"Exit",
};
						
int nchoices = (sizeof(menu_options)) / (sizeof(char *));

int main() {
	/*the_window window
	menu_window menu_window
	abscissa abscissa
	ordinate ordinate*/
	WINDOW *the_window, *menu_window, *the_window_menu;
	ITEM **my_items;
	MENU *my_menu;
	list *fb;
	
	int height, width, abscissa, ordinate, exit = 0;
	int highlight = 1;
	int ch, c, choice = 0, i, j;
	char str[81];
	
	fb = (list *)malloc(sizeof(list));
	if (fb == NULL)
		return;
	
	InitialiseBuffer(fb);
	
	initscr();
	clear();
	noecho();
	cbreak();
	start_color();
	/*Checking whether the terminal supports colors*/
	if (has_colors() == FALSE){
		endwin();
		printf("Your terminal does not support colors\n");
	}
	keypad(stdscr, TRUE);
	height = 3;
	width = 10;
	ordinate = (LINES - height)/2;
	abscissa = (COLS - width) / 2;	
	refresh();

	the_window = Create_Window(height, width, ordinate, abscissa);
	mvwhline(the_window, 5, 1,	ACS_HLINE, width - 1);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);

	/* Create items */
	my_items = (ITEM **)calloc(nchoices, sizeof(ITEM *));
	for(i = 0; i < nchoices; ++i)
		my_items[i] = new_item(menu_options[i], menu_options[i]);

	/* Create menu */	
	my_menu = new_menu((ITEM **)my_items);

	/* Set menu option not to show the description */
	menu_opts_off(my_menu, O_SHOWDESC);

	/* Create the window to be associated with the menu */
	the_window_menu = newwin(0, 0, 0, 0);
	keypad(the_window_menu, TRUE);

	/* Set main window and sub window */
	set_menu_win(my_menu, the_window_menu);
	set_menu_sub(my_menu, derwin(the_window_menu, 0, 0, 0, 0));
 	set_menu_format(my_menu, 1, 6);
	set_menu_mark(my_menu, " * ");

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(the_window_menu);
	
	i = 0;
	mvwhline(the_window_menu, 1, 0, ACS_HLINE, COLS);
	mvwprintw(the_window_menu, LINES - 1, 0, "Press F3 to go to the menu, F6 to exit", c);
	
	while(1){
		choice = Toggle_Menu(the_window_menu, my_menu, i);
		i = choice;
		switch(choice){
			case 0:
				Open_File(fb, the_window_menu);
				break;
			case 1:
				New_File(fb, the_window_menu, my_menu);
				break;
			case 2:
				Edit_File(fb, the_window_menu);
				break;
			case 3:
				Save_File(fb, the_window_menu);
				break;
			case 4:
				Save_As(fb, the_window_menu);
				break;
			case 5:
				Exit(fb, the_window_menu);
				exit = 1;
				break;
			default:
				break;
		}
		if (exit)
			break;
	}
	/*Assertion: the user wants to exit the program*/
		
	/* Unpost and free all the memory taken up */
	unpost_menu(my_menu);
	free_menu(my_menu);
	for(j = 0; j < nchoices; ++j)
		free_item(my_items[j]);
                
	clrtoeol();
	refresh();	
	
	/*Ending curses mode*/
	endwin();
	return 0;
}

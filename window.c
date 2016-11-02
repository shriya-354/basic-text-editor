#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "window.h"
#include <menu.h>

/*Creates a new window*/
WINDOW *Create_Window(int height, int width, int ordinate, int abscissa) {
	WINDOW *local_win;
	local_win = newwin(height, width, ordinate, abscissa);
	assume_default_colors(COLOR_WHITE, COLOR_BLACK);
	return local_win;
}

/*Deletes the window*/
void Destroy_Window(WINDOW *local_win) {	
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(local_win);
	delwin(local_win);
}

/*Toggles between the options on the menu bar*/
int Toggle_Menu(WINDOW *the_window_menu, MENU *my_menu, int j) {
	int c, choice;
	choice = 0;
	while((c = wgetch(the_window_menu)) != KEY_F(6)){
		switch(c){	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				if (j < 5)
					j = j + 1;
				break;
			
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				if (j > 0)
					j = j - 1;
				break;
			
			case KEY_LEFT:
				menu_driver(my_menu, REQ_LEFT_ITEM);
				if (j > 0)
					j = j - 1;
				break;
			
			case KEY_RIGHT:
				menu_driver(my_menu, REQ_RIGHT_ITEM);
				if (j < 5)
					j = j + 1;
				break;
			
			case KEY_NPAGE:
				if (j < 5)
					j = j + 1;
				menu_driver(my_menu, REQ_SCR_DPAGE);
				break;
			
			case KEY_PPAGE:
				if (j > 0)
					j = j - 1;
				menu_driver(my_menu, REQ_SCR_UPAGE);
				break;
			
			case KEY_ENTER: 
			case KEY_BREAK: 
			case 10: 
			case 13:
				choice = 1;
				break;
		}
		wrefresh(the_window_menu);
	        if (choice)
	        	break;
	}	
	return j;
}

/*Opens a file*/
void Open_File(list *fb, WINDOW *the_window_menu) {
	char str[MAXCHAR];
	int i = 0, c;
	mvwprintw(the_window_menu, LINES - 3, 0 , "Enter the name of the file: ");
	int y = LINES - 2, x = 0;
	while((c = wgetch(the_window_menu)) != 10) {
		switch(c) {
			case KEY_BACKSPACE: 
			case 8:
				if (x > 0){
					x--;
					wmove(the_window_menu, y, x);
					i--;
				}
				mvwprintw(the_window_menu, y, x, " ");	
				str[i] = ' ';
				break;
			default:
				str[i] = c;
				i++;
				mvwprintw(the_window_menu, y, x, "%c", c);
				x++;
				break;
		}
	}
	str[i] = '\0';
	OpenFile(fb, str);
	Edit_File(fb, the_window_menu);
}


/*Opens a new window*/
void New_File(list *fb, WINDOW *the_window_menu, MENU *my_menu) {
	int j;
	Exit(fb, the_window_menu);
	clrtoeol();
	refresh();	
	main();
	exit(1);
}

/*Edits the current file*/
void Edit_File(list *fb, WINDOW *the_window_menu) {
	/*Printing the existing file in the buffer to the window*/
	PrintBuffer(fb, the_window_menu);
	int x = 0, y = 2, c, choice = 0;
	int ymax = 2;
	
	/*Creating temporary buffers*/
	char str[100][MAXCHAR], string[MAXCHAR];
	int i = 0, j = 0;
	char ch;
	int ln = 2;
	node *p;
	
	/*Copying the contents of the buffer into the 2-D temporary buffer*/
	p = fb->front;
	while(p){
		strcpy(&str[ln][0], p->string);
		p = p->next;
		ln++;
	}	
		
	/*If the buffer was non-empty, the contents of the temp buffer will be rewritten*/
	FreeBuffer(fb);
	InitialiseBuffer(fb);
	ymax = ln;
	y = ymax;
	x = 0;
	
	/*Taking input from the user*/
	while((c = wgetch(the_window_menu)) != KEY_F(6) ||(c = wgetch(the_window_menu)) != KEY_F(3) ){
		switch(c){
			case KEY_UP:				
				if (y > 2){
					y--;
					wmove(the_window_menu, y, x);
					ln--;
				}
				break;
			case KEY_DOWN:
				y++;
				if (y > ymax)
					ymax = y;
				wmove(the_window_menu, y, x);
				ln++;
				break;
			case KEY_LEFT:
				if (x > 0){
					x--;
					wmove(the_window_menu, y, x);
					i--;
				}
				break;
			case KEY_RIGHT:
				if (x < MAXCHAR){
					x++;
					wmove(the_window_menu, y, x);
					i++;
				}
				break;
			case KEY_BACKSPACE: 
			case 8:
				if (x > 0){
					x--;
					wmove(the_window_menu, y, x);
					i--;
				}
				mvwprintw(the_window_menu, y, x, " ");	
				str[y][x] = ' ';
				
				break;
			case 10: 
			case 13:
				
				str[y][x] = '\n';
				
				y++;
				ln++;
				if (y > ymax)
					ymax = y;
				x = 0;
				wmove(the_window_menu, y, x);
				break;
			/*Escape*/
			case 27:
				choice = 1;
				break;
			default:
				str[y][x] = c;
				/*Printing the character read to the screen*/				
				mvwprintw(the_window_menu, y, x, "%c", c);
				x++;
				
				wmove(the_window_menu, y, x);

				break;
		}
		/*If the user has pressed escape*/
		if (choice)
			break;
	}
	
	/*Copying from temp buffer to permanent buffer*/
	for (i = 2; i < ymax ; i++){	
		/*For each character that was read*/
		for (j = 0; str[i][j] != '\n'; j++){
			
			string[j] = str[i][j];
		}
		string[j] = '\0';
		/*Reinserting the strings from the temp buffer to the file buffer*/
		AppendtoBuffer(fb, string);
	}
	
}

/*Saves the current file*/
void Save_File(list *fb, WINDOW *the_window_menu){
	char str[MAXCHAR];
	int i = 0, c;
	/*Taking the name of the file from the user*/
	mvwprintw(the_window_menu, LINES - 3, 0 ,"File Name: ");
	int y = LINES - 2, x = 0;
	while((c = wgetch(the_window_menu)) != 10){
		switch(c){
			case KEY_BACKSPACE: 
			case 8:
				if (x > 0){
					x--;
					wmove(the_window_menu, y, x);
					i--;
				}
				mvwprintw(the_window_menu, y, x, " ");	
				str[i] = ' ';
				break;
			default:
				str[i] = c;
				i++;
				mvwprintw(the_window_menu, y, x, "%c", c);
				x++;
				break;
		}
	}
	str[i] = '\0';
	SaveFile(fb, str);
}

/*Saves the current file under different name*/
void Save_As(list *fb, WINDOW *the_window_menu){
	char str[MAXCHAR];
	int i = 0, c;
	mvwprintw(the_window_menu, LINES - 3, 0 , "Enter the  new name of the file: ");
	int y = LINES - 2, x = 0;
	while((c = wgetch(the_window_menu)) != 10){
		switch(c){
			case KEY_BACKSPACE: case 8:
				if (x > 0){
					x--;
					wmove(the_window_menu, y, x);
					i--;
				}
				mvwprintw(the_window_menu, y, x, " ");	
				str[i] = ' ';
				break;
			default:
				str[i] = c;
				i++;
				mvwprintw(the_window_menu, y, x, "%c", c);
				x++;
				break;
		}
	}
	str[i] = '\0';
	SaveFile(fb, str);
}

/*Exits from the program*/	
void Exit(list *fb, WINDOW *the_window_menu){
	FreeBuffer(fb);
	Destroy_Window(the_window_menu);
	endwin();	
}

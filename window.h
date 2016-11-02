#include <curses.h>
#include <menu.h>
#include "functions.h"

WINDOW *Create_Window(int height, int width, int ordinate, int abscissa);/*Creates a new window*/

void Destroy_Window(WINDOW *local_win);					/*Deletes the window*/

int Toggle_Menu(WINDOW *the_window_menu, MENU *my_menu, int i);		/*Toggles between the options on the menu bar*/

void Open_File(list *fb, WINDOW *the_window_menu);			/*Opens a file*/

void New_File(list *fb, WINDOW *the_window_menu, MENU *my_menu);	/*Opens a new window*/

void Edit_File(list *fb, WINDOW *the_window_menu);			/*Edit the current file*/

void Save_File(list *fb, WINDOW *the_window_menu);			/*Saves the current file*/

void Save_As(list *fb, WINDOW *the_window_menu);			/*Saves the current file under another name*/

void Exit(list *fb, WINDOW *the_window_menu);				/*Exits from the program*/

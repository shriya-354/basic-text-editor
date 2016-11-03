/*Copyright 2016 Shriya Shende
This program is a part of the project Charlie.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include<ncurses.h>
#include<ctype.h>
#include<stdlib.h>
#include"edit.h"

/*
#define COM_MODEL 0
#define INS_MODEL 1
#define SAVE_MODEL 2
#define VK_TAB 9
#define VK_ENTER 10
#define WORD_BUF_NUM 20
#define VK_ESCAPE 27
#define VK_EDIT 58
#define LINE_BUF_NUM 135
*/

int lineIsEmpty(WINDOW *editWin) {
        int y,x,i = 0;
        getyx(editWin,y,x);
        if(x == 0) {
                while(i < COLS && isspace(winch(editWin)))
                        wmove(editWin,y,++i);
                if(i >= COLS) {
                        wmove(editWin,y,x);
                        return 1;
                }
        }
        wmove(editWin,y,x);
	}
	wmove(editWin,y,x);
	return 0;
}

void insert(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag) {
	int y,x;
	getyx(editWin,y,x);
	if(*flag == INS_MODEL && isprint(ch)) {
		winsch(editWin,ch);
		wmove(editWin,y,++x);
	}
	if(*flag == COM_MODEL) {
		if(isdigit(ch) && isdigit(*prev)) {
			*prev =	(*prev - '0') * 10 + ( ch - '0');
			mvprintw(20,30,"%d",*prev);
		}
		else
			*prev = ch;
	}
	if(ch == VK_ESCAPE && *flag == INS_MODEL)
		*flag = COM_MODEL;
}

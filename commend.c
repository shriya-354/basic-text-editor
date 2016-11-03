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
#include"commend.h"
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

void isPrintI(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag) {
	if(*flag == INS_MODEL)
		insert(editWin,comWin,prev,ch,flag);
	else
		*flag = INS_MODEL;
}
void commendDisplay(WINDOW *editWin,WINDOW *comWin) {
	int editY, editX, comY, comX;
	int YMax, XMax;
	getyx(editWin, editY, editX);
	getyx(comWin, comY, comX);
	getYMaxx(comWin, YMax, XMax);
	//wclrtoeol(comWin);
	wmove(comWin, 0, 0);
	wclrtobot(comWin);
	wrefresh(comWin);
	mvwprintw(comWin, 0, XMax-20, "(%d,%d)", (editX + 1), (editY + 1));
	wrefresh(comWin);
	wmove(editWin,editY,editX);
}

void delX(WINDOW *editWin,int *flag,int *buf) {
	if(*flag == COM_MODEL) {
		*buf = winch(editWin);
		if(lineIsEmpty(editWin))
			wdeleteln(editWin);
		else
			wdelch(editWin);
	}
}

void delWord(WINDOW *editWin,int *prev,int *flag,int *buf) {
	int i = 0;
	int y,x;
	getyx(editWin,y,x);
	if(*flag == COM_MODEL) {
		if(*prev == 'd') {
			if(lineIsEmpty(editWin))
				wdeleteln(editWin);
			else {
				while(!isspace(winch(editWin)) && x > 0)
					wmove(editWin,y,--x);
				if(x > 0)
				wmove(editWin,y,++x);
				while(!isspace(winch(editWin))) {
					buf[i++] = winch(editWin); 
					wdelch(editWin);
				}
				buf[i] = '\0';
				if(isspace(winch(editWin)))
					wdelch(editWin);
			}
			*prev = EOF;
		}
	}
}

void delLine(WINDOW *editWin,int *prev,int *flag,int *buf) {
	int y, x, i = 0;
	int ch;
	if(*flag == COM_MODEL && *prev == 'd') {
		getyx(editWin, y, x);
		wmove(editWin, y, 0);
		while(i < COLS - 1) {
			buf[i] = winch(editWin);
			wmove(editWin,y,++i);
		}
		buf[i] = winch(editWin);
		buf[i + 1] = '\0';
		wdeleteln(editWin);
		wmove(editWin,y,0);
		*prev = EOF;
	}
}

void delToEnd(WINDOW *editWin,int *prev,int *flag) {
	int y, x, i;
	getyx(editWin, y, x);
	if(*flag == COM_MODEL && *prev == 'd') {
		i = COLS - x;
		if(lineIsEmpty(editWin))
			wdeleteln(editWin);
	    else { 
		 	while(i >= 0) {
				wdelch(editWin);
				--i;
			}
		}
		*prev = EOF;
	}
}


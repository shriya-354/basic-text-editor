/*****************************************************************************
 * Copyright (C) Shriya Shende shriyavs04@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
#include<ctype.h>
#include<errno.h>
#include"texteditor.h"
int main(int argc,char *argv[]) {
	if(argc < 2) {
		
		fprintf(stderr, "Enter File Name:");
		exit(1);
	}
	else {
		if(!strcmp("-h", argv[1])) {
			FILE *fp = fopen("help.c", "r");
			if(fp == NULL) {
				errno = EINVAL;
				perror("Unable to open : ");
				return errno;
			}
			char ch;
			while((ch = fgetc(fp)) != EOF)
				putchar(ch);
			return 0;
		}
		FILE *fp;
		fp = fopen(argv[1], "a+");
			int y, x, maxy, maxx;
			int ch, prev = EOF;
			int quit = false;
			int charBuf;
			int lastLine;
			WINDOW *editWin, *comWin;
			int flag = COM_MODEL;
			int wordBuf[WORD_BUF_NUM], lineBuf[LINE_BUF_NUM];

			initscr(); /*initscr() initializes the terminal in curses mode*/
			start_color();
			cbreak();
			noecho(); /*  switches off echoing */
			editWin = derwin(stdscr, LINES- 1, COLS, 0, 0);
			comWin = derwin(stdscr, 1,COLS, LINES - 1, 0);
			scrollok(editWin,TRUE);
			refresh(); //prints data on stdscr screen
			keypad(stdscr, TRUE);
			keypad(editWin, TRUE);
			keypad(comWin, TRUE);
			while((ch = getc(fp)) != EOF) {
				waddch(editWin,ch);
			}
			getmaxyx(editWin, maxy, maxx);
			lastLine = getLastLine(editWin, maxy, maxx);
			wmove(editWin, 0, 0); /* moves the cursor to rowth row and colth column */
			commendDisplay(editWin, comWin);
			while((ch = wgetch(editWin)) != EOF && quit == false) {
				switch(ch) {
					case KEY_UP : getyx(editWin, y, x);
							wmove(editWin,--y,x);
							commendDisplay(editWin, comWin);
							break;
					case KEY_DOWN : getyx(editWin, y, x);
							wmove(editWin, ++y, x);
							commendDisplay(editWin, comWin);
							break;
					case KEY_RIGHT : getyx(editWin, y, x);
							 wmove(editWin, y, ++x);
							 commendDisplay(editWin, comWin);
							 break;
					case KEY_LEFT : getyx(editWin, y, x);
							wmove(editWin, y, --x);
							commendDisplay(editWin, comWin);
							break;
					case VK_ESCAPE : flag = COM_MODEL;
							break;
					case VK_EDIT : quit = isPrint(editWin, comWin, fp, &prev, ch, &flag, lastLine);
							insert(editWin, comWin, &prev, ch, &flag);
							break; 
					case 'i' : isPrintI(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'x' : delX(editWin, &flag, &charBuf);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'B' : setBoldChar(editWin, &flag);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'C' : setHighLight(editWin, &flag);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					/*case 'w' : delWord(editWin, &prev, &flag, wordBuf);
							copyWord(editWin, &prev, &flag, wordBuf);
							pasteWord(editWin, &prev, &flag, wordBuf);
							moveWord(editWin, &prev, &flag);
							setBoldWord(editWin, &prev, &flag);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					*/
					case 'l' : delLine(editWin, &prev, &flag, lineBuf);
							copyLine(editWin, &prev, &flag, lineBuf);
							pasteLine(editWin, &prev, &flag, lineBuf);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'e' : delToEnd(editWin, &prev, &flag);
							moveLineEnd(editWin, &prev, &flag);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'G' : moveNum(editWin, &prev, &flag);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'r' : replaceChar(editWin, &flag);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'o' : insertLineUnder(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'O' : insertLineAbove(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case 'R' : replaceLine(editWin, &flag);
							insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
					case VK_ENTER : enterKey(editWin, &flag);
							commendDisplay(editWin, comWin);
							break;
					case KEY_BACKSPACE : backspaceDel(editWin, &flag);
							commendDisplay(editWin, comWin);
							break;
					case VK_TAB : tabKey(editWin, &flag);
							commendDisplay(editWin, comWin);
							break;
					default : insert(editWin, comWin, &prev, ch, &flag);
							commendDisplay(editWin, comWin);
							break;
				}
			}
			endwin();
	}
	return 0;
}

void isPrintI(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag) {
	if(*flag == INS_MODEL)
		insert(editWin,comWin,prev,ch,flag);
	else
		*flag = INS_MODEL;
}

void commendDisplay(WINDOW *editWin, WINDOW *comWin) {
	int editY, editX, comY, comX;
	int maxy, maxx;
	getyx(editWin, editY, editX);
	getyx(comWin, comY, comX);
	getmaxyx(comWin, maxy, maxx);
	wmove(comWin, 0, 0);
	wclrtobot(comWin);
	wrefresh(comWin);
	mvwprintw(comWin, 0, maxx-20 ,"(%d,%d)", (editX + 1), (editY + 1));
	wrefresh(comWin);
	wmove(editWin, editY, editX);
}

void delX(WINDOW *editWin, int *flag, int *buf) {
	if(*flag == COM_MODEL) {
		*buf = winch(editWin);
		if(lineIsEmpty(editWin))
			wdeleteln(editWin);
		else
			wdelch(editWin);
	}
}

void delWord(WINDOW *editWin, int *prev, int *flag, int *buf) {
	int i = 0;
	int y, x;
	getyx(editWin, y, x);
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

void delLine(WINDOW *editWin, int *prev, int *flag, int *buf) {
	int y,x,i = 0;
	int ch;
	if(*flag == COM_MODEL && *prev == 'd') {
		getyx(editWin, y, x);
		wmove(editWin, y, 0);
		while(i < COLS - 1) {
			buf[i] = winch(editWin);
			wmove(editWin, y, ++i);
		}
		buf[i] = winch(editWin);
		buf[i + 1] = '\0';
		wdeleteln(editWin);
		wmove(editWin, y, 0);
		*prev = EOF;
	}
}

void delToEnd(WINDOW *editWin, int *prev, int *flag) {
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

void copyWord(WINDOW *editWin,int *prev,int *flag,int buf[]) {
	int count = 0;
	int tempCh,tempX;
	int y, x;
	getyx(editWin, y, x);
	tempX = x;
	if(*flag == COM_MODEL && *prev == 'c') {
		while(!isspace(tempCh = winch(editWin))) {
			buf[count++] = tempCh;
			wmove(editWin,y,++tempX);
		}
		buf[count] = '\0';
		wmove(editWin, y, x);
		*prev = EOF;
	}
}

void copyLine(WINDOW *editWin, int *prev, int *flag, int buf[]) {
	int count = 0,i = 0;
	int y, x, tempX, temp = COLS;
	getyx(editWin, y, x);
	tempX = x;
	if(*flag == COM_MODEL && *prev == 'c') {
		tempX = 0;
		wmove(editWin, y, tempX);
		while(tempX < COLS) {
			buf[count++] = winch(editWin);
			wmove(editWin, y, ++tempX);
		}
		buf[count] = '\0';
		wmove(editWin,y,x);
		*prev = EOF;
	}
}

void pasteWord(WINDOW *editWin, int *prev, int *flag, int buf[]) {
	int i = 0;
	int prevY, prevX;
	getyx(editWin, prevY, prevX);
	if(*flag == COM_MODEL && *prev == 'p') {
		while(buf[i] != '\0') {
			if(isspace(winch(editWin))) {
				waddch(editWin, buf[i]); /* adds a character into the given window*/
				wmove(editWin, prevY, ++prevX);
			}
			else {
				winsch(editWin,buf[i]);
				wmove(editWin,prevY,++prevX);
			}
			i++;
		}
		*prev = EOF;
	}
}

void pasteLine(WINDOW *editWin, int *prev, int *flag, int buf[]) {
	int i = 0;
	int y, x;
	if(*flag == COM_MODEL && *prev == 'p') {
		winsertln(editWin);
		getyx(editWin, y, x);
		wmove(editWin, y, 0);
		while(buf[i] != '\0')
			waddch(editWin,buf[i++]);
		*prev = EOF;
	}
}

void moveWord(WINDOW *editWin, int *prev, int *flag) {
	int y, x;
	getyx(editWin, y, x);
	if(*flag == COM_MODEL && *prev == 'm' && x < COLS) {
		while(!isspace(winch(editWin)))
			wmove(editWin, y, ++x);
		if(isspace(winch(editWin)))
			wmove(editWin, y, ++x);
		*prev = EOF;
	}
}

void moveLineEnd(WINDOW *editWin, int *prev, int *flag) {
	int y, x;
	getyx(editWin, y, x);
	if(*flag == COM_MODEL && *prev == 'm' && x < COLS) {
		wmove(editWin, y, COLS - 1);
		*prev = EOF;
	}
}

void moveNum(WINDOW *editWin, int *prev,int *flag) {
	int maxy, maxx;
	getmaxyx(editWin, maxy, maxx);
	if(*flag == COM_MODEL && isdigit(*prev)) {
		if(*prev - '0' >= maxy)
			wmove(editWin, maxy - '1', 0);

		else
			wmove(editWin,*prev - '1',0);
		*prev = EOF;
	}
}

void replaceChar(WINDOW *editWin, int *flag) {
	if(*flag == COM_MODEL)
		waddch(editWin, wgetch(editWin));
}

void replaceLine(WINDOW *editWin, int *flag) {
	int ch;
	if(*flag == COM_MODEL) {
		while((ch = wgetch(editWin)) != VK_ESCAPE)
			waddch(editWin,ch);
	}
}

void setBoldChar(WINDOW *editWin,int *flag) {
	if(*flag == COM_MODEL) {
		wattron(editWin,A_BOLD);
		waddch( editWin,winch(editWin));
		wattroff(editWin,A_BOLD);
	}
}

void setBlodAndReplace(WINDOW *editWin,int *prev,int *flag) {
	int ch;
	if(*flag == COM_MODEL && *prev == 'B') {
		wattron(editWin, A_BOLD);
		while((ch = wgetch(editWin)) != VK_ESCAPE)
			waddch(editWin, ch);
		wattroff(editWin, A_BOLD);
		*prev = EOF;
	}
}

void setBoldWord(WINDOW *editWin, int *prev, int *flag) {
	int ch;
	int y,x;
	getyx(editWin, y, x);
	if(*flag == COM_MODEL && *prev == 'b') {
		while(!isspace(winch(editWin)) && x > 0)
			wmove(editWin, y, --x);
		if(x > 0)
			wmove(editWin, y, ++x);
		wattron(editWin, A_BOLD);
		while(!isspace(ch = winch(editWin)))
			waddch(editWin, ch);	
		wattroff(editWin, A_BOLD);
		*prev = EOF;
	}
}

void insertLineUnder(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag) {
	int y,x;
	if(*flag == COM_MODEL) {
		getyx(editWin, y, x);
		wmove(editWin, y + 1, 0);
		winsertln(editWin);
		*flag = INS_MODEL;
	}
	else
		insert(editWin,comWin,prev,ch,flag);
}

void insertLineAbove(WINDOW *editWin, WINDOW *comWin, int *prev, int ch, int *flag) {
	int y, x;
	if(*flag == COM_MODEL) {
		getyx(editWin, y, x);
		wmove(editWin, y - 1, 0);
		winsertln(editWin);
		*flag = INS_MODEL;
	}
	else
		insert(editWin,comWin,prev,ch,flag);
}

void setHighLight(WINDOW *editWin, int *flag) {
	if(*flag == COM_MODEL) {
		init_pair(1, COLOR_RED, COLOR_WHITE);
		wattron(editWin, COLOR_PAIR(1));
		waddch(editWin, winch(editWin));
		wattroff(editWin, COLOR_PAIR(1));
	}
}

void backspaceDel(WINDOW *win, int *flag) {
	int ch;
	int count;
	int y, x;	
	int maxy, maxx;
	int buf[LINE_BUF_NUM];
	getmaxyx(win, maxy, maxx);
	getyx(win, y, x);
	if(*flag == COM_MODEL) {
		if(x > 0)
			wmove(win, y, --x);
		else {
			if(y - 1 >= 0) {
				wmove(win, --y, maxx - 1);
				getyx(win, y, x);
				while(isspace(winch(win)) && x > 0 )
					wmove(win, y, --x);		
			}

			else
				wmove(win,0,0);
		}
	}
	else if(*flag == INS_MODEL) {
		if( x > 0) {
			wmove(win, y, x - 1);
			wdelch(win);
			wmove(win, y, --x);
		}
		else {
			if(y - 1 >= 0) {
				wmove(win, y, maxx - 1);
				getyx(win, y, x);
				while(isspace(winch(win)) && x > 0)
					wmove(win, y, --x);
				if(!isspace(winch(win))) {
					count = x;
					while(x >= 0) {
						buf[x] = winch(win);
						wmove(win, y, --x);
					}
					buf[count + 1] = '\0';
				}
				wmove(win, --y, maxx - 1);
				getyx(win, y, x);
				while(isspace(winch(win)) && x > 0)
					wmove(win, y, --x);
				if(!isspace(winch(win)))
				wmove(win, y, ++x);
				if(count > 0 && count < maxx - x) {
					int i = 0;
					while(buf[i] != '\0') {
						waddch(win, buf[i]);
						i++;
					}
					getyx(win, y, x);
					wmove(win, y + 1, 0);
					wdeleteln(win);
					wmove(win, y, x - count - 1);
				}
			}
			else
				wmove(win, 0, 0);
		}
	}
	else {
		wdelch(win);
		if(x > 0)
			wmove(win, y, --x);
		else
			*flag = COM_MODEL;
	}
}

void enterKey(WINDOW *win, int *flag) {
	int buf[LINE_BUF_NUM];
	int y, x;
	getyx(win, y, x);
	if(*flag == COM_MODEL)
		wmove(win, ++y, 0);
	else if(*flag == INS_MODEL) {
		int i = 0, j = 0, count, maxy, maxx;
		getmaxyx(win, maxy, maxx);
		count = maxx - x;
		while(count > 0) {
			buf[i++] = winch(win);
			wdelch(win);
			count--;
		}
		buf[i] = '\0';
		wmove(win, ++y, 0);
		winsertln(win);
		while(buf[j] != '\0') {
			waddch(win, buf[j]);
			j++;
		}
		wmove(win, y, 0);
	}
}

void tabKey(WINDOW *editWin, int *flag) {
	int i;
	int y, x;
	getyx(editWin, y, x);
	if(*flag == INS_MODEL)
		for(i = 0;i < 4;i++) { 
			winsch(editWin,' ');
			wmove(editWin, y, ++x);
		}
}

void insert(WINDOW *editWin, WINDOW *comWin, int *prev, int ch, int *flag) {
	int y, x;
	getyx(editWin, y, x);
	if(*flag == INS_MODEL && isprint(ch)) {
		winsch(editWin, ch);
		wmove(editWin, y, ++x);
	}
	if(*flag == COM_MODEL) {
		if(isdigit(ch) && isdigit(*prev)) {
			*prev =	(*prev - '0') * 10 + ( ch - '0');
			mvprintw(20, 30, "%d", *prev);
		}
		else
			*prev = ch;
	}
	if(ch == VK_ESCAPE && *flag == INS_MODEL)
		*flag = COM_MODEL;
}


int lineIsEmpty(WINDOW *editWin) {
	int y, x, i = 0;
	getyx(editWin, y, x);
	if(x == 0) {
		while(i < COLS && isspace(winch(editWin)))
			wmove(editWin, y, ++i);
		if(i >= COLS) {
			wmove(editWin, y, x);
			return 1;
		}
	}
	wmove(editWin, y, x);
	return 0;
}

int getLastLine(WINDOW *editWin, int maxy, int maxx) {
	int i, j;
	wmove(editWin, maxy - 1, maxx - 1);
	for(i = maxy - 1; i >= 0; i--) {
		for(j = maxx - 1; j >= 0; j--) {
			if(isspace(winch(editWin)))
				wmove(editWin, i, j);
			else
				return i;
		}
	}
	return 0;
}

int getLastChar(WINDOW *editWin, int maxx) {
	int y, x;
	getyx(editWin, y, x);
	x = maxx - 1;
	wmove(editWin, y, x);
	while(isspace(winch(editWin)) && x > 0)
		wmove(editWin, y, --x);
	wmove(editWin, y, 0);
	return x;
}

char* save(WINDOW *editWin, FILE *fp, int lastLine) {
	int ch;
	int lastChar, temp;
	int maxy, maxx, y, x;
	int line = 0, col = 0;
	fseek(fp, 0L, SEEK_SET);
	getyx(editWin, y, x);
	getmaxyx(editWin, maxy, maxx); 
	if(fp != NULL) {
	    temp = getLastLine(editWin, maxy, maxx);
	    if(temp > lastLine)
	    	lastLine = temp;
		wmove(editWin, 0, 0);
		while(line <= lastLine) {
			col = 0;
			lastChar = getLastChar(editWin, maxx);
			if(lastChar == 0)
			lastChar = maxx - 1;
			while( col <= lastChar) {
				ch = winch(editWin);
				fputc(ch, fp);
				wmove(editWin, line, ++col);
			}
			fputc('\n', fp);
			wmove(editWin, ++line,0);
		}
		wmove(editWin, y, x);
	}
	else
		return "SAVE FAILED";
	return "SAVE SUCCESS!";
}

int isPrint(WINDOW *editWin, WINDOW *comWin, FILE *fp, int *prev, int ch, int *flag, int lastLine) {
	int y, x;
	char *str;
	int quit = false ;
	getyx(comWin, y, x);
	if(*flag == COM_MODEL) {
		*flag = SAVE_MODEL;
		wmove(comWin, 0, 0);
		wclrtoeol(comWin);
		wrefresh(comWin);
		mvwprintw(comWin, 0, 0, "%c",ch);
		while((ch = wgetch(comWin)) != VK_ENTER && *flag == SAVE_MODEL && quit == false) {
			switch(ch) {
				case 'w' : wmove(comWin,0,1);
						   wclrtoeol(comWin);
						   wrefresh(comWin);
						   waddch(comWin,ch);
						   str = save(editWin, fp, lastLine);
						   *prev = ch;
						   break;
				case 'q' : wmove(comWin,0,1);
						   wclrtoeol(comWin);
						   wrefresh(comWin);
						   str = "good bye";
						   waddch(comWin, ch);
						   quit = true;
						   break;
				case VK_ESCAPE : *flag = COM_MODEL;
						   break;
				case VK_ENTER : enterKey(comWin,flag);
						   break;
				case KEY_BACKSPACE : backspaceDel(comWin,flag);
						   break;
				default : wmove(comWin,0,1);
						  wclrtoeol(comWin);
						  wrefresh(comWin);
						  mvwprintw(comWin, 0, 1, "%c is not a commend!", ch);
			}
		}
		mvwprintw(comWin, 0, 1, "%s", str);
		wrefresh(comWin);
		*flag = COM_MODEL;
	}
	return quit;
}

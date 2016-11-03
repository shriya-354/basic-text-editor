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

#include<stdlib.h>
#include<ncurses.h>
#include<ctype.h>
#include"edit.h"
#include"commend.h"
#include"save.h"

#define COM_MODEL 0
#define INS_MODEL 1
#define SAVE_MODEL 2
#define VK_TAB 9
#define VK_ENTER 10
#define WORD_BUF_NUM 20
#define VK_ESCAPE 27
#define VK_EDIT 58
#define LINE_BUF_NUM 135

int main(int argc,char *argv[])	{
	if(argc < 2) {
		fprintf(stderr,"请输入文件名\n");
		exit(1);
	}
	else {
		FILE *fp;
		fp = fopen(argv[1],"r+");
		if(fp == NULL) {
			char create;
			fprintf(stdout,"您输入的文件不存在，是否创建？（Y/N）");
			fscanf(stdin,"%c",&create);
			if(create == 'Y' || create == 'y') {
				fp = fopen(argv[1],"w");
				fclose(fp);
			}	
			if(fp == NULL) {
				fprintf(stderr,"文件打开失败或创建文件失败\n");
				exit(1);
			}
			else
				fprintf(stdout,"文件创建成功！\n");
		} 
		else {
			int y,x,YMax,XMax;
			int ch,prev = EOF;
			int quit = false;
			int charBuf;
			int lastLine;
			WINDOW *editWin,*comWin;
			int flag = COM_MODEL;
			int wordBuf[WORD_BUF_NUM],lineBuf[LINE_BUF_NUM];

			initscr();
			start_color();
			cbreak();
			noecho();
			editWin = derwin(stdscr,LINES- 1,COLS,0,0);
			comWin = derwin(stdscr,1,COLS,LINES - 1,0);
			scrollok(editWin,TRUE);
			refresh();
			keypad(stdscr,TRUE);
			keypad(editWin,TRUE);
			keypad(comWin,TRUE);
			while((ch = getc(fp)) != EOF) {
				waddch(editWin,ch);
			}
			getYMaxx(editWin,YMax,XMax);
			lastLine = getLastLine(editWin,YMax,XMax);
			wmove(editWin,0,0);	
			commendDisplay(editWin,comWin);
			while((ch = wgetch(editWin)) != EOF && quit == false) {
				switch(ch) {
					case KEY_RIGHT : getyx(editWin,y,x);
									 wmove(editWin,y,++x);
									 commendDisplay(editWin,comWin);
									 break;
					case KEY_LEFT : getyx(editWin,y,x);
									wmove(editWin,y,--x);
								    commendDisplay(editWin,comWin);
									break;

					case KEY_DOWN : getyx(editWin,y,x);
                                                                    wmove(editWin,++y,x);
                                                                    commendDisplay(editWin,comWin);
                                                                    break;

					 case KEY_UP : getyx(editWin,y,x);
                                                                  wmove(editWin,--y,x);
                                                                  commendDisplay(editWin,comWin);
                                                                  break;

					case VK_ESCAPE : flag = COM_MODEL;break;
					case VK_EDIT : quit = isPrint(editWin,comWin,fp,&prev,ch,&flag,lastLine);
								   insert(editWin,comWin,&prev,ch,&flag);
								   break;
					case KEY_BACKSPACE : backspaceDel(editWin,&flag);
                                                                     commendDisplay(editWin,comWin);
                                                                                 break;
                                        case VK_ENTER : enterKey(editWin,&flag);
                                                                commendDisplay(editWin,comWin);
                                                                        break;

                                        case VK_TAB : tabKey(editWin,&flag);
                                                              commendDisplay(editWin,comWin);
                                                                  break;
 
					case 'i' : isPrintI(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'x' : delX(editWin,&flag,&charBuf);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'B' : setBoldChar(editWin,&flag);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'C' : setHighLight(editWin,&flag);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'w' : delWord(editWin,&prev,&flag,wordBuf);
							   copyWord(editWin,&prev,&flag,wordBuf);
							   pasteWord(editWin,&prev,&flag,wordBuf);
							   moveWord(editWin,&prev,&flag);
							   setBoldWord(editWin,&prev,&flag);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'l' : delLine(editWin,&prev,&flag,lineBuf);
							   copyLine(editWin,&prev,&flag,lineBuf);
							   pasteLine(editWin,&prev,&flag,lineBuf);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'e' : delToEnd(editWin,&prev,&flag);
							   moveLineEnd(editWin,&prev,&flag);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'G' : moveNum(editWin,&prev,&flag);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'r' : replaceChar(editWin,&flag);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'o' : insertLineUnder(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'O' : insertLineAbove(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					case 'R' : replaceLine(editWin,&flag);
							   insert(editWin,comWin,&prev,ch,&flag);
							   commendDisplay(editWin,comWin);
							   break;
					default : insert(editWin,comWin,&prev,ch,&flag);
							  commendDisplay(editWin,comWin);
							  break; 
				}
				
			}
			endwin();
		}
	}
	return 0;
}

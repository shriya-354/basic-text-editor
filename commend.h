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

#ifndef Commend_h
#define Commend_h

void commendDisplay(WINDOW *editWin,WINDOW *comWin);
void copyWord(WINDOW *editWin,int *prev,int *flag,int buf[]);
void copyLine(WINDOW *editWin,int *prev,int *flag,int buf[]);
void pasteWord(WINDOW *editWin,int *prev,int *flag,int buf[]);
void pasteLine(WINDOW *editWin,int *prev,int *flag,int buf[]);
void replaceChar(WINDOW *editWin,int *flag);
void replaceLine(WINDOW *editWin,int *flag);
void setBoldChar(WINDOW *editWin,int *flag);
void setBlodAndReplace(WINDOW *editWin,int *prev,int *flag);
void setBoldWord(WINDOW *editWin,int *prev,int*flag);
void delX(WINDOW *editWin,int *flag,int *buf);
void delWord(WINDOW *editWin,int *prev,int *flag,int *buf);
void delLine(WINDOW *editWin,int *prev,int *flag,int *buf);
void delToEnd(WINDOW *editWin,int *prev,int *flag);
void moveWord(WINDOW *editWin,int *prev,int *flag);
void moveLineEnd(WINDOW *editWin,int *prev,int *flag);
void moveNum(WINDOW *editWin,int *prev,int *flag);
void insertLineUnder(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag);
void insertLineAbove(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag);
void setHighLight(WINDOW *editWin,int *flag);
void backspaceDel(WINDOW *win,int *flag);
void enterKey(WINDOW *win,int *flag);
void tabKey(WINDOW *editWin,int *flag);
void isPrintI(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag);

#endif

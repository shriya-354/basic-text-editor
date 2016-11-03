#ifndef _TEXTEDITOR__H
#define _TEXTEDITOR__H

#define VK_ESCAPE 27
#define VK_ENTER 10
#define COM_MODEL 0
#define INS_MODEL 1
#define SAVE_MODEL 2
#define WORD_BUF_NUM 20
#define LINE_BUF_NUM 135
#define VK_EDIT 58
#define VK_TAB 9

void commendDisplay(WINDOW *editWin,WINDOW *comWin);
void delX(WINDOW *editWin,int *flag,int *buf);
void delWord(WINDOW *editWin,int *prev,int *flag,int *buf);
void delLine(WINDOW *editWin,int *prev,int *flag,int *buf);
void delToEnd(WINDOW *editWin,int *prev,int *flag);
void copyWord(WINDOW *editWin,int *prev,int *flag,int buf[]);
void copyLine(WINDOW *editWin,int *prev,int *flag,int buf[]);
void pasteWord(WINDOW *editWin,int *prev,int *flag,int buf[]);
void pasteLine(WINDOW *editWin,int *prev,int *flag,int buf[]);
void moveWord(WINDOW *editWin,int *prev,int *flag);
void moveLineEnd(WINDOW *editWin,int *prev,int *flag);
void moveNum(WINDOW *editWin,int *prev,int *flag);
void replaceChar(WINDOW *editWin,int *flag);
void replaceLine(WINDOW *editWin,int *flag);
void setBoldChar(WINDOW *editWin,int *flag);
void setBlodAndReplace(WINDOW *editWin,int *prev,int *flag);
void setBoldWord(WINDOW *editWin,int *prev,int*flag);
void insertLineUnder(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag);
void insertLineAbove(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag);
void setHighLight(WINDOW *editWin,int *flag);
void backspaceDel(WINDOW *win,int *flag);
void enterKey(WINDOW *win,int *flag);
void tabKey(WINDOW *editWin,int *flag);
void isPrintI(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag);
void insert(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag);
int lineIsEmpty(WINDOW *editWin);
int getLastLine(WINDOW *editWin,int maxy,int maxx);
int getLastChar(WINDOW *editWin,int maxx);
char* save(WINDOW *editWin,FILE *fp,int lastLine);
int isPrint(WINDOW *editWin,WINDOW *comWin,FILE *fp,int *prev,int ch,int *flag,int lastLine);

#endif

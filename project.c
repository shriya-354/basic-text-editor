#include<ncurses.h>
#include<stdlib.h>
#include<ctype.h>
#include"save.h"
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

int getLastLine(WINDOW *editWin,int YMax,int XMax) {
        int i,j;
        wmove(editWin,YMax - 1,XMax - 1);
        for(i = YMax - 1;i >= 0;i--) {
                for(j = XMax - 1;j >= 0;j--) {
                        if(isspace(winch(editWin)))
                                wmove(editWin,i,j);
                        else
                                return i;
                }
        }
        return 0;
}
int getLastChar(WINDOW *editWin,int XMax) {
        int y,x;
        getyx(editWin,y,x);
        x = XMax - 1;
        wmove(editWin,y,x);
        while(isspace(winch(editWin)) && x > 0)
                wmove(editWin,y,--x);
        wmove(editWin,y,0);
        return x;
}
       

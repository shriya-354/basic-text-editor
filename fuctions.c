#include <stdio.h>
#include <string.h>
#include "functions.h"
#include <menu.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void InitialiseBuffer(list *fb) {
	fb->front = NULL;
	fb->rear = NULL;
	fb->line_ctr = 0;
}

int BufferEmpty(list *fb) {
	if (fb->front == NULL)
		return 0;
	return 1;	
}

int BufferFull(list *fb){
	//buffer isnt full unless malloc fails
	return 0; 	
}
 
/*Will store the read character in the node (or in a new node and link to end of file buffer) */
void AppendtoBuffer(list *fb, char *ReadLine) {
	node *newnode;
	int i;
	newnode = (node *)malloc(sizeof(node));

	if (newnode == NULL)
		return;
	
	fb->line_ctr++;

	for(i = 0; i < strlen(ReadLine); i++)
		newnode->string[i] = ReadLine[i];
	newnode->prev = fb->rear;
	if (fb->rear)
		fb->rear->next = newnode;
	newnode->next = NULL;
	newnode->line_numb = fb->line_ctr;
	fb->rear = newnode;
	if (fb->front == NULL)
		fb->front = newnode;
}

void PrintBuffer(list *fb, WINDOW *the_window_menu) {
	node *m;
	m = fb->front;
	int y = 2;
	int x = 0;
	while (m){
		//printf("%s\n", m->string);
		mvwprintw(the_window_menu, y, x, "%s\n", m->string);		
		m = m->next;
		y++;
	}
}

void OpenFile(list *fb, char *Filename) {
	FILE *fp;
	fp = fopen(Filename, "r");
	if (fp == NULL)
		return;
	
	char str[MAXCHAR], ch;
	int i = 0, c;
	int y = 5, x = 0;
	
	//Get the input from the file and store in a string
	c = fgetc(fp);
	while(c != EOF){
		str[i] = c;
		i++;
		x++;
		if (c == 10 || c == 13) {
			str[i] = '\0';
			//Insert string into the buffer
			AppendtoBuffer(fb, str);
			i = 0;
			y++;
		}
		c = fgetc(fp);
	}	
	fclose(fp);
}

void EditBuffer(list *fb, int LineNo, char *NewString) {
	node *m;
	m = fb->front;
	while (m->line_numb < LineNo)
		m = m->next;
		
	strcpy(m->string, NewString);
}

void InsertBuffer(list *fb, int LineNo, char *InsertString) {
	node *m, *tmp; 
	m = fb->front;
	
	/*Traversing uptil node to be inserted*/
	while (m->line_numb < LineNo - 1)
		m = m->next;
	
	tmp = (node *)malloc(sizeof(node));
	tmp->line_numb = LineNo;
	strcpy(tmp->string, InsertString);
	
	/*linking of the new node*/
	if (LineNo == 1) {
		tmp->prev = NULL;
		tmp->next = m;
		fb->front = tmp;
	}
	else if (LineNo == fb->line_ctr + 1) {
		tmp->prev = fb->rear;
		tmp->next = NULL;
		fb->rear = tmp;
	}
	else{
		tmp->prev = m;
		tmp->next = m->next;
		m->next = tmp;
	}
	
	/*Changing line numbers of the rest of the nodes*/
	m = tmp->next;
	while(m){
		(m->line_numb)++;
		m = m->next;
	}	
	
	(fb->line_ctr)++;
}

void SaveFile(list *fb, char *Filename) {
	FILE *fp;
	fp = fopen(Filename, "w");
	node *m;
	m = fb->front;
	if (m == NULL)
		return;
	
	while(m){
		fprintf(fp, "%s\n", m->string);
		m = m->next;
	}
	
	fclose(fp);
}

void FreeBuffer(list *fb) {
	node *m;
	m = fb->front;
	if (m == NULL)
		return;
	
	while (m){
		m = m->next;
		free(fb->front);
		fb->front = m;		
	}
	
	InitialiseBuffer(fb);
}

void DeleteBufferLineNo(list *fb, int LineNo) {
	node *m, *l, *k;
	m = fb->front;
	
	/*Traversing the list*/
	while (m->line_numb < LineNo - 1)
		m = m->next;
	
	/*Deleting the node*/
	if (LineNo != 1)
		l = m->next;
	else if (LineNo == 1)
		l = m;
	k = l->next;
	
	if (l == fb->front)
		fb->front = k;
		
	if (l == fb->rear)
		fb->rear = m;
	
	m->next = k;
	k->prev = m;
	free(l);
	
	/*update the following line nos*/		
	while(k){
		(k->line_numb)--;
		k = k->next;
	}
	
	(fb->line_ctr)--;	
}

void popStr(list *myList, char *tmp) {
	node *i;
	int cntr;
	cntr=1;
	for(i = myList->front; i != NULL; i = i->next) {
		if(cntr == myList->line_ctr){
			strcpy(tmp, i->string);
		}
		cntr++;
	}
	DeleteBufferLineNo(myList, myList->line_ctr);
}

/* "u" -- the undo function */
void undo(list *fb, list *cmdHistory, list *histContent, list *backupBuffer) {
	int tmpInt;
	int atLine;
	char cmd;
	char tmpCmd[MAX_CMDCHAR];
	char tmpStr[MAXCHAR];
	node *j;
	node *h;
	int k;
	int cntr;
	cntr=1;
	popStr(cmdHistory,tmpCmd);
	cmd = tmpCmd[0];
	atLine = atoi(&tmpCmd[1]);
	switch(cmd){
		case 'a':
			if(strlen(tmpCmd)==1){
				DeleteBufferLineNo(fb,fb->line_ctr);
			}
			else{
				DeleteBufferLineNo(fb,atLine+1);
			}
			break;
		case 'r':
		case 'd':
			popStr(histContent,tmpStr);
			replace(fb,atLine,tmpStr);
			if(strcmp(tmpCmd,"d0\n")==0){
				/* undo DELETE ALL */
				/* POP the lineNumber */
				for(j = histContent->front; j != NULL; j = j->next) {
					if(cntr==histContent->line_ctr){
						tmpInt = j->line_numb;
					}
					cntr++;
				}
				DeleteBufferLineNo(histContent,histContent->line_ctr);
				cntr = 1;
				for(k=1;k<=tmpInt;k++) {
						/* POP	the backup string */
					for(h = backupBuffer->front; h != NULL; h = h->next){
						if(cntr == backupBuffer->line_ctr){
							strcpy(tmpStr, h->string);
						}
						cntr++;
					}
					DeleteBufferLineNo(backupBuffer,backupBuffer->line_ctr);
					cntr=1;
					InsertBuffer(fb,0,tmpStr);
				}
			}
			else 
				popStr(histContent,tmpStr);InsertBuffer(fb,atLine-1,tmpStr);

			break;
		case 'm':
			/* POP string */
			for(j=histContent->front;j!=NULL;j=j->next){
				if(cntr==histContent->line_ctr){
					tmpInt = j->line_numb;
				}
				cntr++;
			}
			DeleteBufferLineNo(histContent,histContent->line_ctr);
			if(strcmp(tmpCmd,"m\n")==0){
				for(k=1;k<=tmpInt;k++){
					DeleteBufferLineNo(fb,fb->line_ctr);
				}
			}
			else {
				for(k=1;k<=tmpInt;k++) {
						DeleteBufferLineNo(fb,atLine+1);
				}
			}
			break;
		default:
			break;
	}
}

void replace(list *fb,int atLine,char *restOfString) {
	node *i;
	int counter;
	counter = 1;
	for(i = fb->front; i != NULL; i = i->next) {
		if(counter==atLine){
			strcpy(i->string, restOfString);
		}counter++;
	}
}

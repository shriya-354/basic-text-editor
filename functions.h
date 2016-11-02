#include <stdio.h>
#include <string.h>
#include <menu.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXCHAR 80
#define MAX_FN_CHAR 10
#define MAX_CMDCHAR 5


typedef struct node {
	char string[MAXCHAR];
	int line_numb;
	struct node *next, *prev; 
}node;

typedef struct list {
	node *front, *rear;
	int line_ctr;
}list;

void InitialiseBuffer(list *fb);				/*Initialising the list*/

int BufferEmpty(list *fb);					/*Checking if buffer is empty*/

int BufferFull(list *fb);					/*Checking if buffer is full*/

void AppendtoBuffer(list *fb, char *ReadLine);			/*Will store the read line in a new node and link to end of file buffer */

void PrintBuffer(list *fb, WINDOW *the_window_menu);		/*Will print the entire file (in the buffer)*/

void OpenFile(list *fb, char *Filename);				/*Will open a file in the buffer*/

void SaveFile(list *fb, char *Filename);			/*Will save the buffer as a file*/

void FreeBuffer(list *fb); 					/*Will free the entire buffer*/

void EditBuffer(list *fb, int LineNo, char *NewString);		/*Will edit a line in the file*/

void InsertBuffer(list *fb, int LineNo, char *InsertString);	/*Will insert a node in the buffer*/

void popStr(list *myList, char *tmp);

void undo(list *fb, list *cmdHistory, list *histContent, list *backupBuffer); /*Will undo previous operation*/

void replace(list *fb,int atLine,char *restOfString);		/*Will replace content in particular node*/

void DeleteBufferLineNo(list *fb, int LineNo);			/*Will delete a node from the buffer given line number*/

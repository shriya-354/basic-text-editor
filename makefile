project: texteditor.c texteditor.h
	cc  -texteditor.c -o project -lncurses -lmenu
clean: 
	rm *.o

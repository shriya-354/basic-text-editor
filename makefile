project: texteditor.c texteditor.h
	cc  -texteditor -o project -lncurses -lmenu
clean: 
	rm *.o

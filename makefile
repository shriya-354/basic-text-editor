project: mainfunction.o fuctions.o window.o
	cc mainfunction.o fuctions.o window.o -o project -lncurses -lmenu
mainfunction.o: mainfunction.c window.h functions.h
	cc -c mainfunction.c
fuctions.o: fuctions.c window.h functions.h
	cc -c fuctions.c
window.o: window.c window.h functions.h
	cc -c window.c

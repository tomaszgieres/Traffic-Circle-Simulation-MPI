CC=gcc
#CFLAGS=-g
#LFLAGS=

all:

choose_exit_test: choose_exit_test.o functions.o
	$(CC) choose_exit_test.o functions.o -o choose_exit_test

choose_exit_unit_test.o: choose_exit_unit_test.c
	$(CC) -c choose_exit_unit_test.c -o choose_exit_unit_test.o
	
functions.o: functions.c
	$(CC) -c functions.c -o functions.o
	 

clean:
	rm -f *.o
	
.PHONY: all clean

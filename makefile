main: 
	compile clean

compile: main.c
	mpicc.mpich main.c -o hello

run:
	mpirun -np 5 hello

clean:
	rm -f *.o
	
.PHONY: all clean

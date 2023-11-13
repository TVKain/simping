all:
	gcc -c simping.c main.c simping_config.c
	gcc -o simping main.o simping.o simping_config.o

	gcc -c test-1.c 
	gcc -o test-1 test-1.o simping.o
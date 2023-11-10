all:
	gcc -c simping.c main.c simping_config.c
	gcc -o simping main.o simping.o simping_config.o
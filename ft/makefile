CC = gcc
FLAGS = -Wall -Werror -I -O2 -g  

objects = main.o print.o address.o socket.o listen.o send.o receive.o file.o

fly: $(objects)
	$(CC) $(FLAGS) -o fly $(objects)

main.o: main.c message.h
	$(CC) $(FLAGS) -c main.c

address.o: address.c message.h address.h include.h
	$(CC) $(FLAGS) -c address.c

socket.o: socket.c message.h include.h
	$(CC) $(FLAGS) -c socket.c

listen.o: listen.c message.h include.h listen.h
	$(CC) $(FLAGS) -c listen.c

send.o: send.c message.h include.h file.h
	$(CC) $(FLAGS) -c send.c

receive.o: receive.c message.h include.h file.h
	$(CC) $(FLAGS) -c receive.c

file.o: file.c message.h include.h
	$(CC) $(FLAGS) -c file.c


print.o: print.c message.h include.h
	$(CC) $(FLAGS) -c print.c

clean: 
	rm *.o fly
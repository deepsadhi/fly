objects = main.o print.o ip.o

fly: $(objects) 
	gcc -Wall -Werror -O2 -g -o fly $(objects)

main.o: main.c fly.h message.h
	gcc -Wall -Werror -O2 -g -c main.c 

print.o: print.c fly.h message.h
	gcc -Wall -Werror -O2 -g -c print.c 

ip.o: ip.c fly.h
	gcc -Wall -Werror -O2 -g -c ip.c 

clean:
	rm *.o fly




all: client server

client:client.o socket_stuff.o
	gcc -o client client.o socket_stuff.o

server:server.o socket_stuff.o
	gcc -o server server.o socket_stuff.o

client.o:client.c socket_stuff.h
	gcc -c client.c

server.o: server.c socket_stuff.h
	gcc -c server.c

socket_stuff.o:socket_stuff.c socket_stuff.h
	gcc -c socket_stuff.c

clean:
	rm *.o
	rm *~

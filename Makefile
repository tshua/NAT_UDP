CC = g++
CFLAGS = -Wall
SERVER = ./server/server.cpp
LINUX_CLIENT = ./linux_client/unix_client.cpp

all:
	$(CC) $(CFLAGS) $(SERVER) -o s -g
	$(CC) $(CFLAGS) $(LINUX_CLIENT) -o c -lpthread -g

clean:
	rm -vf s c

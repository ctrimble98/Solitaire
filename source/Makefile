CC	=	g++
FLAGS	=	-g	-O2	-Wextra

all:	server

server:	server.cpp	server.h	shuffle.cpp	card.cpp	card.h	klondike.cpp	klondike.h
	${CC}	${FLAGS}	server.cpp	shuffle.cpp	card.cpp	klondike.cpp	-o	server

clean:
	rm	-f	server

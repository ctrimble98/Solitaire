CC	=	g++
FLAGS	=	-g	-O2	-Wextra

all:	server

server:	server.cpp	server.h	shuffle.cpp	card.cpp	card.h	move.cpp	move.h	klondike.cpp	klondike.h	random.cpp
	${CC}	${FLAGS}	server.cpp	shuffle.cpp	card.cpp	move.cpp	klondike.cpp	random.cpp	-o	server

clean:
	rm	-f	server

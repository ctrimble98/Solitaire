CC	=	g++
FLAGS	=	-g	-O2	-Wextra

all:	server

server:	server.cpp	server.h	shuffle.cpp	card.cpp	card.h	move.cpp	move.h	klondike.cpp	klondike.h	solver.cpp	solver.h	solverCompare.cpp	solverCompare.h
	${CC}	${FLAGS}	server.cpp	shuffle.cpp	card.cpp	move.cpp	klondike.cpp	solver.cpp	solverCompare.cpp	-o	server

clean:
	rm	-f	server

CC	=	g++		-std=c++17
FLAGS	=	-g	-O2	-Wextra

all:	server

server:	server.cpp	server.h	shuffle.cpp	card.cpp	card.h	move.cpp	move.h	klondike.cpp	klondike.h	heuristic.cpp	heuristic.h	solver.cpp	solver.h	solverCompare.cpp	solverCompare.h	verifier.cpp	verifier.h
	${CC}	${FLAGS}	server.cpp	shuffle.cpp	card.cpp	move.cpp	klondike.cpp	heuristic.cpp	solver.cpp	solverCompare.cpp	verifier.cpp	-o	server

clean:
	rm	-f	server	server.exe	out.txt	klondike.json	solvOut.txt

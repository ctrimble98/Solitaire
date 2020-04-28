CC	=	g++		-std=c++17
FLAGS	=	-g	-O2	-Wextra	-pthread

SRC_DIR	=	source
INC_DIR	=	include
OBJ_DIR	=	obj

SRC_FILES	=	$(wildcard $(SRC_DIR)/*.cpp)
INC_FILES	=	$(wildcard $(INC_DIR)/*.h)
OBJ_FILES	=	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp	$(INC_FILES)
	$(CC)	-c	-o	$@	$<	-I $(INC_DIR)	$(FLAGS)

klondikeSolver:	$(OBJ_FILES)
	$(CC)	-o $@	$^	$(FLAGS)

all:	klondikeSolver

clean:
	rm	-f	$(OBJ_DIR)/*.o klondikeSolver

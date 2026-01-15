# Copyright (c) Benjamín Gajardo (also known as +KZ)

CFLAGS = `pkg-config --cflags raylib` -I ${CURDIR} -g
LDFLAGS = `pkg-config --libs raylib`
SRC = main.cpp input_handler.cpp game.cpp globals.cpp entities/entity_list.cpp entities/player.cpp collision.cpp level_handler.cpp
OBJ = $(SRC:%.cpp=%.o)
DEP = $(SRC:%.cpp=%.d)

all: bedw

bedw: $(OBJ)
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} -MMD -c $< ${CFLAGS} -o $@

-include $(DEP)

clean:
	rm -f bedw
	rm -f *.o
	rm -f *.d
	rm -f entities/*.o
	rm -f entities/*.d
	rm -f lib/*.o
	rm -f lib/*.d



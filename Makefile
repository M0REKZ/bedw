# Copyright (c) Benjamín Gajardo (also known as +KZ)

CXX = g++
CFLAGS = `pkg-config --cflags raylib` -I ${CURDIR} -g
LDFLAGS = `pkg-config --libs raylib`
SRC = \
	main.cpp \
	input_handler.cpp \
	game.cpp \
	globals.cpp \
	entities/entity_list.cpp \
	entities/player.cpp \
	entities/killer_bot.cpp \
	entities/killer_drone.cpp \
	entities/menu_camera.cpp \
	entities/victim.cpp \
	collision.cpp \
	config_handler.cpp \
	level_handler.cpp \
	level_script_handler.cpp \
	pause_handler.cpp \
	helper_fs.cpp \
	helper_ui.cpp
OBJ = $(SRC:%.cpp=%.o)
DEP = $(SRC:%.cpp=%.d)

all: ./build/bedw ./build/data

./build/data: data
	cp -r ./data $@

./build/bedw: $(OBJ)
	mkdir -p build
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} -MMD -c $< ${CFLAGS} -o $@

-include $(DEP)

clean:
	rm -r -f build
	rm -f *.o
	rm -f *.d
	rm -f entities/*.o
	rm -f entities/*.d
	rm -f lib/*.o
	rm -f lib/*.d



#    Brain Evil: Dark World
#    Copyright (C) Benjamín Gajardo (also known as +KZ)
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

CXX = g++
CFLAGS = `pkg-config --static --cflags raylib` -I ${CURDIR} -g
LDFLAGS = `pkg-config --static --libs raylib`

ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
		LDFLAGS += -framework Cocoa -framework OpenGL -framework IOKit
    endif
endif

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



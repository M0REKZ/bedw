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

ifeq ($(OS),Windows_NT)
#	Requires adding libraylib.a in a folder named "lib"
	CFLAGS = -I ${CURDIR}/src -g
	LDFLAGS = -lraylib -L ./lib/
else
	CFLAGS = `pkg-config --static --cflags raylib` -I ${CURDIR}/src -g
	LDFLAGS = `pkg-config --static --libs raylib`

    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
		LDFLAGS += -framework Cocoa -framework OpenGL -framework IOKit
    endif
endif

SRC = \
	src/main.cpp \
	src/input_handler.cpp \
	src/game.cpp \
	src/globals.cpp \
	src/entities/entity_list.cpp \
	src/entities/player.cpp \
	src/entities/killer_bot.cpp \
	src/entities/killer_drone.cpp \
	src/entities/menu_camera.cpp \
	src/entities/victim.cpp \
	src/collision.cpp \
	src/config_handler.cpp \
	src/level_handler.cpp \
	src/level_script_handler.cpp \
	src/pause_handler.cpp \
	src/helper_fs.cpp \
	src/helper_ui.cpp
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
	rm -f src/*.o
	rm -f src/*.d
	rm -f src/entities/*.o
	rm -f src/entities/*.d
	rm -f lib/*.o
	rm -f lib/*.d



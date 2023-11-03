ROOT := ${CURDIR}

CXX := g++
CPPFLAGS := -std=c++17 -g -Wall
LIBS := -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS := -L$(ROOT)/sfml/lib
INCLUDES := -I$(ROOT)/include -I$(ROOT)/sfml/include
PROGRAM := snake

all: main.o app.o game.o menu.o
	$(CXX) --output $(PROGRAM) \
	main.o \
	app.o \
	game.o \
	menu.o \
	$(LDFLAGS) \
	$(LIBS) \
	-rpath "@executable_path/sfml/lib"

main.o: src/main.cpp
	$(CXX) $(CPPFLAGS) -c src/main.cpp $(INCLUDES)

app.o: src/app.cpp
	$(CXX) $(CPPFLAGS) -c src/app.cpp $(INCLUDES)

game.o: src/game.cpp
	$(CXX) $(CPPFLAGS) -c src/game.cpp $(INCLUDES)

menu.o: src/menu.cpp
	$(CXX) $(CPPFLAGS) -c src/menu.cpp $(INCLUDES)

install:
	cp ./$(PROGRAM) /usr/local/bin

run: all
	./snake

clean:
	rm -f *.o src/*.o $(PROGRAM)

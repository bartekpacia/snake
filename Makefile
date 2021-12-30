ROOT := ${CURDIR}

CXX := g++
CPPFLAGS := -std=c++17
LIBS := -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS := -L$(ROOT)/sfml/lib
INCLUDES := -I$(ROOT)/include -I$(ROOT)/sfml/include
PROGRAM := snake

all: main.o app.o gui.o
	$(CXX) --output $(PROGRAM) \
	main.o \
	app.o \
	gui.o \
	$(LDFLAGS) \
	$(LIBS)

main.o: src/main.cpp
	$(CXX) $(CPPFLAGS) -c src/main.cpp $(INCLUDES)

app.o: src/app.cpp
	$(CXX) $(CPPFLAGS) -c src/app.cpp $(INCLUDES)

gui.o: src/gui.cpp
	$(CXX) $(CPPFLAGS) -c src/gui.cpp $(INCLUDES)

install:
	cp ./$(PROGRAM) /usr/local/bin

run: all
	./snake

clean:
	rm -f *.o src/*.o $(PROGRAM)

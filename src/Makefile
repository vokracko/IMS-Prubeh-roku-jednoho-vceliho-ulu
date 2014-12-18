FLAGS=-std=c++11 -g -O2
BINARY=simulace

all: build

build: main.o params.o day.o bee.o
	g++ $(FLAGS) *.o -o $(BINARY) -lsimlib -lm


clean:
	rm -rf *.o $(BINARY) *.log 2>/dev/null

%.o: %.cc
	g++ $(FLAGS) -c $< -o $@

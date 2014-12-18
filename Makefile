GNUPLOT := $(shell which gnuplot)
GCC=gcc
all: build

build:
	make -C src && mv ./src/simulace ./

run: build
	./simulace --input experiments/zimovani
	mv experiments/honey.dat experiments/honeyZimovani.dat
	mv experiments/beeCount.dat experiments/beeCountZimovani.dat
	./simulace --input experiments/vykaceni
	mv experiments/honey.dat experiments/honeyVykaceni.dat
	mv experiments/beeCount.dat experiments/beeCountVykaceni.dat
	@which gnuplot 2>/dev/null && make graph-vykaceni && make graph-zimovani || echo "Generování grafů vyžaduje gnuplot"

graph-zimovani:
	gnuplot experiments/zimovani.plt
	gnuplot experiments/beeCountZimovani.plt

graph-vykaceni:
	gnuplot experiments/vykaceni.plt
	gnuplot experiments/beeCountVykaceni.plt

clean:
	rm -rf simulace *.png 2>/dev/null
	make -C src clean

pack: clean
	zip 03_xvokra00_xvesel63.zip -r src/* experiments/* studie.pdf Makefile README



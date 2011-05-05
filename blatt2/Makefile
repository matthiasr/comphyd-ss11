CFLAGS?= -std=c99 -I/usr/local/include -I/usr/X11/include/libpng14 -fopenmp
LDFLAGS?= -L/usr/local/lib -L/usr/X11/lib -lpng14 -fopenmp

all: aufg2.png

aufg2: aufg2.o

aufg2.png: aufg2
	./aufg2 aufg2.png

.PHONY: clean

clean:
	rm -f *.o
	rm -f aufg2 aufg2.png

mem: mem.c mem.h
	gcc -g -c -Wall -m32 -fpic mem.c
	gcc -shared -Wall -m32 -o libmem.so mem.o

clean:
	rm -rf mem.o libmem.so

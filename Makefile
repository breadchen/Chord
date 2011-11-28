test: key.o identifier.o test.c
	gcc -g test.c key.o identifier.o -lssl -lcrypto -o test
key.o: key.c key.h
	gcc -g -c key.c
identifier.o: identifier.c identifier.h
	gcc -g -c identifier.c

clean:
	rm -rf key.o identifier.o test

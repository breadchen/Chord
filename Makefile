test: key.o test.c
	gcc -g test.c key.o -lssl -lcrypto -o test
key.o: key.c
	gcc -g -c key.c

clean:
	rm -rf key.o

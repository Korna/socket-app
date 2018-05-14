.PHONY: clean prepare functions test

bin/udpserver: prepare obj/receiver.o obj/functions.o
	gcc -o bin/udpserver obj/receiver.o obj/functions.o

bin/test: prepare obj/functions.o obj/functest.o obj/cmain.o
	gcc -Wall -o bin/test obj/functions.o obj/functest.o obj/cmain.o

obj/functest.o: prepare test/functest.c
	gcc -Wall -c test/functest.c -o obj/functest.o -Ithirdparty -Isrc

obj/cmain.o: prepare test/main.c
	gcc -Wall -c test/main.c -o obj/cmain.o -Ithirdparty


obj/receiver.o: prepare src/receiver.c
	gcc -c src/receiver.c -o obj/receiver.o

obj/functions.o: prepare src/functions.c
	gcc -c src/functions.c -o obj/functions.o

	
clean:
	rm -f bin/* 
	rm -f obj/*
	
functions: bin/udpserver
	bin/udpserver 
	
prepare: bin obj

bin:
	mkdir bin

test: bin/test
	bin/test

obj:
	mkdir obj
all: main

main: src/main.o src/login.o
		gcc -o main src/main.o src/login.o -Iinclude

src/main.o: src/main.c
		gcc -c src/main.c -Iinclude

src/login.o: src/login.c
		gcc -c src/login.c -Iinclude

clean:
		rm -f src/*.o main

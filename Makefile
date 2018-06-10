all: socket.o main.o compile clean

compile:
	@g++ *.o -o exe

fullcompile:
	@g++ socket.o main.o -o exe  -g -Wall

main.o:
	@g++ -c main.cpp

socket.o:
	@g++ -c socket.cpp

run:
	./exe

fullrun:
	valgrind -v --track-origins=yes --leak-check=full --show-leak-kinds=all ./exe

clean:
	@rm *.o

zip:
	zip -r socket *.c *.h Makefile *.md

 
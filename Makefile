all: socket.o tiktaktoe.o main.o client.o compile clean

compile:
	@g++ socket.o main.o tiktaktoe.o -o exe -g -Wall
	@g++ socket.o client.o tiktaktoe.o -o exe2 -g -Wall

fullcompile:
	@g++ socket.o main.o -o exe  -g -Wall

main.o:
	@g++ -c main.cpp

client.o:
	@g++ -c client.cpp

socket.o:
	@g++ -c socket.cpp

tiktaktoe.o:
	@g++ -c tiktaktoe.cpp

run1:
	./exe

run2:
	./exe2

fullrun:
	valgrind -v --track-origins=yes --leak-check=full --show-leak-kinds=all ./exe

clean:
	@rm *.o

zip:
	zip -r socket *.c *.h Makefile *.md

 
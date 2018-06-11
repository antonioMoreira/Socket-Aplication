all: socket.o tictactoe.o server.o client.o compile clean

compile:
	@g++ socket.o server.o tictactoe.o -o exe -g -Wall
	@g++ socket.o client.o tictactoe.o -o exe2 -g -Wall

fullcompile:
	@g++ socket.o server.o -o exe  -g -Wall

server.o:
	@g++ -c server.cpp

client.o:
	@g++ -c client.cpp

socket.o:
	@g++ -c socket.cpp

tictactoe.o:
	@g++ -c tictactoe.cpp

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
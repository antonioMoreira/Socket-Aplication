export PORT_NUMBER
export IP_ADDR

all: socket.o tictactoe.o server.o client.o compile clean

compile:
	@g++ socket.o server.o tictactoe.o -o exe1 -g -Wall
	@g++ socket.o client.o tictactoe.o -o exe2 -g -Wall

server.o:
	@g++ -c server.cpp

client.o:
	@g++ -c client.cpp

socket.o:
	@g++ -c socket.cpp

tictactoe.o:
	@g++ -c tictactoe.cpp

server:
	@read -p "Enter port number: " PORT_NUMBER ; \
	echo "You chose port number $$PORT_NUMBER" ; \
	./exe1 $$PORT_NUMBER
		
client:
	@read -p "Enter port number: " PORT_NUMBER ; read -p "Enter IP addr: " IP_ADDR ; \
	echo "You chose port number $$PORT_NUMBER" ; echo "You chose $$IP_ADDR IP addr" ; \
	./exe2 $$PORT_NUMBER $$IP_ADDR


fullrun:
	valgrind -v --track-origins=yes --leak-check=full --show-leak-kinds=all ./exe

clean:
	@rm *.o

zip:
	zip -r Socket *.cpp *.h Makefile *.md *.pdf
DATA_1=$(shell read DATA_1)

all: socket.o server.o client.o compile clean

compile:
	@g++ socket.o server.o -o exe1 -g -Wall
	@g++ socket.o client.o -o exe2 -g -Wall

fullcompile:
	@g++ socket.o server.o -o exe  -g -Wall

server.o:
	@g++ -c server.cpp

client.o:
	@g++ -c client.cpp

socket.o:
	@g++ -c socket.cpp

run1:
	@#@ echo "Please enter port: \t" ;  ; echo "Your port is: $(DATA_1)"
	@ ./exe $(DATA_1)
run2:
	@ echo "Please enter port number: \t" 
	@ read DATA_1
	@ echo "Please enter ip addr: \t"
	@ read DATA_2
	@ ./exe2 $(DATA_1) $(DATA_2)

fullrun:
	valgrind -v --track-origins=yes --leak-check=full --show-leak-kinds=all ./exe

clean:
	@rm *.o

zip:
	zip -r socket *.c *.h Makefile *.md

 
CC = g++
FILES=main.cpp Client.cpp SenderServer.cpp ReceiverServer.cpp ThreadConnection.cpp MessagePool.cpp Contact.cpp Group.cpp conio.c

exemplo: clean
	$(CC) -std=c++11 $(FILES) -lpthread -o trab

clean:
	- rm trab

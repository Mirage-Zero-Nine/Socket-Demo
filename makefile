all: 
	gcc -Wall Server_A.c -o serverA -lm
	gcc -Wall Server_B.c -o serverB -lm
	gcc -Wall Server_C.c -o serverC -lm
	gcc -Wall Main_Server.c -o server
	gcc -Wall client.c -o client

client:
	gcc -Wall client.c -o client


# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
# Reference: https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

serverA : 
	./serverA
serverB : 
	./serverB
serverC : 
	./serverC
server: 
	./server

.PHONY: clean serverA serverB serverC server all
clean:
	$(RM) serverA
	$(RM) serverB
	$(RM) serverC
	$(RM) client
	$(RM) server
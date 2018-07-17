all: 
	gcc -Wall serverA.c -o serverA -lm
	gcc -Wall serverB.c -o serverB -lm
	gcc -Wall serverC.c -o serverC -lm
	gcc -Wall aws.c -o aws
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
aws: 
	./aws

.PHONY: clean serverA serverB serverC aws all
clean:
	$(RM) serverA
	$(RM) serverB
	$(RM) serverC
	$(RM) client
	$(RM) aws
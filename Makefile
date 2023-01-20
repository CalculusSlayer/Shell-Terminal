#WARNINGS = -Wall -Wextra

all: sshell

sshell: sshell.o
	gcc $(WARNINGS) -o sshell sshell.o

sshell.o: sshell.c
	gcc $(WARNINGS) -c sshell.c

clean:
	rm -f *.o sshell

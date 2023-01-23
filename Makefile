# Makefile

CFLAGS = -Wall -Wextra -Wextra
CFLAGS += -g
#CFLAGS += -O2
EXEC1 = sshell
DEPENDENCIES = token.o

all: $(EXEC1)

$(EXEC1): $(EXEC1).o $(DEPENDENCIES)
	gcc $(CFLAGS) -o $(EXEC1) $(EXEC1).o $(DEPENDENCIES)

$(EXEC1).o: $(EXEC1).c token.h
	gcc $(CFLAGS) -c $(EXEC1).c 

token.o: token.c token.h
	gcc $(CFLAGS) -c token.c

#$(EXEC2): $(EXEC2).o
#	gcc $(WARNINGS) -o $(EXEC2) $(EXEC2).o $(DEBUG)

#$(EXEC2).o: $(EXEC2).c
#	gcc $(WARNINGS) -c $(EXEC2).c $(DEBUG)

clean:
	rm -f *.o $(EXEC1)

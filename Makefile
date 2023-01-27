# Makefile

CFLAGS = -Wall -Wextra -Wextra
CFLAGS += -g
#CFLAGS += -O2
EXEC1 = sshell
DEPENDENCIES = token.o linked_list.o
HEADERS = token.h linked_list.h sshell.h 

all: $(EXEC1) token_test linked_list_test

$(EXEC1): $(EXEC1).o $(DEPENDENCIES) 
	gcc $(CFLAGS) -o $(EXEC1) $(EXEC1).o $(DEPENDENCIES)

$(EXEC1).o: $(EXEC1).c $(HEADERS)
	gcc $(CFLAGS) -c $(EXEC1).c 

token.o: token.c token.h $(HEADERS)
	gcc $(CFLAGS) -c token.c

token_test: token_test.o token.o linked_list.o
	gcc $(CFLAGS) -o token_test token_test.o token.o linked_list.o

token_test.o: token_test.c $(HEADERS)
	gcc $(CFLAGS) -c token_test.c

linked_list_test: linked_list_test.o linked_list.o
	gcc $(CFLAGS) -o linked_list_test linked_list_test.o linked_list.o

linked_list_test.o: linked_list_test.c $(HEADERS)
	gcc $(CFLAGS) -c linked_list_test.c

linked_list.o: linked_list.c $(HEADERS)
	gcc $(CFLAGS) -c linked_list.c

clean:
	rm -f *.o $(EXEC1) token_test linked_list_test

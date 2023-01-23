# Makefile

WARNINGS = -Wall -Wextra -Wextra
DEBUG =
DEBUG += -g
EXEC1 = sshell
EXEC2 = token

all: $(EXEC1) $(EXEC2)

$(EXEC1): $(EXEC1).o
	gcc $(WARNINGS) -o $(EXEC1) $(EXEC1).o $(DEBUG)

$(EXEC1).o: $(EXEC1).c
	gcc $(WARNINGS) -c $(EXEC1).c $(DEBUG)

#$(EXEC2): $(EXEC2).o
#	gcc $(WARNINGS) -o $(EXEC2) $(EXEC2).o $(DEBUG)

#$(EXEC2).o: $(EXEC2).c
#	gcc $(WARNINGS) -c $(EXEC2).c $(DEBUG)

clean:
	rm -f *.o $(EXEC1) $(EXEC2)

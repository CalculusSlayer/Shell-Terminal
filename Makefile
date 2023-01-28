#--------------------------------------------------------------------
# Nayeel Imtiaz
# Ky Ngo
# ECS 150, Project 1
#
# Makefile
#
# make				makes sshell, token_test, and linked_list_test
# make sshell			makes sshell
# make token_test		makes token_test
# make linked_list_test		makes linked_list_test
# make clean			Removes all objects files and executables
#---------------------------------------------------------------------

CC = gcc
CFLAGS = -Wall -Wextra -Wextra

# Uncomment Line below to add debug flag
CFLAGS += -g

# Uncomment line below to add compilation optimization flag
#CFLAGS += -O2

HEADERS = $(wildcard *.h)
DEPENDENCIES = token.o linked_list.o generalList.o

.PHONY: all
all: sshell token_test linked_list_test generalList_test

# $@ refers to target name (left side)
# $^ refers to ALL the prereqs (right side)
sshell: sshell.o $(DEPENDENCIES) 
	$(CC) $(CFLAGS) -o $@ $^

token_test: token_test.o token.o linked_list.o
	$(CC) $(CFLAGS) -o $@ $^ 

linked_list_test: linked_list_test.o linked_list.o
	$(CC) $(CFLAGS) -o $@ $^

generalList_test: generalList_test.o $(DEPENDENCIES)

# $< refers to first prereq (right side)
%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o sshell token_test linked_list_test generalList_test

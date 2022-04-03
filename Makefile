BASEDIR = $(shell pwd)

BATSDIR = $(BASEDIR)/bats
BATS_INCDIR = $(BATSDIR)/inc
BATS_SRCDIR = $(BATSDIR)/src
BATS_SRC = $(wildcard $(BATS_SRCDIR)/*.cpp)
BATS_SRC_O = $(patsubst %.c, %.o, $(BATS_SRC))
BATS_INC_FLAG = -I $(BATS_INCDIR) 

CC = g++
CFLAGS = -g -Wall

bats_sim: $(BATS_SRC_O)
	$(CC) $(CFLAGS) -o $@ $^ $(BATS_INC_FLAG)

scheduler.o: scheduler.h

task.o: task.h

clean:
	rm bats_sim

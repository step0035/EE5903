BASEDIR = $(shell pwd)

BATSDIR = $(BASEDIR)/bats
BATS_INCDIR = $(BATSDIR)/inc
BATS_SRCDIR = $(BATSDIR)/src
BATS_SRC = $(wildcard $(BATS_SRCDIR)/*.cpp)
BATS_SRC_O = $(patsubst %.c, %.o, $(BATS_SRC))
BATS_INC_FLAG = -I $(BATS_INCDIR) 

FTPARTDIR = $(BASEDIR)/ftpart
FTPART_INCDIR = $(FTPARTDIR)/inc
FTPART_SRCDIR = $(FTPARTDIR)/src
FTPART_SRC = $(wildcard $(FTPART_SRCDIR)/*.cpp)
FTPART_SRC_O = $(patsubst %.c, %.o, $(FTPART_SRC))
FTPART_INC_FLAG = -I $(FTPART_INCDIR) 

CC = g++
CFLAGS = -g -Wall -Wno-unused-but-set-variable

all: bats ftpart

bats: bats_sim

ftpart: ftpart_sim

bats_sim: $(BATS_SRC_O)
	$(CC) $(CFLAGS) -o $@ $^ $(BATS_INC_FLAG)

ftpart_sim: $(FTPART_SRC_O)
	$(CC) $(CFLAGS) -o $@ $^ $(FTPART_INC_FLAG)

clean:
	rm bats_sim
	rm ftpart_sim

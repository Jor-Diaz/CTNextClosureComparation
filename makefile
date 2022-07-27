CFLAGS = -std=c++11 -msse4.2 -O3 -march=native
CC = g++
SDSLFLAGS = -DNDEBUG -I ~/include -L ~/lib -lsdsl -ldivsufsort -ldivsufsort64
TESTDIR = executables
RANKTYPE=v
RUNS=t

all:
	$(CC) $(CFLAGS)  nextclosure-bit_vector.cpp -o $(TESTDIR)/nx_bitvector.out $(SDSLFLAGS)
	$(CC) $(CFLAGS)  nextclosure-bool_array.cpp -o $(TESTDIR)/nx_boolarray.out $(SDSLFLAGS)	
	$(CC) $(CFLAGS)  nextclosure-compressed-binary-tree.cpp -o $(TESTDIR)/nx_cmbt.out $(SDSLFLAGS) -pthread
	#$(TESTDIR)/test.out  --rank $(RANKTYPE) --runs $(RUNS) 


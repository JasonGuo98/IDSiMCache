CC=g++

ICacheDIR = ./cache/include 
IDSDIR = ./datastructure/include
IUtilsDIR = ./utils
ITLDIR = ./traceloader/include
IProfDIR = ./profiler/include
CFLAGS=-I$(ICacheDIR) -I$(IDSDIR) -I$(IUtilsDIR) -I$(ITLDIR) -I$(IProfDIR) -std=c++11 -g


all:  testNormalTrace testLRU testMRC
# all:  testNormalTrace testLRU
	echo "make all"


testMRC: testMRC.o mrc_sim.o normal_trace.o LRU.o  _list.o list_node.o list_iterator.o
	$(CC) -o $@ $^ $(CFLAGS)

testMRC.o: testMRC.cc
	$(CC) -c -o $@ $< $(CFLAGS)

mrc_sim.o: ./profiler/mrc_sim.cc
	$(CC) -c -o $@ $< $(CFLAGS)

testNormalTrace: normal_trace.o testNormalTrace.o
	$(CC) -o $@ $^ $(CFLAGS)

normal_trace.o: ./traceloader/normal_trace.cc
	$(CC) -c -o $@ $< $(CFLAGS)

testNormalTrace.o: ./testNormalTrace.cc
	$(CC) -c -o $@ $< $(CFLAGS)

testLRU: testLRU.o LRU.o  _list.o list_node.o list_iterator.o
# testLRU: testLRU.o LRU.o  list.o
	$(CC) -o $@ $^ $(CFLAGS)

testLRU.o: testLRU.cc
	$(CC) -c -o $@ $< $(CFLAGS)


LRU.o: ./cache/LRU.cc
	$(CC) -c -o $@ $^ $(CFLAGS)

# list.o: ./datastructure/list.c ./datastructure/list_node.c  ./datastructure/list_iterator.c
# 	gcc -shared -lc  $^ -o$@  $(CFLAGS)

_list.o: ./datastructure/list.c
	$(CC) -c -o $@ $^ $(CFLAGS)

list_node.o: ./datastructure/list_node.c 
	$(CC) -c -o $@ $^ $(CFLAGS)
	
list_iterator.o: ./datastructure/list_iterator.c
	$(CC) -c -o $@ $^ $(CFLAGS)

clean:
	rm *.o
	rm testLRU testNormalTrace testMRC
CC=g++

ICacheDIR = ./cache/include 
IDSDIR = ./datastructure/include
IUtilsDIR = ./utils
ITLDIR = ./traceloader/include
IProfDIR = ./profiler/include
IAnalDIR = ./analyzer/include
IPython = /usr/include/python3.8
INumpy = /usr/include/python3.8/numpy
IWrapper = ./pywrapper/include
CFLAGS=-I$(ICacheDIR) -I$(IDSDIR) -I$(IUtilsDIR) -I$(ITLDIR) -I$(IProfDIR) -I$(IAnalDIR) -I$(INumpy) -I$(IPython) -I$(IWrapper) -std=c++11 -g -fPIC -Wall




all:  testNormalTrace testLRU testMRC testBasicinfo c_MrcSim.so
# all:  testNormalTrace testLRU
	echo "make all"

c_MrcSim.so: py_mrc_sim_wrapper.o mrc_sim.o normal_trace.o LRU.o _list.o list_node.o list_iterator.o
	$(CC) -fPIC -shared $^ -o $@  $(CFLAGS) 
	# $(CC) $^ -c -o $@  $(CFLAGS) 
	# $(CC) -Xlinker -export-dynamic -fPIC $^ -o $@ -shared  $(CFLAGS) 

py_mrc_sim_wrapper.o: ./pywrapper/py_mrc_sim_wrapper.cc
	$(CC) -fPIC -c -o $@ $< $(CFLAGS)

testBasicinfo: normal_trace.o testBasicinfo.o trace_basic_anal.o
	$(CC) -o $@ $^ $(CFLAGS)


testBasicinfo.o: testBasicinfo.cc
	$(CC) -c -o $@ $< $(CFLAGS)

trace_basic_anal.o: ./analyzer/trace_basic_anal.cc
	$(CC) -c -o $@ $< $(CFLAGS)
	


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

.PHONY:clean all
clean:
	rm *.o
	rm testLRU testNormalTrace testMRC
	rm c_MrcSim.so
	rm all
FLEX = flex
CXX = clang++
CXXFLAGS = -g -c -std=c++11


objects = driver.o parser.o scanner.o ke_struct.o Rete_Network.o construct_rete.o \
		solver.o

# 如果bison文件有更新，需要先执行 bison -o parser.cc parser.yy 再执行 make

solver : $(objects)
	$(CXX) -o $@ $(objects)

%.cc: %.ll
	$(FLEX) $(FLEXFLAGS) -o$@ $<

driver.o : driver.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

parser.o : parser.cc parser.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

scanner.o : scanner.cc parser.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

ke_struct.o : ke_struct.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

Rete_Network.o : Rete_Network.cc Rete_Network.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

construct_rete.o : construct_rete.cc Rete_Network.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

solver.o : solver.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

.PHONY : clean

clean :
	rm solver scanner.cc $(objects)
BISON = bison
FLEX = flex
CXX = clang++
CXXFLAGS = -g -c -std=c++11


objects = driver.o parser.o scanner.o ke_struct.o solver.o

solver : $(objects)
	$(CXX) -o $@ $(objects)


%.cc %.hh: %.yy driver.hh ke_struct.hh
	$(BISON) $(BISONFLAGS) -o $*.cc $<

%.cc: %.ll
	$(FLEX) $(FLEXFLAGS) -o$@ $<

driver.o : driver.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

parser.o : parser.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

scanner.o : scanner.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

ke_struct.o : ke_struct.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

solver.o : solver.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

.PHONY : clean
clean :
	rm solver
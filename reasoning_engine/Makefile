BISON = bison
FLEX = flex
CXX = clang++
CXXFLAGS = -g -c -std=c++11
GRAPHVIZ_INCLUDE = -I /opt/homebrew/include
GRAPHVIZ_DYLIB = -L /opt/homebrew/lib -lcgraph -lgvc

objects = driver.o parser.o scanner.o ke_struct.o Rete_Network.o construct_rete.o \
		global_var_def.o draw_rete_network.o eval.o reasoning.o solver.o


solver : $(objects)
	$(CXX) $(GRAPHVIZ_DYLIB) -o $@ $(objects)

%.cc %.hh: %.yy driver.hh ke_struct.hh
	$(BISON) $(BISONFLAGS) -o $*.cc $<

%.cc: %.ll
	$(FLEX) $(FLEXFLAGS) -o$@ $<

driver.o : driver.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

parser.o : parser.cc parser.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

scanner.o : scanner.cc parser.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

ke_struct.o : ke_struct.cc ke_struct.hh Rete_Network.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

Rete_Network.o : Rete_Network.cc Rete_Network.hh ke_struct.hh
	$(CXX) $(CXXFLAGS) $(GRAPHVIZ_INCLUDE) -o$@ $<

construct_rete.o : construct_rete.cc Rete_Network.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

global_var_def.o : global_var_def.cc global_var_decl.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

draw_rete_network.o : draw_rete_network.cc
	$(CXX) $(CXXFLAGS) $(GRAPHVIZ_INCLUDE) -o$@ $<

eval.o : eval.cc eval.hh
	$(CXX) $(CXXFLAGS) -o$@ $<

reasoning.o : reasoning.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

solver.o : solver.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

.PHONY : clean

clean :
	rm solver scanner.cc $(objects)
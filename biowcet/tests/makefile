PROGRAM         = unit_test
SOURCES         = tests.cpp

SOURCES += /home/OTAWA-Studies/biowcet/src/cfgmatrix/CfgMatrix.cpp
SOURCES += /home/OTAWA-Studies/biowcet/src/cfgmatrix/SparseMatrix.cpp
SOURCES += /home/OTAWA-Studies/biowcet/src/wipet/wcet_ipet.cpp
SOURCES += /home/OTAWA-Studies/biowcet/src/wbio/wcet_bio.cpp
SOURCES += /home/OTAWA-Studies/biowcet/src/wdyn/wcet_dyn.cpp
SOURCES += /home/OTAWA-Studies/biowcet/src/utils/utilities.cpp
SOURCES += /home/OTAWA-Studies/biowcet/src/aco/aco.cpp

MICROARCH	      = trivial

# Caminho para a pasta do Google Test
GTEST_DIR = /usr/src/googletest/googletest/

# Configuration
FLAGSA          = $(shell otawa-config --cflags) -I$(GTEST_DIR)/include
DATADIR         = $(shell otawa-config --datadir)
CXXFLAGS        = -DDATA_DIR="$(DATADIR)"
LIBDIR          = $(shell otawa-config --libdir)
LDLIBS          = $(shell otawa-config --libs) $(LIBDIR)/otawa/otawa/etime.so $(LIBDIR)/otawa/otawa/branch.so -L$(GTEST_DIR)/lib -lgtest -lgtest_main
CFLAGS          = -std=c++11 
LDFLAGS         = -dlopen force 
CXX             = g++
LD              = libtool --mode=link --tag=CXX g++ 

# Derivated
OBJECTS         = $(SOURCES:.cpp=.o)

# Rules
all: $(PROGRAM) remove_o
	export LD_LIBRARY_PATH=$(LIBDIR):$(LIBDIR)/otawa/otawa

$(PROGRAM): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS) 

%.o: %.cpp
	$(CXX) $(FLAGSA) $(CXXFLAGS) $(CFLAGS) -c $< -o $@ 

remove_o:
	@rm -rf src/cfgmatrix/*.o  src/wipet/*.o src/wbio/*.o src/wdyn/*.o *.o src/utils/*.o src/aco/*.o

clean: remove_o
	@rm -rf $(PROGRAM) *~ core.* *.lo .libs csv dot png 

# Alvo para executar os testes
test: $(PROGRAM)
	./$(PROGRAM) --gtest_output=xml:TestResults.xml # Substitua o comando pelo comando de execução de seus testes


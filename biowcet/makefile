PROGRAM         = wcet
SOURCES         = main.cpp

SOURCES += cfgmatrix/CfgMatrix.cpp
SOURCES += cfgmatrix/SparseMatrix.cpp
SOURCES += wipet/wcet_ipet.cpp
SOURCES += wbio/wcet_bio.cpp


TEST_FOLDER        = test_files
TEST_TARGET_BS     = $(TEST_FOLDER)/bs/bs.elf
TEST_TARGET_BS100  = $(TEST_FOLDER)/bsort100/bsort100.elf
TEST_TARGET_IF     = $(TEST_FOLDER)/if/if.elf
TEST_TARGET_FORIF  = $(TEST_FOLDER)/forif/forif.elf
TEST_TARGET_FUNC   = $(TEST_FOLDER)/func/func.elf
TEST_TARGET_LUDCMP = $(TEST_FOLDER)/ludcmp/ludcmp.elf

MICROARCH	      = trivial
#MICROARCH	      = lpc2138

# Configuration
FLAGSA          = $(shell otawa-config --cflags)
DATADIR         = $(shell otawa-config --datadir)
CXXFLAGS        = -DDATA_DIR="$(DATADIR)"
LIBDIR          = $(shell otawa-config --libdir)
LDLIBS          = $(shell otawa-config --libs) $(LIBDIR)/otawa/otawa/etime.so $(LIBDIR)/otawa/otawa/branch.so
CFLAGS          = -std=c++11 
LDFLAGS         = -dlopen force 
CXX             = g++
LD              = libtool --mode=link --tag=CXX g++ 

# Derivated
OBJECTS         = $(SOURCES:.cpp=.o)

# Rules
all: $(PROGRAM)
	export LD_LIBRARY_PATH=$(LIBDIR):$(LIBDIR)/otawa/otawa

$(PROGRAM): $(OBJECTS) cfgmatrix/SparseMatrix.o cfgmatrix/CfgMatrix.o wipet/wcet_ipet.o wbio/wcet_bio.o
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS) 

%.o: %.cpp
	$(CXX) $(FLAGSA) $(CXXFLAGS) $(CFLAGS) -c $< -o $@ 

clean:
	@rm -rf *.o $(PROGRAM) *~ core.* *.lo .libs csv dot png cfgmatrix/*.o cfgmatrix/*.gch wipet/*.o wbio/*.o

if: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_IF)

bs: $(PROGRAM)
	mkff $(TEST_TARGET_BS).elf
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_BS)

bsort100: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_BS100)

forif: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_FORIF)

func: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_FUNC)

ludcmp: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_LUDCMP)

erro: $(PROGRAM)
	./$(PROGRAM) 

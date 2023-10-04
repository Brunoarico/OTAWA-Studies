PROGRAM         = biowcet
SOURCES         = test.cpp


TEST_FOLDER        = test_files
TEST_TARGET_BS     = $(TEST_FOLDER)/bs/bs.elf
TEST_TARGET_BS100  = $(TEST_FOLDER)/bsort100/bsort100.elf
TEST_TARGET_IF     = $(TEST_FOLDER)/if/if.elf
TEST_TARGET_FORIF  = $(TEST_FOLDER)/forif/forif.elf
TEST_TARGET_FUNC   = $(TEST_FOLDER)/func/func.elf

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
DEPS            = $(addprefix .deps,$(SOURCES:.cpp=.d))

# Rules
all: .deps $(PROGRAM)
	export LD_LIBRARY_PATH=$(LIBDIR):$(LIBDIR)/otawa/otawa

$(PROGRAM): $(OBJECTS) cfgmatrix/SparseMatrix.o cfgmatrix/CfgMatrix.o
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.deps:
	mkdir .deps

cfgmatrix/CfgMatrix.o: cfgmatrix/CfgMatrix.cpp cfgmatrix/CfgMatrix.h
	$(CXX) $(FLAGSA) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

cfgmatrix/SparseMatrix.o: cfgmatrix/SparseMatrix.cpp cfgmatrix/SparseMatrix.h
	$(CXX) $(FLAGSA) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -MM -MF .deps/$*.d -c $<
	$(CXX) $(FLAGSA) $(CXXFLAGS) $(CFLAGS) -c $< -o $@ 

-include $(DEPS)

clean:
	rm -rf *.o $(PROGRAM) *~ core.* *.lo .libs .deps .eld csv dot png cfgmatrix/*.o

if: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_IF)

bs: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_BS)

bsort100: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_BS100)

forif: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_FORIF)

func: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_FUNC)
MODULES         =             
PROGRAM         = biocet
SOURCES         = test.cpp


TEST_FOLDER        = test_files
TEST_TARGET_BS     = $(TEST_FOLDER)/bs/bs.elf
TEST_TARGET_BS100  = $(TEST_FOLDER)/bsort100/bsort100.elf
TEST_TARGET_IF     = $(TEST_FOLDER)/if/if.elf
TEST_TARGET_FORIF  = $(TEST_FOLDER)/forif/forif.elf
HARDWRE_FOLDER     = hardware

#MICROARCH	      = trivial
MICROARCH	      = lpc2138

# Configuration
FLAGS           = -I./otawaBase/include/ 
DATADIR         = otawaBase/share
CXXFLAGS        = $(FLAGS) -DDATA_DIR="$(DATADIR)"
LDLIBS          = -L./otawaBase/lib -lotawa -lelm -lgel++ -lgel ./otawaBase/lib/otawa/otawa/etime.so ./otawaBase/lib/otawa/otawa/branch.so
CFLAGS          = -std=c++11 
LDFLAGS         = -dlopen force 
CXX             = g++
LD              = libtool --mode=link --tag=CXX g++ 
LIBRARY_PATH    = ./otawaBase/lib:./otawaBase/lib/otawa/otawa
# Derivated
OBJECTS         = $(SOURCES:.cpp=.o)
DEPS            = $(addprefix .deps,$(SOURCES:.cpp=.d))

# Rules
all: .deps $(PROGRAM)
	export LD_LIBRARY_PATH=./otawaBase/lib:./otawaBase/lib/otawa/otawa

$(PROGRAM): $(OBJECTS)
	$(LD) -o $@ $^ SparseMatrix.h SparseMatrix.cpp CfgMatrix.h CfgMatrix.cpp $(LDFLAGS) $(LDLIBS)

.deps:
	mkdir .deps

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -MM -MF .deps/$*.d -c $<
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@ 

-include $(DEPS)

clean:
	rm -rf *.o $(PROGRAM) *~ core.* *.lo .libs .deps .eld csv dot png

if: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_IF)

bs: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_BS)

bsort100: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_BS100)

forif: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_FORIF)

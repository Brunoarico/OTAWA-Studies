MODULES         =             
PROGRAM         = biocet
SOURCES         = test.cpp


TEST_FOLDER     = test_files
TEST_TARGET_BS     = $(TEST_FOLDER)/bs/bs.elf
TEST_TARGET_BS100  = $(TEST_FOLDER)/bsort100/bsort100.elf
TEST_TARGET_IF     = $(TEST_FOLDER)/if/if.elf
HARDWRE_FOLDER  = hardware

MICROARCH	      = trivial

# Configuration
FLAGS           = -I./otawaBase/include
DATADIR         = otawaBase/share
CXXFLAGS        = $(FLAGS) -DDATA_DIR="$(DATADIR)"
LDLIBS          = -L./otawaBase/lib -lotawa -lelm -lgel++ -lgel
CFLAGS          = -std=c++11 
LDFLAGS         = -dlopen force
CXX             = g++
LD              = libtool --mode=link --tag=CXX g++
LIBRARY_PATH := ./otawaBase/lib
# Derivated
OBJECTS         = $(SOURCES:.cpp=.o)
DEPS            = $(addprefix .deps,$(SOURCES:.cpp=.d))

# Rules
all: .deps $(PROGRAM)
	echo $(LIBRARY_PATH)
	export LD_LIBRARY_PATH=$(LIBRARY_PATH)

$(PROGRAM): $(OBJECTS)
	$(LD) -o $@ $^ SparseMatrix.h SparseMatrix.cpp $(LDFLAGS) $(LDLIBS)

.deps:
	mkdir .deps

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -MM -MF .deps/$*.d -c $<
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@ 

-include $(DEPS)

clean:
	rm -rf *.o $(PROGRAM) *~ core.* *.lo .libs .deps .eld *.csv *.dot *.png
if: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_IF)

bs: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_BS)

bs100: $(PROGRAM)
	./$(PROGRAM) -S -s $(MICROARCH) $(TEST_TARGET_BS100)


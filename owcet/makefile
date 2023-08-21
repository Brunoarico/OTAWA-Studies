MODULES         =             
PROGRAM         = biocet
SOURCES         = test.cpp


TEST_FOLDER     = test_files
TEST_TARGET     = $(TEST_FOLDER)/bs.elf
HARDWRE_FOLDER  = hardware

# Configuration
FLAGS           = -I./otawaBase/include
DATADIR         = otawaBase/share
CXXFLAGS        = $(FLAGS) -DDATA_DIR="$(DATADIR)"
LDLIBS          = -L./otawaBase/lib -lotawa -lelm -lgel++ -lgel
CFLAGS          = -std=c++11 
LDFLAGS         = -dlopen force
CXX             = g++
LD              = libtool --mode=link --tag=CXX g++
LD_LIBRARY_PATH = ./otawaBase/lib
# Derivated
OBJECTS         = $(SOURCES:.cpp=.o)
DEPS            = $(addprefix .deps,$(SOURCES:.cpp=.d))

# Rules
all: .deps $(PROGRAM)
	echo $(LD_LIBRARY_PATH)
	export LD_LIBRARY_PATH
	echo $(LD_LIBRARY_PATH)

$(PROGRAM): $(OBJECTS)
	$(LD) -o $@ $^ SparseMatrix.h SparseMatrix.cpp $(LDFLAGS) $(LDLIBS)

.deps:
	mkdir .deps

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -MM -MF .deps/$*.d -c $<
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@ 

-include $(DEPS)

clean:
	rm -rf *.o $(PROGRAM) *~ core.* *.lo .libs .deps .eld

run: $(PROGRAM)
	./$(PROGRAM) -S -s trivial $(TEST_TARGET)
	# ./$(PROGRAM) -S -s lpc2138 $(TEST_TARGET)


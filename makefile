MODULES         =             
PROGRAM         = ipet
SOURCES         = ipet.cpp msgLogger.cpp


TEST_FOLDER     = test_files
TEST_TARGET     = $(TEST_FOLDER)/bs.elf
HARDWRE_FOLDER  = hardware

# Configuration
FLAGS           = $(shell otawa-config --cflags)
DATADIR         = $(shell otawa-config --datadir)
CXXFLAGS        = $(FLAGS) -DDATA_DIR="$(DATADIR)"
LDLIBS          = $(shell otawa-config --libs)
CFLAGS          = -std=c++11 
LDFLAGS         = -dlopen force
CXX             = g++
LD              = libtool --mode=link --tag=CXX g++
LD_LIBRARY_PATH = $(shell otawa-config --libdir)
# Derivated
OBJECTS         = $(SOURCES:.cpp=.o)
DEPS            = $(addprefix .deps,$(SOURCES:.cpp=.d))

# Rules
all: .deps $(PROGRAM)
	echo $(LD_LIBRARY_PATH)
	export LD_LIBRARY_PATH

$(PROGRAM): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.deps:
	mkdir .deps

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -MM -MF .deps/$*.d -c $<
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@ 

-include $(DEPS)

clean:
	rm -rf *.o $(PROGRAM) *~ core.* *.lo .libs .deps

run: $(PROGRAM)
	./$(PROGRAM) $(TEST_TARGET) $(HARDWRE_FOLDER)

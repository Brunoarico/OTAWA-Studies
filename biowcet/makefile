PROGRAM         = wcet
SOURCES         = main.cpp

SOURCES += src/cfgmatrix/CfgMatrix.cpp
SOURCES += src/cfgmatrix/SparseMatrix.cpp
SOURCES += src/wipet/wcet_ipet.cpp
SOURCES += src/wbio/wcet_bio.cpp
SOURCES += src/wdyn/wcet_dyn.cpp
SOURCES += src/utils/utilities.cpp
SOURCES += src/aco/aco.cpp
SOURCES += src/cfgGen/cfgGen.cpp

TEST_FOLDER        = test_files
TEST_TARGET_BS     = $(TEST_FOLDER)/bs/
TEST_TARGET_BS100  = $(TEST_FOLDER)/bsort100/
TEST_TARGET_IF     = $(TEST_FOLDER)/if/
TEST_TARGET_FORIF  = $(TEST_FOLDER)/forif/
TEST_TARGET_FUNC   = $(TEST_FOLDER)/func/
TEST_TARGET_MATMUL = $(TEST_FOLDER)/matmul/
TEST_TARGET_FIR = $(TEST_FOLDER)/fir/
TEST_TARGET_LOTFUNC = $(TEST_FOLDER)/lotFunc/
TEST_TARGET_NEASTED = $(TEST_FOLDER)/neasted/

MICROARCH	      = m3

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

$(PROGRAM): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS) 

%.o: %.cpp
	$(CXX) $(FLAGSA) $(CXXFLAGS) $(CFLAGS) -c $< -o $@ 

remove_o:
	@rm -rf src/cfgmatrix/*.o  src/wipet/*.o src/wbio/*.o src/wdyn/*.o *.o src/utils/*.o src/aco/*.o src/cfgGen/*.o

clean: remove_o
	@rm -rf $(PROGRAM) *~ core.* *.lo .libs csv dot png 

if: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_IF)/main.c 

bs: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_BS)/main.c 

bsort100: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_BS100)/main.c

forif: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_FORIF)/main.c 

func: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_FUNC)/main.c

matmul: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_MATMUL)/main.c

fir:
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_FIR)/main.c  

neasted:
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_NEASTED)/main.c  

lotfunc:
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_LOTFUNC)/main.c  

ifd: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_IF)/main.c -d

bsd: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_BS)/main.c -d

bsort100d: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_BS100)/main.c -d

forifd: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_FORIF)/main.c -d

funcd: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_FUNC)/main.c -d

matmuld: $(PROGRAM)
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_MATMUL)/main.c -d 

fird:
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_FIR)/main.c -d

neastedd:
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_NEASTED)/main.c -d

lotfuncd:
	./$(PROGRAM) -s $(MICROARCH) -p $(TEST_TARGET_LOTFUNC)/main.c -d 

runAll: if bs bsort100 forif func matmul fir neasted lotfunc

runAlld: ifd bsd bsort100d forifd funcd matmuld fird neastedd lotfuncd

include ../Makefile.inc

CFLAGS:=-I../utils $(CFLAGS) -lpthread -ldl
LDFLAGS:=-L../utils -lCraftUtils $(LDFLAGS)

# HDR are pure headers
HDR=
# OBJ must include a .cpp and .h with same name
OBJ=compiled_network.o#Cifar10.o
OLD_EXE=quantize_params_kh.exe quantize_params_nb.exe
EXE=cifar10_test_accuracy.exe

all: $(EXE)

# Rule for object files, each object must have a header
#$(OBJ): %.o: %.h
#	$(CXX) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

# Rule for executables
$(EXE): %.exe: %.o $(OBJ)
	g++ $^ -o $@ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean: 
	rm -f cifar10_test_accuracy.o *.exe

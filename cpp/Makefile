CC=g++
CFLAGS=-g -std=gnu++11 -I $(HALIDE_INC) -L $(HALIDE_BIN) -lHalide -lpthread -ldl
MINIZIP_LDFLAGS=-lminizip -laes -lz
LDFLAGS=$(MINIZIP_LDFLAGS)

# HDR are pure headers
HDR=InputConvLayer.h Debug.h Timer.h# BitVector.h QuantizeParams.h Layers.h Typedefs.h
# OBJ must include a .cpp and .h with same name
OBJ=DataIO.o ParamIO.o ZipIO.o Common.o # Cifar10.o
EXE=cifar10_inference.exe

all: $(EXE)

# Rule for object files, each object must have a header
$(OBJ): %.o: %.cpp %.h
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# Rule for executables
$(EXE): %.exe: %.o $(OBJ)
	g++ $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o *.exe

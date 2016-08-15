CXX=g++
CFLAGS=-std=gnu++11 -O3
MINIZIP_LDFLAGS=-lminizip -laes -lz
LDFLAGS=$(MINIZIP_LDFLAGS)

AR=ar rcs

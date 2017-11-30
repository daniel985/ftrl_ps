CC  = gcc
CXX = g++
ROOT = .
INCLUDE = -I$(ROOT)/ps-lite/include
LIBS = -L./ps-lite/deps/lib -lstdc++ -lprotobuf-lite -lzmq
SRC = ./src
INC = ./include
BIN = ./bin
RM = rm -rf
MV = mv
CFLAGS   = -Wall -O2 -D_FILE_OFFSET_BITS=64
CXXFLAGS = -std=c++11 -lpthread -Wall -O3 -D_FILE_OFFSET_BITS=64 -DSIZEOF_LOG_INT=8

TARGET = ps_main

all : $(TARGET)
	$(RM) $(BIN)/*
	$(MV) $(TARGET) $(BIN)

ps_main : main.o ps-lite/build/libps.a
	$(CXX) -o ps_main main.o ps-lite/build/libps.a $(LIBS) $(CXXFLAGS)

main.o : $(SRC)/main.cpp $(SRC)/common.h $(SRC)/server.h $(SRC)/work.h $(SRC)/io.h ps-lite/build/libps.a
	$(CXX) -c $(SRC)/main.cpp $(INCLUDE) $(CXXFLAGS)

.PHONY : clean
clean :
	$(RM) $(TARGET) *.o core

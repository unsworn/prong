CC=gcc
CXX=g++

TARGET=deskew

INCLUDE=-I. -I/opt/local/include

YAJL2?=0

CFLAGS=-g $(INCLUDE) -DHAVE_FREEIMAGE=1 -DHAVE_YAJL2=$(YAJL2) -DDEBUG=1 

CXXFLAGS=$(CFLAGS)

LDFLAGS=-L/opt/local/lib -lfreeimage -lyajl

C_SOURCE=$(wildcard *.c)

CPP_SOURCE=$(wildcard *.cpp)

OBJECTS=$(C_SOURCE:.c=.o) $(CPP_SOURCE:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

clean:
	@rm -rf $(OBJECTS) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

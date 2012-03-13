CC=gcc
CXX=g++

TARGET=deskew

INCLUDE=-I. -I/opt/local/include

CFLAGS=-g $(INCLUDE) -DHAVE_FREEIMAGE=1 -DDEBUG=1

CXXFLAGS=$(CFLAGS)

LDFLAGS=-L/opt/local/lib -lfreeimage -lyajl

C_SOURCE=$(wildcard *.c)
CPP_SOURCE=$(wildcard *.cpp)

OBJECTS=$(C_SOURCE:.c=.o) $(CPP_SOURCE:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

test:
	echo $(SOURCES)

clean:
	@rm -rf $(OBJECTS) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

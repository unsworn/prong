CC=gcc
CXX=g++

TARGET=deskew

INCLUDE=-I. -I/opt/local/include

CFLAGS=-g $(INCLUDE) -DHAVE_FREEIMAGE

CXXFLAGS=$(CFLAGS)

LDFLAGS=-L/opt/local/lib -lfreeimage

OBJECTS=\
	list.o \
	options.o \
	imagedata.o \
	analyzer.o \
	main.o

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

clean:
	@rm -rf $(OBJECTS) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

CC=gcc
CXX=g++

TARGET=deskew

INCLUDE=-I. -I/opt/local/include

CFLAGS=-g $(INCLUDE) -DHAVE_FREEIMAGE=1 -DDEBUG=1

CXXFLAGS=$(CFLAGS)

LDFLAGS=-L/opt/local/lib -lfreeimage

OBJECTS=\
	xml.o \
	list.o \
	options.o \
	JSON.o \
	job.o \
	imageutils.o \
	imagedata.o \
	analyzer.o \
	log.o \
	main.o

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

clean:
	@rm -rf $(OBJECTS) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PHONY:clean all
CC=$(CC)
CFLAGS=-Wall -g -lstdc++ -pthread  -w  -L/home/lvxu/work/dbus/lib
#CC=g++
CC=/opt/gcc-arm-linux-gnueabihf-8.3.0/bin/arm-linux-gnueabihf-g++
#CFLAGS=-Wall -g  -pthread  -w
LIBS = -ldbus-1

BIN=back

SUBDIR=$(shell ls -d */)
APPINCLUDE = -I./ -I/home/lvxu/work/dbus/include
ROOTSRC=$(wildcard *.cpp)
ROOTOBJ=$(ROOTSRC:%.cpp=%.o)

SUBSRC=$(shell find $(SUBDIR) -name '*.cpp')
SUBOBJ=$(SUBSRC:%.cpp=%.o)

$(BIN):$(ROOTOBJ) $(SUBOBJ)
	$(CC) $(CFLAGS)   $(APPINCLUDE) $(ROOTOBJ) $(SUBOBJ) -o $(BIN) $(LIBS)
.cpp.o:
	$(CC) $(CFLAGS) $(APPINCLUDE) -c $< -o $@
clean:
	rm -f *.o $(BIN) $(ROOTOBJ) $(SUBOBJ)

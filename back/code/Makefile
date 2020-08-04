.PHONY:clean all
#CC=$(CC)
CFLAGS=-Wall -g -lstdc++ -pthread -l dbus-1 -w
CC=g++
#CFLAGS=-Wall -g  -pthread  -w
LIBS = -l paho-mqtt3c

BIN=back

SUBDIR=$(shell ls -d */)
APPINCLUDE = -I./
ROOTSRC=$(wildcard *.cpp)
ROOTOBJ=$(ROOTSRC:%.cpp=%.o)

SUBSRC=$(shell find $(SUBDIR) -name '*.cpp')
SUBOBJ=$(SUBSRC:%.cpp=%.o)

$(BIN):$(ROOTOBJ) $(SUBOBJ)
	$(CC) $(CFLAGS)   $(APPINCLUDE) $(ROOTOBJ) $(SUBOBJ) -o $(BIN) $(LIBS)
.c.o:
	$(CC) $(CFLAGS) $(APPINCLUDE) -c $< -o $@
clean:
	rm -f *.o $(BIN) $(ROOTOBJ) $(SUBOBJ)

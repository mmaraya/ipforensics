SHELL    = /bin/sh 
SRCDIR   = src/
OBJDIR   = obj/
BINDIR   = bin/
NAME     = ipf
EXE      = $(BINDIR)$(NAME)

CC       = g++
CFLAGS   = -c -Wall -std=c++11 
LDFLAGS  = 

SOURCES  = src/main.cpp src/ip4and6.cpp src/device.cpp src/packet.cpp src/host.cpp
OBJECTS  = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
        
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

CXX = g++
CXXFLAGS =  -O2 -DNDEBUG
DEFS =

SOURCE = \
        lcs.cpp cmatrix.cpp

OBJECT = \
        lcs.o cmatrix.o

EXE = \
        lcs
INCLUDES = -I. -I./include

LIBDIRS = -L./lib

LIBS =  -lpthread

all:
        $(CXX) $(CXXFLAGS) $(DEFS) $(INCLUDES) -c $(SOURCE)
        $(CXX) $(LIBDIRS) $(OBJECT) $(LIBS) $(INCLUDE) -o $(EXE)
        rm *.o

lcs.o:
        lcs.cpp
cmatrix.o:
        cmatrix.cpp
clean:
        rm *.o

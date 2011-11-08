# all
all: lcs

# link
lcs: cmatrix.o lcs.o
	g++ cmatrix.o lcs.o -o lcs

# compile
cmatrix.o: cmatrix.cpp
	g++ -c cmatrix.cpp -o cmatrix.o

lcs.o: lcs.cpp
	g++ -c lcs.cpp -o lcs.o

# clean
clean:
	rm -rf *.o lcs
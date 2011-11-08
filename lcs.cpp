#include "cmatrix.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <vector>

using namespace std;

#define markFunction() { printf("%s: %d,%d\n", __func__, x, y); }

int max(int a, int b) { if(a > b) { return a; } else { return b; } }

char* createString(int dim)
{
	char* buffer = new char[dim+1];
	for(int i = 0; i < dim; i++) {
		int randomNumber = rand() % 26;
		buffer[i] = 97 + randomNumber;
	}
	buffer[dim] = '\0';
	return buffer;
}

typedef struct coordsT {
	int i;
	int j;
	coordsT(int i1, int j1) : i(i1), j(j1) {}
} coords;

typedef struct elementT {
	char m_x;
	char m_y;
	cMatrix& m_c;
	int m_i;
	int m_j;
	int m_threadNumber;
	elementT(char x, char y, cMatrix& c, int i, int j, int threadNumber) : m_x(x), m_y(y), m_c(c), m_i(i), m_j(j), m_threadNumber(threadNumber) {}
} element;

void* computeElement(void* ele)
{
	char x = ((element*)ele)->m_x;
	char y = ((element*)ele)->m_y;
	cMatrix& c = ((element*)ele)->m_c;
	int i = ((element*)ele)->m_i;
	int j = ((element*)ele)->m_j;
	int threadNumber = ((element*)ele)->m_threadNumber;

//	printf("comparing %c and %c, computing c[%d][%d] on thread %d\n", x, y, i, j, threadNumber);

	if(x == y) {
		c(i,j) = c(i-1,j-1)+1;
	}
	else {
		c(i,j) = max(c(i,j-1), c(i-1,j));
	}

	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	int numberOfThreads = 0;
	bool dynamicStrings = false;
	char *x = NULL, *y = NULL;

	// handle command line arguments
	if(argc == 5) {
		// lcs string1 string2 -p thread
		x = argv[1];
		y = argv[2];
		numberOfThreads = atoi(argv[4]);
	}
	else if(argc == 6) {
		// lcs -d dim1 dim2 -p thread
		srand(time(NULL));
		x = createString(atoi(argv[2]));
		y = createString(atoi(argv[3]));
		numberOfThreads = atoi(argv[5]);
		dynamicStrings = true;
	}
	else {
		printf("correct usage:\n lcs <string 1> <string 2> -p <number of threads>\n");
		printf(" lcs -d <length of string 1> <length of string 2> -p <number of threads>\n");
		return 1;
	}

	printf("string 1 = %s\nstring 2 = %s\nnumber of threads = %d\n", x, y, numberOfThreads);
	int x_size = strlen(x)+1;
	int y_size = strlen(y)+1;
	cMatrix c(x_size, y_size);

	// set up thread array
	pthread_t threads[numberOfThreads];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// find the number of wavefronts, then create vectors
	// containing the index pairs of each wavefront
	// ie. - each vector contains all the index pairs for that wavefront
	int numberOfWavefronts = x_size+y_size;
	vector<coords>* wavefronts = new vector<coords>[numberOfWavefronts];
	for(int i = 1; i < x_size; i++) {
		for(int j = 1; j < y_size; j++) {
			(wavefronts[i+j]).push_back(coords(i,j));
		}
	}

	struct timeval start, end;
    long mtime, seconds, useconds;

	// use gettimeofday to compute the time required to calculate c
	gettimeofday(&start, NULL);	
	int threadsInUse = 0;
	for(int i = 2; i < numberOfWavefronts; i++) {
		for(int j = 0; j < wavefronts[i].size(); j++) {
			int index_i = wavefronts[i][j].i;
			int index_j = wavefronts[i][j].j;

			// create an element object that contains pertinent information
			element* ele = new element(x[index_i-1], y[index_j-1], c, index_i, index_j, threadsInUse);
			// spawn a thread to work on that object
			pthread_create(&(threads[threadsInUse]), &attr, computeElement, (void*)ele);
			threadsInUse++;

			// if we have used all the threads in our pool, let them finish before continuing
			if(threadsInUse == numberOfThreads) {
				for(int k = 0; k < numberOfThreads; k++) {
					pthread_join(threads[k], NULL);
				}
				threadsInUse = 0;
			}
		}
		// if we have oustanding threads in use, let them finish before continuing
		// to the next wavefront
		if(threadsInUse > 0) {
			for(int k = 0; k < numberOfThreads; k++) {
				pthread_join(threads[k], NULL);
			}
			threadsInUse = 0;
		}
	}
    gettimeofday(&end, NULL);

	// compute duration of the calculation loop
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    printf("elapsted time: %ld milliseconds\n", mtime);
	printf("lcs: %d\n", c(x_size-1, y_size-1));

	// cleanup
	if(dynamicStrings) {
		delete[] x;
		delete[] y;
	}

	pthread_exit(NULL);
	return 0;
}
#include "cmatrix.h"
#include <stdio.h>
#include <string.h>

cMatrix::cMatrix(unsigned int rows, unsigned int cols)
: m_rows(rows)
, m_cols(cols)
{
	m_data = new int[rows*cols];
	memset(m_data, 0, rows*cols*sizeof(int));
}

cMatrix::~cMatrix()
{
	delete[] m_data;
}

int& cMatrix::operator() (unsigned int row, unsigned int col)
{
	return m_data[m_cols*row + col];
}

int cMatrix::operator() (unsigned int row, unsigned int col) const
{
	return m_data[m_cols*row + col];
}

void cMatrix::print()
{
	printf("%d x %d matrix\n", m_rows, m_cols);
	for(int i = 0; i < m_rows; i++) {
		for(int j = 0; j < m_cols; j++) {
			printf("%d ", m_data[m_cols*i + j]);
		}
		printf("\n");
	}
}
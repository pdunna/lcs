#ifndef CMATRIX_H
#define CMATRIX_H

class cMatrix
{
public:
	cMatrix(unsigned int row, unsigned int col);
	~cMatrix();

	int& operator()(unsigned int row, unsigned int col);
	int operator()(unsigned int row, unsigned int col) const;

	void print();

private:
	int m_rows, m_cols;
	int* m_data;
};

#endif CMATRIX_H
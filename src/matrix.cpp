/***********************************************************************
 *
 * Copyright (C) 2008 Lars Petter Mostad
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/
 
#include "matrix.h"

Matrix::Matrix(const Matrix &M)
{
	rows = M.rows;
	columns = M.columns;
	elements = new double[rows*columns];
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			elements[i*columns + j] = M.elements[i*columns + j];
}

Matrix::Matrix(int rows, int columns)
{
	this->rows = rows;
	this->columns = columns;
	this->elements = new double[rows*columns];
}

Matrix::Matrix(int rows, int columns, double t)
{
	this->rows = rows;
	this->columns = columns;
	this->elements = new double[rows*columns];
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			this->elements[i*columns + j] = t;
}

Matrix::Matrix(int rows, int columns, double elements[])
{
	this->rows = rows;
	this->columns = columns;
	this->elements = new double[rows*columns];
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			this->elements[i*columns + j] = elements[i*columns + j];
}

Matrix::~Matrix()
{
	delete[] elements;
}

double Matrix::getElement(int i, int j) const
{
	return elements[i*columns + j];
}

const Matrix &Matrix::setElement(int i, int j, double val)
{
	elements[i*columns + j] = val;
	return *this;
}

int Matrix::numRows() const
{
	return rows;
}

int Matrix::numCols() const
{
	return columns;
}

Matrix Matrix::Tr() const
{
	Matrix trans(columns,rows);
	for(int i = 0 ; i < rows ; i++)
	{
		for(int j = 0 ; j < columns ; j++)
		{
			trans.setElement(j, i, this->getElement(i, j));
		}
	}
	return trans;
}

const Matrix &Matrix::operator=(const Matrix &M)
{
	if(elements!=0)
		delete[] elements;
	rows = M.rows;
	columns = M.columns;
	elements = new double[rows*columns];
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			elements[i*columns + j] = M.elements[i*columns + j];
	return *this;
}



Matrix operator+(const Matrix &A, const Matrix &B) throw(MatrixError)
{
	if(A.numRows() != B.numRows() || A.numCols() != B.numCols())
		throw MatrixError("Incompatible matrices for operator \"+\"!");

	Matrix C(A.numRows(), A.numCols());
	
	for(int i = 0 ; i < C.numRows() ; i++)
	{
		for(int j = 0 ; j < C.numCols() ; j++)
		{
			C.setElement(i, j, A.getElement(i, j) + B.getElement(i, j));
		}
	}
	
	return C;
}

Matrix operator-(const Matrix &A, const Matrix &B) throw(MatrixError)
{
	if(A.numRows() != B.numRows() || A.numCols() != B.numCols())
		throw MatrixError("Incompatible matrices for operator \"+\"!");
	return A + (double)(-1)*B;
}

Matrix operator*(const double c, const Matrix &A)
{
	Matrix B(A.numRows(), A.numCols());
	
	for(int i = 0 ; i < B.numRows() ; i++)
	{
		for(int j = 0 ; j < B.numCols() ; j++)
		{
			B.setElement(i, j, c * A.getElement(i, j));
		}
	}
	
	return B;
}

Matrix operator*(const Matrix &A, const double c)
{
	return c*A;
}

Matrix operator*(const Matrix &A, const Matrix &B) throw(MatrixError)
{	
	if(A.numCols() != B.numRows())
		throw MatrixError("Incompatible matrices for operator \"*\"!");
		
	Matrix C(A.numRows(), B.numCols());
	
	for(int i = 0 ; i < C.numRows() ; i++)
	{
		for(int j = 0 ; j < C.numCols() ; j++)
		{
			double temp = 0;
			for(int k = 0 ; k < A.numCols() ; k++)
				temp += A.getElement(i,k) * B.getElement(k,j);
			C.setElement(i,j,temp);
		}
	}
	
	return C;
}

Matrix operator^(const Matrix &A, const Matrix &B) throw(MatrixError)
{
	if(A.numRows() != B.numRows() || A.numCols() != B.numCols())
		throw MatrixError("Incompatible matrices for operator \"+\"!");

	Matrix C(A.numRows(), A.numCols());
	
	for(int i = 0 ; i < C.numRows() ; i++)
	{
		for(int j = 0 ; j < C.numCols() ; j++)
		{
			C.setElement(i, j, A.getElement(i, j) * B.getElement(i, j));
		}
	}
	
	return C;
}

Matrix abs(const Matrix &M)
{
	Matrix B(M);
	for(int i = 0 ; i < B.numRows() ; i++)
	{
		for(int j = 0 ; j < B.numCols() ; j++)
		{
			B.setElement(i, j, fabs(B.getElement(i, j)));
		}
	}
	return B;
}

Matrix buildDCT(int N)
{
	Matrix DCT(N,N);
	
	for(int i=0 ; i < N; i++)
	{
		DCT.setElement(i, 0, 1/sqrt(N));
	}
	
	for(int j=1 ; j < N; j++)
	{
		for(int i=0 ; i < N; i++)
		{
			DCT.setElement(i, j, sqrt(2.0/N)*cos(3.1415926*(i+0.5)*j/N));
		}
	}
	
	return DCT;
}

double matrixSum(const Matrix &M)
{
	double sum = 0;
	for(int i = 0; i < M.numRows() ; i++)
	{
		for(int j = 0; j < M.numCols(); j++)
		{
			sum += M.getElement(i,j);
		}
	}
	return sum;
}

double matrixSNR(const Matrix &O, const Matrix &R)
{
	return matrixSum(O^O) / matrixSum((O - R)^(O - R));
}

double matrixPSNR(const Matrix &O, const Matrix &R, double peak)
{
	Matrix P(8,8,peak);
	return matrixSum(P^P) / matrixSum((O - R)^(O - R));
}

Matrix buildZigzag(int N, int n)
{
	// TODO: Generalisere
	if(N!=8 || n>(N*N))
		throw MatrixError("N must be 8 and n must not be grater than N*N!");
	const int x[] = {0, 1, 0, 0, 1, 2, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 5, 6, 7, 7, 6, 7};
	const int y[] = {0, 0, 1, 2, 1, 0, 0, 1, 2, 3, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 4, 5, 6, 7, 7, 6, 5, 6, 7, 7};
	
	Matrix M(N, N, (double)0);
	
	for(int i = 0 ; i < n ; i++)
	{
		M.setElement(y[i], x[i], 1);
	}
	return M;
}

Matrix clampMatrix(Matrix M, double min, double max)
{
	if(min > max)
		return M;
	for(int i = 0; i < M.numRows() ; i++)
	{
		for(int j = 0; j < M.numCols(); j++)
		{
			if(M.getElement(i,j) < min)
				M.setElement(i, j, min);
			else if(M.getElement(i,j) > max)
				M.setElement(i, j, max);
		}
	}
	return M;
}

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

#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>

class MatrixError {
public:
	MatrixError(const char *cause) { this->cause = cause; }
	const char *getCause() { return cause; }
private:
	const char *cause;
};

class Matrix {
public:
	Matrix(const Matrix &M);
	Matrix(int rows, int columns);
	Matrix(int rows, int columns, double t);
	Matrix(int rows, int columns, double elements[]);
	~Matrix();
	double getElement(int i, int j) const;
	const Matrix &setElement(int i, int j, double val);
	int numRows() const;
	int numCols() const;
	Matrix Tr() const;
	const Matrix &operator=(const Matrix &M);
private:
	int rows, columns;
	double *elements;
};

Matrix operator+(const Matrix &A, const Matrix &B) throw(MatrixError);
Matrix operator-(const Matrix &A, const Matrix &B) throw(MatrixError);
Matrix operator*(const double c, const Matrix &A);
Matrix operator*(const Matrix &A, const double c);
Matrix operator*(const Matrix &A, const Matrix &B) throw(MatrixError);
Matrix operator^(const Matrix &A, const Matrix &B) throw(MatrixError);

Matrix abs(const Matrix &M);

Matrix buildDCT(int N);
double matrixSum(const Matrix &M);
double matrixSNR(const Matrix &O, const Matrix &R);
double matrixPSNR(const Matrix &O, const Matrix &R, double peak = 255);
Matrix buildZigzag(int N, int n);
Matrix clampMatrix(Matrix M, double min, double max);

#endif

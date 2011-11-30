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

#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <QWidget>
#include "matrix.h"

class MatrixView : public QWidget
{
	Q_OBJECT
public:
	MatrixView(QWidget *parent = 0);
	
	virtual void paintEvent(QPaintEvent *e);
	virtual QSize sizeHint() const;
public:
	void printMatrix();
	virtual void mousePressEvent(QMouseEvent *e);
public slots:
	void setMatrix(Matrix M);

protected:
	Matrix* displayMatrix;
	Matrix data;
	int pixelScale;
	bool pixelAbs;
	double matrixScale;
};

class TransMatrixView : public MatrixView
{
	Q_OBJECT
public:
	TransMatrixView();
public slots:
	void setMatrix(Matrix M);
protected:
	Matrix trans;
};

class MaskMatrixView : public MatrixView
{
	Q_OBJECT
public:
	MaskMatrixView();
	virtual void mousePressEvent(QMouseEvent *e);
signals:
	void maskChanged(Matrix);
protected:
	Matrix mask;
};

class RecMatrixView : public MatrixView
{
	Q_OBJECT
public:
	RecMatrixView();
public slots:
	void setMatrix(Matrix M);
	void setMask(Matrix M);
protected:
	Matrix trans;
	Matrix mask;
	Matrix rec;
};

class DiffMatrixView : public RecMatrixView
{
	Q_OBJECT
public:
	DiffMatrixView();
public slots:
	void setMatrix(Matrix M);
	void setMask(Matrix M);
protected:
	Matrix diff;
};

#endif

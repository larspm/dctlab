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

#ifndef DCTLABDLG_H
#define DCTLABDLG_H

#include <QtGui>
#include "imageview.h"
#include "matrixview.h"
#include "matrix.h"

class DCTLabDialog : public QDialog
{
	Q_OBJECT
public:
	DCTLabDialog(QString file, QWidget *parent = 0);
public slots:
	void setDataMatrix(Matrix AA);
	void setMaskMatrix(Matrix MM);
	void updateStats();
signals:
	void dataChanged(Matrix);
	void maskChanged(Matrix);
private:
	Matrix A;
	Matrix M;

	QHBoxLayout mainLayout;
	QVBoxLayout mvLayout1;
	QVBoxLayout mvLayout2;
	
	ImageView iv;
	MatrixView orig;
	TransMatrixView trans;
	MaskMatrixView mask;
	RecMatrixView rec;
	DiffMatrixView diff;
	QLabel stats;
};

#endif

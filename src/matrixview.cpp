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

#include "matrixview.h"
#include <QtGui>
#include <QMessageBox>
#include <sstream>

MatrixView::MatrixView(QWidget *parent) : QWidget(parent), data(8,8)
{
	pixelScale = 16;
	matrixScale = 1;
	pixelAbs = false;
	displayMatrix = &data;
}

void MatrixView::setMatrix(Matrix M)
{
	data = M;
	repaint();
}

void MatrixView::printMatrix()
{
	std::stringstream ss;
	for(int i = 0 ; i < displayMatrix->numRows() ; i++)
	{
		for(int j = 0 ; j < displayMatrix->numCols() ; j++)
		{
			ss << displayMatrix->getElement(i, j) << "\t";
		}
		if(i != (displayMatrix->numRows() - 1))
			ss << std::endl;
	}
	
	QLabel *l = new QLabel(ss.str().c_str());
	QHBoxLayout *lo = new QHBoxLayout;
	lo->addWidget(l);
	QDialog *dlg = new QDialog(this);
	dlg->setLayout(lo);
	dlg->show();
}

void MatrixView::mousePressEvent(QMouseEvent *e)
{
	printMatrix();
}

void MatrixView::paintEvent(QPaintEvent *e)
{
	QImage image(size(), QImage::Format_ARGB32);
	QPainter ip(&image);
	
	for(int i = 0 ; i < 8 ; i++)
	{
		for(int j = 0 ; j < 8 ; j++)
		{
			double color = matrixScale * displayMatrix->getElement(i,j);
			
			if(pixelAbs)
				color = abs(color);
			
			if(color < 0)
				image.setPixel(j,i,QColor(0,0,255).rgb());
			else if(color > 255)
				image.setPixel(j,i,QColor(255,0,0).rgb());
			else
				image.setPixel(j,i,QColor(color,color,color).rgb());
		}
	}
	
	ip.end();
	QPainter wp(this);
	wp.scale(pixelScale,pixelScale);
	wp.drawImage(0,0,image);
}

QSize MatrixView::sizeHint() const
{
	return QSize(data.numCols() * pixelScale, data.numRows() * pixelScale);
}

TransMatrixView::TransMatrixView() : MatrixView(), trans(8,8)
{
	displayMatrix = &trans;
	pixelAbs = true;
	matrixScale = 1.0 / 8.0;
}

void TransMatrixView::setMatrix(Matrix M)
{
	data = M;
	Matrix D = buildDCT(8);
	trans = D.Tr() * data * D;
	repaint();
}

MaskMatrixView::MaskMatrixView() : MatrixView(), mask(8,8)
{
	mask = buildZigzag(8, 10);
	displayMatrix = &mask;
	matrixScale = 255.0;
}

void MaskMatrixView::mousePressEvent(QMouseEvent *e)
{
	int xx = e->x()/pixelScale;
	int yy = e->y()/pixelScale;
	
	if(e->button() == Qt::RightButton)
	{
		if(mask.getElement(yy,xx)!=0)
			mask.setElement(yy, xx, 0);
		else
			mask.setElement(yy, xx, 1);
	}
	else
	{
		const int x[] = {0, 1, 0, 0, 1, 2, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 5, 6, 7, 7, 6, 7};
		const int y[] = {0, 0, 1, 2, 1, 0, 0, 1, 2, 3, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 4, 5, 6, 7, 7, 6, 5, 6, 7, 7};
		for(int i = 0 ; i < 64 ; i++)
			if(x[i]==xx && y[i]==yy)
				mask = buildZigzag(8, i+1);
	}
	repaint();
	emit maskChanged(mask);
}

RecMatrixView::RecMatrixView() : MatrixView(), trans(8,8), mask(8,8,1.0), rec(8,8)
{
	displayMatrix = &rec;
}

void RecMatrixView::setMatrix(Matrix M)
{
	data = M;
	Matrix D = buildDCT(8);
	trans = D.Tr() * data * D;
	rec = clampMatrix(D * (trans^mask) * D.Tr(), 0, 255);
	repaint();
}

void RecMatrixView::setMask(Matrix M)
{
	mask = M;
	Matrix D = buildDCT(8);
	rec = clampMatrix(D * (trans^mask) * D.Tr(), 0, 255);
	repaint();
}

DiffMatrixView::DiffMatrixView() : RecMatrixView(), diff(8,8)
{
	displayMatrix = &diff;
	pixelAbs = true;
}

void DiffMatrixView::setMatrix(Matrix M)
{
	RecMatrixView::setMatrix(M);
	diff = data - rec;
	repaint();
}
void DiffMatrixView::setMask(Matrix M)
{
	RecMatrixView::setMask(M);
	diff = data - rec;
	repaint();
}

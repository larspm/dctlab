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

#include "imageview.h"
#include <QtGui>

ImageView::ImageView(QString file, QWidget *parent) : QWidget(parent), image(file), selection(-1,-1)
{}

void ImageView::paintEvent(QPaintEvent *e)
{
	QPainter wp(this);
	wp.drawImage(0,0,image);
	if(selection.x()<0)
		return;
	QPen pen(Qt::red, 1);
	wp.setPen(pen);
	wp.drawRect(selection.x()-5, selection.y()-5, 9, 9);
}

void ImageView::mousePressEvent(QMouseEvent *e)
{
	selection = e->pos();
	Matrix A(8,8);
	for(int i = 0 ; i < 8 ; i++)
	{
		for(int j = 0 ; j < 8 ; j++)
		{
			QColor color(image.pixel(selection.x() + j - 4, selection.y() + i - 4));
			double lum = (color.red() + color.green() + color.blue()) / 3.0;
			A.setElement(i, j, lum); 
		}
	}
	
	emit selectionChanged(A);
	repaint();
}

QSize ImageView::sizeHint() const
{
	return image.size();
}

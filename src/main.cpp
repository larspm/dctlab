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

#include <QtGui>
#include "dctlabdlg.h"

int main(int argc, char* argv[]) 
{ 
	QApplication app(argc, argv);
	
	QString filename = QFileDialog::getOpenFileName(0, "", ".", "Images (*.png *.jpg)");
	if(filename.isEmpty())
		return 0;
	
	DCTLabDialog lab(filename);
	
	lab.show();

	return app.exec();
} 

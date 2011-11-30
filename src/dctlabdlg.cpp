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

#include "dctlabdlg.h"

DCTLabDialog::DCTLabDialog(QString file, QWidget *parent) : QDialog(parent), A(8,8), M(8,8), iv(file)
{
	connect(&mask, SIGNAL(maskChanged(Matrix)),this, SLOT(setMaskMatrix(Matrix)));
	connect(&iv, SIGNAL(selectionChanged(Matrix)), this, SLOT(setDataMatrix(Matrix)));
	connect(this, SIGNAL(dataChanged(Matrix)), &orig, SLOT(setMatrix(Matrix)));
	connect(this, SIGNAL(dataChanged(Matrix)), &trans, SLOT(setMatrix(Matrix)));
	connect(this, SIGNAL(dataChanged(Matrix)), &rec, SLOT(setMatrix(Matrix)));
	connect(this, SIGNAL(maskChanged(Matrix)), &rec, SLOT(setMask(Matrix)));
	connect(this, SIGNAL(dataChanged(Matrix)), &diff, SLOT(setMatrix(Matrix)));
	connect(this, SIGNAL(maskChanged(Matrix)), &diff, SLOT(setMask(Matrix)));
	
	mvLayout1.addWidget(new QLabel("Original:"));
	mvLayout1.addWidget(&orig);
	mvLayout1.addWidget(new QLabel("DCT coefficients:"));
	mvLayout1.addWidget(&trans);
	mvLayout1.addWidget(new QLabel("Mask(click):"));
	mvLayout1.addWidget(&mask);
	mvLayout1.addStretch();
	
	mvLayout2.addWidget(new QLabel("Reconstruction:"));
	mvLayout2.addWidget(&rec);
	mvLayout2.addWidget(new QLabel("Error signal:"));
	mvLayout2.addWidget(&diff);
	mvLayout2.addWidget(&stats);
	mvLayout2.addStretch();
	
	mainLayout.addWidget(&iv);
	mainLayout.addLayout(&mvLayout1);
	mainLayout.addLayout(&mvLayout2);
	setLayout(&mainLayout);
}

void DCTLabDialog::setDataMatrix(Matrix AA)
{
	A = AA;
	emit dataChanged(A);
	updateStats();
}

void DCTLabDialog::setMaskMatrix(Matrix MM)
{
	M = MM;
	emit maskChanged(M);
	updateStats();
}

void DCTLabDialog::updateStats()
{
	Matrix D = buildDCT(8);
	Matrix C = D.Tr() * A * D;
	Matrix rec = clampMatrix(D * (C^M) * D.Tr(), 0, 255);
	Matrix E = abs(A-rec);
	
	double max = E.getElement(0, 0);
	for(int i = 0 ; i < 8 ; i++)
		for(int j = 0 ; j < 8 ; j++)
			if(max < E.getElement(i, j))
				max = E.getElement(i, j); 
	
	stats.setText(
		"Statistics:\n\n"
		"PSNR: " + QString::number(10*log10(matrixPSNR(A,rec)), 'g', 3) + " dB\n"
		"SNR : " + QString::number(10*log10(matrixSNR(A,rec)), 'g', 3) + " dB\n"
		"MAE : " + QString::number(matrixSum(E)/64.0, 'g', 3) + "\n"
		"MaxE: " + QString::number(max, 'g', 3)
		);
}

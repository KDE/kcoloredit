/***************************************************************************
                          loadpalettedlg.cpp  -  description
                             -------------------
    begin                : Sat Jul 8 2000
    copyright            : (C) 2000 by Artur Rataj
    email                : art@zeus.polsl.gliwice.pl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <string.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qpushbutton.h>
#include <klocale.h>
#include <kstddirs.h>
#include <kpalette.h>
#include <kfiledialog.h>

#include "main.h"
#include "palette.h"
#include "loadpalettedlg.h"

LoadPaletteDlg::LoadPaletteDlg() : KDialog(0, "", TRUE) {
	fileName = "";
	QVBoxLayout* topLayout = new QVBoxLayout(this, 8);
	QLabel* label = new QLabel(i18n( "Select a palette" ), this);
	topLayout->addWidget(label);
	paletteBox = new QComboBox(false, this);
	browseFileNameInserted = false;
	QStringList palettesList = Palette::getKDEPalettes();
	for(QStringList::Iterator palette = palettesList.begin();
		palette != palettesList.end(); ++palette) {
		bool prepend = (*palette).contains( "colors/Custom_Colors" );
		QString fileName = locate("config", (*palette));
		if(prepend) {
			palettesFileNames.prepend(fileName);
			setFileName(&fileName);
		}	else {
			palettesFileNames.append(fileName);
			if(palette == palettesList.begin())
				setFileName(&fileName);
		}
		QString paletteName = (*palette).mid(strlen( palettesDir ) + strlen( "/" ));
		if(paletteName == "Custom_Colors")
			paletteName = i18n("Custom Colors");
		else if(paletteName == "Recent_Colors")
			paletteName = i18n("Recent Colors");
		if(prepend)
			paletteBox->insertItem(paletteName, 0);
		else
			paletteBox->insertItem(paletteName);
	}
	connect(paletteBox, SIGNAL( activated(int) ), SLOT( setFileName(int) ));
	topLayout->addWidget(paletteBox);
	QHBoxLayout* browseLayout = new QHBoxLayout(4);
	QPushButton* browseButton = new QPushButton(i18n( "Browse" ), this);
	connect(browseButton, SIGNAL( clicked() ), SLOT( browseFileNames() ));
	browseLayout->addWidget(browseButton);
	browseLayout->addStretch(10);
	topLayout->addLayout(browseLayout);
	QHBoxLayout* buttonsLayout = new QHBoxLayout(4);
	QPushButton* okButton = new QPushButton(i18n( "OK" ), this);
	connect(okButton, SIGNAL( clicked() ), SLOT( accept() ));
	buttonsLayout->addStretch(10);
	buttonsLayout->addWidget(okButton, 10);
	buttonsLayout->addStretch(10);
	QPushButton* cancelButton = new QPushButton(i18n( "Cancel" ), this);
	connect(cancelButton, SIGNAL( clicked() ), SLOT( reject() ));
	buttonsLayout->addWidget(cancelButton, 10);
	buttonsLayout->addStretch(10);
	topLayout->addStretch(10);
	topLayout->addLayout(buttonsLayout, 10);
	setCaption(i18n( "Load Palette" ));
	resize(300, 155);
}
LoadPaletteDlg::~LoadPaletteDlg() {
}
	
void LoadPaletteDlg::setFileName(QString* fileName) {
	this->fileName = *fileName;
}

void LoadPaletteDlg::setFileName(int index) {
	setFileName(&palettesFileNames[index]);
}

void LoadPaletteDlg::browseFileNames() {
	QString fileToOpen = KFileDialog::getOpenFileName(lastOpenPaletteFileDir,
		i18n("*|All files"), this, i18n("Open File..."));
	if(!fileToOpen.isEmpty()) {
		fileName = fileToOpen;
		if(browseFileNameInserted) {
			paletteBox->removeItem(0);
			palettesFileNames.remove(palettesFileNames.begin());
		}
		paletteBox->insertItem(fileName, 0);
		paletteBox->setCurrentItem(0);
		palettesFileNames.prepend(fileName);
		browseFileNameInserted = true;
		lastOpenPaletteFileDir = fileName;
	}
}

QString LoadPaletteDlg::getFileName() {
	return fileName;
}
#include "loadpalettedlg.moc"

/***************************************************************************
                          loadpalettedlg.h  -  description
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

#ifndef LOADPALETTEDLG_H
#define LOADPALETTEDLG_H

#include <qcombobox.h>
#include <kdialog.h>

/**A dialog showing a list of installed palettes, with a possibility
  *of choosing a custom file
  *@author Artur Rataj
  */
class LoadPaletteDlg : public KDialogBase  {
	Q_OBJECT
	
public:
	/** constructs the dialog
	 */
	LoadPaletteDlg(QWidget *parent = 0, const char *name = 0);
	~LoadPaletteDlg();
	/** @return the fetched file name
	 */
	QString getFileName();
	
protected slots:
	/** sets fileName
	 */
	void setFileName(QString* fileName);	
	/** sets fileName to that at position index in palettesFileNames
	 */
	void setFileName(int index);	
	/** browses file names and if a file name fetched sets fileName
	 */
	void browseFileNames();	

private:
	/** A widget holding palettes names
	 */
	QComboBox* paletteBox;
	/** A list of KDE palettes file names
	 */
	QStringList palettesFileNames;
	/** A fetched palette file name
	 */
	QString fileName;
	/** whether a browse file name hab already been inserted into
	 *  palettesFilenames
	 */
	bool browseFileNameInserted;
};

#endif

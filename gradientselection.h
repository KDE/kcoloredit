/***************************************************************************
                          gradientselection.h  -  description
                             -------------------
    begin                : Wed Jul 12 2000
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

#ifndef GRADIENTSELECTION_H
#define GRADIENTSELECTION_H

#include <qwidget.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>

#include "kxycolorselector.h"
#include "kzcolorselector.h"
#include "color.h"

/** A widget for selecting colors from gradients
  * @author Artur Rataj
  */
class GradientSelection : public QWidget  {
   Q_OBJECT

public:
	/** Constructs the widget */
	GradientSelection(QWidget *parent=0, const char *name=0);
	~GradientSelection();
	/** Enables or disables the color synchronize button */
	void enableSynchronizeColorButton(bool enable);

signals:
	/** A signal that a color value has changed by edition */
	void valueChanged(Color*);
	/** Synchronizes the widget color */
	void synchronizeColor();

public slots:
	/** Sets a color */
	void slotSetValue(Color* color);
	/** Whether to ignore the set value slot. Default is false. */
	void slotIgnoreSetValue(bool ignore);

protected:
	/** Components indices. If the gradient selector shows one of them,
	 *  the two components selector should be in a mode indicated by a value
	 *  of the appropriate index
	 */
	enum { H_COMPONENT = KXYColorSelector::TYPE_VS,
	       S_COMPONENT = KXYColorSelector::TYPE_HV,
	       V_COMPONENT = KXYColorSelector::TYPE_HS };
	
	/** Variable global component checkbox */
	QCheckBox* variableCheckBox;
	/** Synchronize color button */
	QPushButton* synchronizeColorButton;
	/** HSV buttons button group widget */
	QButtonGroup hsvButtons;
	/** The two components selector */
	KXYColorSelector* xyColorSelector;
	/** The gradient selector */
	KZColorSelector* zColorSelector;
	/** Whether the two component color selector global component is variable */
	bool variableGlobalComponent;
	/** Whether to ignore the set value slot */
	bool ignoreSetValue;
	/** The selected color */
	Color color;
	/** The selected color H component */
	int hComponent;
	/** The selected color S component */
	int sComponent;
	/** The selected color V component */
	int vComponent;
	/** The gradient selector component index */
	int	zColorSelectorComponentIndex;
	/** Updates two component selector colors */
	void updateXyColorSelector(const bool modeChanged);
	/** Updates gradient selector colors */
	void updateZColorSelector();
	
protected slots:
	/** Sets color selection mode */
	void slotSetColorSelectionMode(int mode);
	/** Sets if the two component selector has a variable global component */
	void slotSetVariableGlobalComponent(bool variable);
	/** Notifies that the two component color selector pointer position changed */
	void slotXyColorSelectorPosChanged(int x, int y);
	/** Notifies that the gradient color selector pointer position changed */
	void slotZColorSelectorPosChanged(int y);
	/** sends synchronizeColor signal */
	void slotSynchronizeColor();
};

#endif

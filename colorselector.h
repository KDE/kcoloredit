/***************************************************************************
                          colorselector.h  -  description
                             -------------------
    begin                : Sun Jul 9 2000
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

#ifndef COLORSELECTOR_H
#define COLORSELECTOR_H

#include <qwidget.h>
#include <qcolor.h>
#include <qbuttongroup.h>
#include <kcolordialog.h>

#include "color.h"
#include "gradientselection.h"

/** Color selector widget
 *  @author Artur Rataj
 */
class ColorSelector : public QWidget  {
	Q_OBJECT

public:
	/** constructs a color selector widget */
	ColorSelector(QWidget *parent=0, const char *name=0);
	~ColorSelector();
	/** @return the selected color */
	const Color& color();

signals:
	/** A signal that a color value has changed */
	void valueChanged(Color*);
	
public slots:
	/** Called if a color changed */
	void slotSetColor(Color color);
	/** Called if a color changed */
	void slotSetColor(Color* color);
	/** called if a color changed in the color patch */
	void slotSetColor(const QColor& color);
	/** Called by the gradient selection, to replace or modify a color */
	void slotGradientSelectionChangeColor(Color* gradientSelectionColor);
	/** Called by the gradient selection, to synchronize its color */
	void slotGradientSelectionSynchronizeColor();
	/** Called if color replace mode is chosen */
	void slotColorReplace();
	/** called if color change mode is chosen */
	void slotColorChange();
	/** Called if a color change value changed */
	void slotColorChangeValueChanged(int value);
	
private:
	/** Color change mode constants */
	enum { MODE_REPLACE = 0,
	       MODE_CHANGE = 1,
	/** Maximum color change value */
	       MAX_COLOR_CHANGE_VALUE = 16 };
	
	/** A color change slider widget */
	QWidget* colorChangeSliderWidget;
	/** Color change buttons button group widget */
	QButtonGroup colorChangeButtons;
	/** A color patch widget */
	KColorPatch* colorPatch;
	/** A gradient selection widget */
	GradientSelection* gradientSelection;
	/** The current color */
	Color m_color;
	/** Color change mode */
	int colorChangeMode;
	/** Current color change value */
	int colorChangeValue;
	/** Floating--point precision RGB components, for color change mode */
	double fRComponent;
	double fGComponent;
	double fBComponent;
	/** Whether in the floating-point precision components mode */
	bool fComponentsMode;
	
	/** Scales a component according to componentDiff and colorChangeValue */
	void scaledComponent(double* const component, const double componentDiff);
};

#endif

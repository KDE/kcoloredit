/***************************************************************************
                          color.h  -  description
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

#ifndef COLOR_H
#define COLOR_H

#include "main.h"

/**A color class, holds its components and name
  *@author Artur Rataj
  */

class Color {
	public:
		/** indices of components
		 */
		enum { RED_INDEX = 0,
		       GREEN_INDEX = 1,
		       BLUE_INDEX = 2,
		       COMPONENTS_NUM = 3 };

	public:
		/** constructs a color
		 */
		Color();
		/** constructs a color with given components and a name
		 */
		Color(const int red, const int green, const int blue, const QString& name);
		~Color();
		/** sets a component
		 */
		void setComponent(const int index, const int value);
		/** sets components
		 */
		void setComponents(const int red, const int green, const int blue);
		/** sets a name
		 */
		void setName(const QString& name);
		/** @return a component
		 */
		int component(const int index) const;
		/** @return components
		 */
		const int* components() const;
		/** @return a color name
		 */
		const QString& name() const;
		/** @return if is equal to color
		 */
		bool equals(const Color& color);
		/** modifies a component, amount can be either positive or negative
		 */
		void modifyComponent(const int index, const int value, const double amount);
		/** modifies components, amount can be either positive or negative
		 */
		void modifyComponents(const int red, const int green, const int blue, const double amount);

	protected:
		/** components table
		 */
		int m_components[COMPONENTS_NUM];
		/** a color name
		 */
		QString m_name;
};

#endif

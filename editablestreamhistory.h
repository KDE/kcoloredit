/***************************************************************************
                          editablestreamhistory.h  -  description
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

#ifndef EDITABLESTREAMHISTORY_H
#define EDITABLESTREAMHISTORY_H

/** A template class adding undo/redo history for EDITABLE_STREAM paste and cut
  * methods
  * @author Artur Rataj
  */
template <class EDITABLE_STREAM> class EditableStreamHistory {
public:
	/** Constructs the class with stream as an EDITABLE_STREAM and
	 *  a given number of undo levels
	 */
	EditableStreamHistory(EDITABLE_STREAM* stream, const int undoLevels);
	~EditableStreamHistory();
	/** Cuts a stream at index, of length length.
	 *  Uses undo/redo history.
	 *  @return A stream that has been cut out
	 */
	EDITABLE_STREAM cut(const int index, const int length);
	/** Pastes a stream at index. Uses undo/redo history */
	void paste(const int index, EDITABLE_STREAM& pasteStream);
	/** Replaces a stream at index. Uses undo/redo history */
	void replace(const int index, EDITABLE_STREAM& replaceStream);
	/** @return Whether undo possible */
	bool undoPossible();
	/** @return Whether redo possible */
	bool redoPossible();
	/** Undoes if possible */
	void undo();
	/** Redoes if possible */
	void redo();
	/** @return A pointer to editableStream */
	EDITABLE_STREAM* editableStream();

protected:
	/** An editable stream */
	EDITABLE_STREAM* m_editableStream;
	/** A number of undo levels */
	int m_undoLevels;
};

template <class EDITABLE_STREAM> EditableStreamHistory<EDITABLE_STREAM>::
	EditableStreamHistory(EDITABLE_STREAM* stream, const int undoLevels) {
	m_editableStream = stream;
	m_undoLevels = undoLevels;
}
template <class EDITABLE_STREAM> EditableStreamHistory<EDITABLE_STREAM>::~EditableStreamHistory() {
}
	
template <class EDITABLE_STREAM> EDITABLE_STREAM
	EditableStreamHistory<EDITABLE_STREAM>::cut(const int index, const int length) {
	EDITABLE_STREAM cut_stream = m_editableStream->cut(index, length);
	return cut_stream;
}

template <class EDITABLE_STREAM> void
	EditableStreamHistory<EDITABLE_STREAM>::paste(const int index, EDITABLE_STREAM& pasteStream) {
	m_editableStream->paste(index, pasteStream);
}

template <class EDITABLE_STREAM> void
	EditableStreamHistory<EDITABLE_STREAM>::replace(const int index, EDITABLE_STREAM& replaceStream) {
	m_editableStream->cut(index, replaceStream.length());
	m_editableStream->paste(index, replaceStream);
}

template <class EDITABLE_STREAM> bool
	EditableStreamHistory<EDITABLE_STREAM>::undoPossible() {
	return false;
}

template <class EDITABLE_STREAM> bool
	EditableStreamHistory<EDITABLE_STREAM>::redoPossible() {
	return false;
}

template <class EDITABLE_STREAM> void
	EditableStreamHistory<EDITABLE_STREAM>::undo() {
}

template <class EDITABLE_STREAM> void
	EditableStreamHistory<EDITABLE_STREAM>::redo() {
}

template <class EDITABLE_STREAM> EDITABLE_STREAM*
	EditableStreamHistory<EDITABLE_STREAM>::editableStream() {
	return m_editableStream;
}

#endif

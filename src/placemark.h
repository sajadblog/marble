#ifndef PLACEMARK_H
#define PLACEMARK_H

// #include "geopoint.h"
// #include <QDebug>
#include <QPixmap>
#include <QRect>
#include "geopoint.h"
#include "placemarkstorage.h"

class PlaceMark : public PlaceMarkStorage {


public:
	PlaceMark();
	PlaceMark( QString );

	void coordinate( float& lng, float& lat );
	void setCoordinate( float lng, float lat );

	const int symbol() const { return m_symbol; }
	void setSymbol( int symbol ){ m_symbol = symbol; }

	const Quaternion& getQuatPoint() const { return m_coordinate.getQuatPoint(); }	

	const QPixmap textPixmap() const { return m_textPixmap; }
	const QRect textRect() const { return m_rect; }
	void setTextPixmap( QPixmap textPixmap ){ m_textPixmap = textPixmap; }
	void setTextRect( QRect textRect ){ m_rect = textRect;}
	void clearTextPixmap(){ if ( m_textPixmap.isNull() == false ) { m_textPixmap = QPixmap(); } }

private:
	GeoPoint m_coordinate;
	QPixmap m_textPixmap;
	QRect m_rect;
	int m_symbol;
};

#endif // PLACEMARK_H

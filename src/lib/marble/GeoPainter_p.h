//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2009 Torsten Rahn <tackat@kde.org>
// Copyright 2009 Bastian Holst <bastianholst@gmx.de>

#ifndef MARBLE_GEOPAINTERPRIVATE_H
#define MARBLE_GEOPAINTERPRIVATE_H

#include <QVector>

#include "MarbleGlobal.h"

class QPolygonF;
class QSizeF;
class QPainterPath;
class QRectF;

namespace Marble
{

class ViewportParams;
class GeoDataCoordinates;

class GeoPainterPrivate
{
 public:
    GeoPainterPrivate( const ViewportParams *viewport, MapQuality mapQuality );

    ~GeoPainterPrivate();


    void createAnnotationLayout ( qreal x, qreal y,
                                  QSizeF bubbleSize,
                                  qreal bubbleOffsetX, qreal bubbleOffsetY,
                                  qreal xRnd, qreal yRnd,
                                  QPainterPath& path, QRectF& rect );

    static GeoDataLinearRing createLinearRingFromGeoRect( const GeoDataCoordinates & centerCoordinates,
                                                          qreal width, qreal height );

    static bool doClip( const ViewportParams *viewport );

    const ViewportParams *const m_viewport;
    const MapQuality       m_mapQuality;
    qreal             *const m_x;
};

} // namespace Marble

#endif

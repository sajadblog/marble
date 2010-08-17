//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2007-2008 Inge Wallin  <ingwa@kde.org>
// Copyright 2007-2009 Torsten Rahn  <rahn@kde.org>
//


#ifndef MARBLE_ABSTRACTPROJECTION_H
#define MARBLE_ABSTRACTPROJECTION_H


/** @file
 * This file contains the headers for AbstractProjection.
 *
 * @author Inge Wallin  <inge@lysator.liu.se>
 * @author Torsten Rahn <rahn@kde.org>
 */

#include <QtCore/QRect>
#include <QtCore/QVector>
#include <QtGui/QPainterPath>
#include <QtGui/QPolygonF>

#include "GeoDataLatLonAltBox.h"
#include "GeoDataCoordinates.h"
#include "marble_export.h"

namespace Marble
{

// The manhattan distance in pixels at which extra nodes get created for tessellation.
static const int tessellationPrecision = 10;
static const int latLonAltBoxSamplingRate = 4;

class GeoDataLineString;
class ViewportParams;
class AbstractProjectionPrivate;


/**
 * @short A base class for all projections in Marble.
 */

class MARBLE_EXPORT AbstractProjection
{
    // Not a QObject so far because we don't need to send signals.
 public:
    enum SurfaceType {
        Cylindrical,
        Pseudocylindrical,
        Hybrid,
        Conical,
        Pseudoconical,
        Azimuthal
    };

    enum PreservationType {
        NoPreservation,
        Conformal,
        EqualArea
    };

    /**
     * @brief Construct a new AbstractProjection.
     */
    AbstractProjection();

    virtual ~AbstractProjection();

    virtual qreal  maxValidLat() const;

    qreal  maxLat()  const;
    void setMaxLat( qreal maxLat );

    virtual qreal  minValidLat() const;

    qreal  minLat()  const;
    void setMinLat( qreal minLat );

    virtual bool repeatableX() const;

    bool   repeatX() const;
    void   setRepeatX( bool repeatX );

    virtual bool   traversablePoles()  const        { return false; }
    virtual bool   traversableDateLine()  const     { return false; }

    virtual SurfaceType surfaceType() const = 0; 

    virtual PreservationType preservationType() const { return NoPreservation; }

    // The projection surface can have different orientations:
    // - normal: the surface's axis of symmetry matches the Earth's axis
    // - transverse: orthogonally oriented compared to the Earth's axis
    // - oblique: somewhere in between

    virtual bool   isOrientedNormal() const        { return true; }

    virtual void vertexCoordinates( const qreal lon, const qreal lat,
                                    qreal &x, qreal &y, qreal &z ) const = 0;

    /**
     * @brief Get the screen coordinates corresponding to geographical coordinates in the map.
     * @param lon    the lon coordinate of the requested pixel position in radians
     * @param lat    the lat coordinate of the requested pixel position in radians
     * @param viewport the viewport parameters
     * @param x      the x coordinate of the pixel is returned through this parameter
     * @param y      the y coordinate of the pixel is returned through this parameter
     * @return @c true  if the geographical coordinates are visible on the screen
     *         @c false if the geographical coordinates are not visible on the screen
     *
     * @see ViewportParams
     */
    virtual bool screenCoordinates( const qreal lon, const qreal lat,
                                    const ViewportParams *viewport,
                                    qreal& x, qreal& y ) = 0;

    /**
     * @brief Get the screen coordinates corresponding to geographical coordinates in the map.
     *
     * @param geopoint the point on earth, including altitude, that we want the coordinates for.
     * @param viewport the viewport parameters
     * @param x      the x coordinate of the pixel is returned through this parameter
     * @param y      the y coordinate of the pixel is returned through this parameter
     * @param globeHidesPoint  whether the point gets hidden on the far side of the earth
     *
     * @return @c true  if the geographical coordinates are visible on the screen
     *         @c false if the geographical coordinates are not visible on the screen
     *
     * @see ViewportParams
     */
    virtual bool screenCoordinates( const GeoDataCoordinates &geopoint, 
                                    const ViewportParams *viewport,
                                    qreal &x, qreal &y, 
                                    bool &globeHidesPoint ) = 0;

    // Will just call the virtual version with a dummy globeHidesPoint.
    bool screenCoordinates( const GeoDataCoordinates &geopoint, 
                            const ViewportParams *viewport,
                            qreal &x, qreal &y );

    bool screenCoordinates( const GeoDataCoordinates &geopoint,
                            const ViewportParams *viewport,
                            QPointF &screenpoint );

    /**
     * @brief Get the coordinates of screen points for geographical coordinates in the map.
     *
     * @param coordinates the point on earth, including altitude, that we want the coordinates for.
     * @param viewport the viewport parameters
     * @param x      the x coordinates of the pixels are returned through this parameter
     * @param y      the y coordinate of the pixel is returned through this parameter
     * @param pointRepeatNum      the amount of times that a single geographical
                                  point gets represented on the map
     * @param globeHidesPoint  whether the point gets hidden on the far side of the earth
     *
     * @return @c true  if the geographical coordinates are visible on the screen
     *         @c false if the geographical coordinates are not visible on the screen
     *
     * @see ViewportParams
     */
    bool screenCoordinates( const GeoDataCoordinates &coordinates,
                                    const ViewportParams *viewport,
                                    qreal *x, qreal &y, int &pointRepeatNum,
                                    bool &globeHidesPoint );

    virtual bool screenCoordinates( const GeoDataCoordinates &coordinates,
                                    const ViewportParams *viewport,
                                    qreal *x, qreal &y, int &pointRepeatNum,
                                    const QSizeF& size,
                                    bool &globeHidesPoint ) = 0;


    bool screenCoordinates( const GeoDataLineString &lineString,
                            const ViewportParams *viewport,
                            QVector<QPolygonF*> &polygons );

    /**
     * @brief Get the earth coordinates corresponding to a pixel in the map.
     * @param x      the x coordinate of the pixel
     * @param y      the y coordinate of the pixel
     * @param viewport the viewport parameters
     * @param lon    the longitude angle is returned through this parameter
     * @param lat    the latitude angle is returned through this parameter
     * @param unit   the unit of the angles for lon and lat.
     * @return @c true  if the pixel (x, y) is within the globe
     *         @c false if the pixel (x, y) is outside the globe, i.e. in space.
     */
    virtual bool geoCoordinates( const int x, const int y,
                                 const ViewportParams *viewport,
                                 qreal& lon, qreal& lat,
                                 GeoDataCoordinates::Unit unit = GeoDataCoordinates::Degree ) = 0;


    virtual GeoDataLatLonAltBox latLonAltBox( const QRect& screenRect,
                                                      const ViewportParams *viewport );

    bool exceedsLatitudeRange( const GeoDataCoordinates& coords ) const;

    bool exceedsLatitudeRange( const GeoDataLineString& lineString ) const;


    virtual bool mapCoversViewport( const ViewportParams *viewport ) const = 0;

    virtual QPainterPath mapShape( const ViewportParams *viewport ) const = 0;

    QRegion mapRegion( const ViewportParams *viewport ) const;

    // The usage of the following methods is DEPRECATED!

    /**
     * @deprecated
     * use @see screenCoordinates(qreal,qreal,const ViewportParams*,qreal&,qreal&)
     */
    bool screenCoordinates( qreal lon, qreal lat,
                                    const ViewportParams *viewport,
                                    int& x, int& y );

    /* DEPRECATED */
    bool screenCoordinates( const GeoDataCoordinates &geopoint, 
                                    const ViewportParams *viewport,
                                    int &x, int &y, bool &globeHidesPoint );

 protected:
    bool lineStringToPolygon( const GeoDataLineString &lineString,
                                    const ViewportParams *viewport,
                                    QVector<QPolygonF*> &polygons );

    virtual void horizonToPolygon( const ViewportParams *viewport,
                           const GeoDataCoordinates & disappearCoords,
                           const GeoDataCoordinates & reappearCoords,
                           QPolygonF* );

    // This method tessellates a line segment in a way that the line segment
    // follows great circles. The count parameter specifies the
    // number of nodes generated for the polygon. If the
    // clampToGround flag is added the polygon contains count + 2
    // nodes as the clamped down start and end node get added.

    void tessellateLineSegment(     const GeoDataCoordinates &aCoords,
                                    qreal ax, qreal ay,
                                    const GeoDataCoordinates &bCoords,
                                    qreal bx, qreal by,
                                    QPolygonF * polygon,
                                    const ViewportParams *viewport,
                                    TessellationFlags f = 0 );

 private:
    Q_DISABLE_COPY( AbstractProjection )
    AbstractProjectionPrivate * const d;
};

}

#endif

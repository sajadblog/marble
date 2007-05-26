//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2007 Torsten Rahn <tackat@kde.org>"
// Copyright 2007      Inge Wallin  <ingwa@kde.org>"
//

#include "katlasflag.h"

#include <QtCore/QDebug>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPixmapCache>
#include <QtSvg/QSvgRenderer>

#include "katlasdirs.h"


KAtlasFlag::KAtlasFlag( QObject* parent )
    : QObject(parent)
{
    QPixmapCache::setCacheLimit ( 384 );
}

void KAtlasFlag::setFlag( const QString& filename, const QSize& size )
{
    m_filename = filename;
    m_size     = size;
}


void KAtlasFlag::slotDrawFlag()
{
    QString  keystring = QString( m_filename ).replace( "flags/", "" );

    if ( !QPixmapCache::find( keystring, m_pixmap ) ) {
        QSvgRenderer  svgobj( m_filename, this );
        QSize         flagsize = svgobj.viewBox().size();
        flagsize.scale( m_size, Qt::KeepAspectRatio );

        m_pixmap = QPixmap( flagsize );
        m_pixmap.fill(Qt::transparent);

        QPainter painter( &m_pixmap );
        painter.setRenderHint( QPainter::Antialiasing, true );

        QRect  viewport( QPoint( 0, 0 ), flagsize );
        painter.setViewport( viewport );
        svgobj.render( &painter );
        QPixmapCache::insert( keystring, m_pixmap );
    }

    emit flagDone();
}


#ifndef Q_OS_MACX
#include "katlasflag.moc"
#endif

/***********************************************************************
*
* BTK Painting type aliases.
* Clean BTK-branded API surface over Qt painting/rendering types.
*
***********************************************************************/

#ifndef BTK_PAINTING_ALIASES_H
#define BTK_PAINTING_ALIASES_H

#include <qpainter.h>
#include <qcolor.h>
#include <qpen.h>
#include <qbrush.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qicon.h>
#include <qbitmap.h>
#include <qregion.h>
#include <qpolygon.h>
#include <qtransform.h>
#include <qmatrix.h>
#include <qgradient.h>
#include <qpainterpath.h>

using BTKPainter     = QPainter;
using BTKColor       = QColor;
using BTKPen         = QPen;
using BTKBrush       = QBrush;
using BTKFont        = QFont;
using BTKFontMetrics = QFontMetrics;
using BTKImage       = QImage;
using BTKPixmap      = QPixmap;
using BTKIcon        = QIcon;
using BTKBitmap      = QBitmap;
using BTKRegion      = QRegion;
using BTKPolygon     = QPolygon;
using BTKPolygonF    = QPolygonF;
using BTKTransform   = QTransform;
using BTKMatrix      = QMatrix;
using BTKGradient    = QGradient;
using BTKPath        = QPainterPath;
#endif

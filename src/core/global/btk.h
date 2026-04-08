/***********************************************************************
*
* BTK application framework umbrella header.
*
* Including <btk.h> gives access to the entire BTK framework surface
* through BTK-branded aliases:
*
*   Core:    BtkString, BtkSharedPointer, BtkSignal, BtkInputOwner
*            BTKTimer, BTKThread, BTKFile, BTKVariant, etc.
*   Gui:     Widgets, painting, layout, input, windowing, focus
*            BTKWidget, BTKButton, BTKPainter, BTKColor, etc.
*   Network: TCP/UDP, HTTP, SSL/TLS
*            BTKTcpSocket, BTKNetworkManager, etc.
*   Sql:     Database access
*            BTKDatabase, BTKQuery, etc.
*   Xml:     XML parsing (DOM, SAX, streaming)
*   Svg:     SVG rendering
*   OpenGL:  Cross-platform OpenGL
*   Vulkan:  Cross-platform Vulkan
*
* For smaller compilation units, include only the specific BTK headers
* you need: <btkcore.h>, <btkgui.h>, <btknetwork.h>, etc.
*
* IMPORTANT: This header includes GUI types. For core-only usage,
* use <btkcore.h> instead.
*
***********************************************************************/

#ifndef BTK_FRAMEWORK_H
#define BTK_FRAMEWORK_H

#include <btkcore.h>
#include <btkgui.h>
#include <btknetworktypes.h>
#include <btksqltypes.h>

#endif

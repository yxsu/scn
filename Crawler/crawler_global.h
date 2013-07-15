#ifndef CRAWLER_GLOBAL_H
#define CRAWLER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CRAWLER_LIBRARY)
#  define CRAWLERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CRAWLERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CRAWLER_GLOBAL_H

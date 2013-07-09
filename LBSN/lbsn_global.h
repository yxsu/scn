#ifndef LBSN_GLOBAL_H
#define LBSN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LBSN_LIBRARY)
#  define LBSNSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LBSNSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LBSN_GLOBAL_H
